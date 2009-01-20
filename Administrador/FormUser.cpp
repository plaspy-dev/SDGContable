// FormUser.cpp : implementation file
//

#include "stdafx.h"
# include "administrador.h"
#include "FormUser.h"
# include "iofile/iofile.h"
#include "DlgEditar.h"
# include "DlgPasswd.h"
// CFormUser

IMPLEMENT_DYNCREATE(CFormUser, CConectorFormView)

CFormUser::CFormUser()
	: CConectorFormView(CFormUser::IDD)
{
	m_nCurrentGroup= -1;
}

CFormUser::~CFormUser()
{
	for(int i=0; i<m_pUsuarios.GetCount(); i++) {
		CUsuario *info= (CUsuario *)m_pUsuarios[i];
		delete info;
	}
	m_pUsuarios.RemoveAll();
}

void CFormUser::DoDataExchange(CDataExchange* pDX)
{
	CConectorFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTGRUPOS, m_LCGrupos);
	DDX_Control(pDX, IDC_LISTUSUARIOS, m_LCUser);
	DDX_Control(pDX, IDC_BUTTONNUEVO, m_BNuevo);
}

BEGIN_MESSAGE_MAP(CFormUser, CConectorFormView)
	ON_BN_CLICKED(IDC_BUTTONNUEVO, OnBnClickedButtonnuevo)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTGRUPOS, OnNMDblclkListgrupos)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTUSUARIOS, OnNMDblclkListusuarios)
	ON_NOTIFY(NM_RCLICK, IDC_LISTUSUARIOS, OnNMRclickListusuarios)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LISTGRUPOS, OnNMRclickListgrupos)
	ON_COMMAND( 32000,OnVer)
	ON_COMMAND( 32001,OnBnClickedButtonnuevo)
	ON_COMMAND( 22000,OnEditar)
	ON_COMMAND( 22001,OnContrasena)
	ON_COMMAND( 22002,OnEliminar)
END_MESSAGE_MAP()


// CFormUser diagnostics

#ifdef _DEBUG
void CFormUser::AssertValid() const
{
	CConectorFormView::AssertValid();
}

void CFormUser::Dump(CDumpContext& dc) const
{
	CConectorFormView::Dump(dc);
}
#endif //_DEBUG


// CFormUser message handlers

void CFormUser::OnBnClickedButtonnuevo()
{
	int sel;
	sel= m_LCGrupos.SelectedIndex;
	if( sel==-1)
		sel=0;
	Editar(NULL, m_nCurrentGroup);
}

void CFormUser::OnVer() 
{
	FillLista(m_LCGrupos.SelectedIndex);
}

void CFormUser::OnEditar()
{
	int pos= m_LCUser.SelectedIndex;
	if( pos!=-1) 
		Editar((CUsuario *)m_LCUser.GetItemData(pos));
}

void CFormUser::OnEliminar()
{
	if(!m_pUsuario3->CanDelete(this)) 
		return;
	int pos= m_LCUser.GetNextItem(-1, LVIS_SELECTED);
	if( pos!=-1) { // eliminar usuario
		CUsuario *info= (CUsuario *)m_LCUser.GetItemData(pos);
		if( info) {
			if( MessageBox("Esta seguro de eliminar a\r\n" + info->GetNombre(), NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {

				EliminarUsuario(info->GetNombre());
				CString temp;
				temp.Format("UPDATE usuario SET estado=-1 WHERE id=%d", info->GetID());
				m_pConector->Query(temp, 0, this);
				info->SetId(0);
				m_LCUser.DeleteItem(pos);
			}
		}
	}
}

void CFormUser::CambiarContrasena(CString usuario, CString newpasswd) {

	CString encriptado= CUsuario::GetCodificado(usuario);
	CString query;

	query.Format("SET PASSWORD FOR '%s'=PASSWORD('%s')", encriptado, newpasswd);
	m_pConector->Query(query, 0, this);
	m_pConector->Query("FLUSH PRIVILEGES", 0, this);
}

void CFormUser::EliminarUsuario(CString usuario) {
	
	CString temp;
	CString encriptado;
	// usuario encriptado en md5
	encriptado= CUsuario::GetCodificado(usuario);
	// borramos de la tabla de usuarios
	temp.Format( "DELETE FROM mysql.user WHERE User=\"%s\" OR User=\"%s\"", usuario, encriptado);
	m_pConector->Query( temp, 0, this);
	// borramos de la tabla de bases de datos al usuario
	temp.Format( "DELETE FROM mysql.db WHERE User=\"%s\" OR User=\"%s\"", usuario, encriptado);
	m_pConector->Query( temp, 0, this);
	m_pConector->Query("FLUSH PRIVILEGES", 0, this);
}

void CFormUser::AgregarUsuario(CUsuario *usuario, CString passwd) {

	CString metodo,realpasswd;
	
	for( int i=0; i<m_uaIDS.GetCount(); i++) {
		if( usuario->GetGrupo()==m_uaIDS.GetAt(i)) {
			if(m_uaMetodos[i]>31) { // administrador
				metodo="GRANT ALL PRIVILEGES ON *.* TO '%0' %1 WITH GRANT OPTION";
			} else if(m_uaMetodos[i]>15) { // borrado
				metodo="GRANT SELECT,INSERT,UPDATE,DELETE,EXECUTE,CREATE TEMPORARY TABLES ON %2.* TO '%0' %1";
			} else if(m_uaMetodos[i]>7) { // actualizacion
				metodo="GRANT SELECT,INSERT,UPDATE,DELETE,EXECUTE,CREATE TEMPORARY TABLES ON %2.* TO '%0' %1";
			} else if(m_uaMetodos[i]>3) { // insertar
				metodo="GRANT SELECT,INSERT,EXECUTE,CREATE TEMPORARY TABLES ON %2.* TO '%0' %1";
			} else { // solo lectura e impresion
				metodo="GRANT SELECT,EXECUTE,CREATE TEMPORARY TABLES ON %2.* TO '%0' %1";
			}
			break;
		}
	}

	if( metodo.IsEmpty())
		return;

	if( !passwd.IsEmpty()) // realizar grant con contrasena
		realpasswd.Format(" IDENTIFIED BY '%s' ", passwd);
	// o simplemente grant sencillo

	CString encriptado= usuario->GetCodificado();

	metodo.Replace("%0", encriptado);
	metodo.Replace("%1", realpasswd);
	metodo.Replace("%2", m_pConector->GetNombreDatabase());
	m_pConector->Query(metodo, 0, this);
}

void CFormUser::OnContrasena()
{
	CDlgPasswd dlg;
	CString temp, query;

	if( dlg.DoModal()==IDOK) {
		temp= dlg.m_szContrasena;
		int pos= m_LCUser.GetNextItem(-1, LVIS_SELECTED);
		if( pos!=-1) {
			CUsuario *info= (CUsuario *)m_LCUser.GetItemData(pos);
			CambiarContrasena(info->GetNombre(), temp);
		}
	}
}

void CFormUser::Editar(CUsuario *usuario, int grupo) {
	
	CDlgEditar dlg(m_pUsuario3);

	dlg.m_pActual= usuario;	
	dlg.m_dwGrupo= usuario ? usuario->GetGrupo() : grupo;
	if( usuario) {
		dlg.m_szContrasena="-1";
	}
	dlg.m_szNombreCompleto= usuario ? usuario->GetDescripcion() : "";
	dlg.m_saNombres= &m_saGrupos;
	dlg.m_uaIDS= &m_uaIDS;

	if(dlg.DoModal()==IDOK) {

		CString temp;
		int posicion;
		CQuery query("usuario");

		query["login"]= dlg.m_szNombre;
		query["nombre"]= dlg.m_szNombreCompleto;
		temp.Format("%d", dlg.m_dwGrupo);
		query["grupo"] =temp;

		if(usuario) {

			usuario->SetUser(dlg.m_szNombre);
			usuario->SetDescripcion(dlg.m_szNombreCompleto);
			usuario->SetGrupo(dlg.m_dwGrupo);

			temp.Format("id='%d'", usuario->GetID());
			m_pConector->Query(query.GetUpdate(temp));
		} else {
			
			CStringArray valores;
			if( m_pConector->Find(query.GetInsert(), this, &valores)>0) {
				
				usuario= new CUsuario(dlg.m_szNombre, atoi(valores[0]), dlg.m_dwGrupo);
				usuario->SetDescripcion(dlg.m_szNombreCompleto);
				m_pUsuarios.Add(usuario);
			}
		}
		
		// este es el grant
		AgregarUsuario(usuario, dlg.m_szContrasena);
		posicion= m_nCurrentGroup;
		m_nCurrentGroup=-1;
		FillLista(posicion);
	}
}

LRESULT CFormUser::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch ( message) {

		case WM_MYSQL:
			{
				CMySQLMessage query(lParam);

				if( wParam==QUERY_RESULT) {

					if(query.GetID()== query_usuarios) {

						CString nombre= query["login"];
						DWORD	id= atol(query["id"]);
						CUsuario *info= new CUsuario(nombre, id, atol(query["grupo"]));
						m_pUsuarios.Add(info);
						info->SetDescripcion(query["nombre"]);

					} else if(query.GetID()==query_grupos) {
						int pos= m_LCGrupos.AddItem(query["nombre"]);
						m_LCGrupos.SetItemText(pos, 1, query["descripcion"]);
						m_LCGrupos.SetItemData(pos, atoi(query["id"]));

						m_uaIDS.Add(atoi(query["id"]));
						m_uaMetodos.Add(atoi(query["permiso"]));
						m_saGrupos.Add(query["nombre"]);
					}
				} else if( wParam==THREAD_END) {
					if(query.GetID()==query_usuarios) {
						m_LCUser.SetMessageLista("No hay Usuarios");
						FillLista(0);
					}
					// invalidar toda la seleccion
				}
			}
			return 0;
	}
	return CConectorFormView::WindowProc(message, wParam, lParam);
}

void CFormUser::OnInitialUpdate()
{
	CConectorFormView::OnInitialUpdate();
	GetDocument()->SetTitle("Usuarios");

	m_LCGrupos.InsertColumn(0, "Grupo");
	m_LCGrupos.InsertColumn(1, "Descripción");
	m_LCGrupos.SetItemText(m_LCGrupos.AddItem("Administradores"), 1, "Grupo de administradores");
	m_LCGrupos.SelectedIndex=0;

	m_uaIDS.Add(0);
	m_uaMetodos.Add(35);
	m_saGrupos.Add("Administradores");


	m_LCUser.InsertColumn(0, "Usuario", LVCFMT_LEFT, 100);
	m_LCUser.InsertColumn(1, "Nombre completo", LVCFMT_LEFT, 100);
	m_LCUser.InsertColumn(2, "Grupo", LVCFMT_LEFT, 100);
	m_LCUser.SetMessageLista("Cargando Usuarios");
	m_LCUser.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_LCUser.SetOrdenable();
	m_LCGrupos.AutoSizeColumns();

	CQuery grupos("grupo");

	grupos["id"]="grupo.id";
	grupos["nombre"]="grupo.nombre";
	grupos["descripcion"]="grupo.descripcion";
	grupos["permiso"]="MAX(permisos.permiso)";

	m_pConector->Query(grupos.GetSelect("", "LEFT JOIN permisos ON (permisos.grupo=grupo.id) GROUP BY grupo.id"), query_grupos, this);

	// usuarios
	CQuery query("usuario");

	query+="id";
	query+="nombre";
	query+="login";
	query+="grupo";

	m_pConector->Query(query.GetSelect("estado IS NULL OR estado!=-1"), query_usuarios, this);
	m_BNuevo.EnableWindow(m_pUsuario3->CanInsert(this));
}

void CFormUser::AddLista(CUsuario *Usuario) {

	int pos= m_LCUser.InsertItem(m_LCUser.GetItemCount(), Usuario->GetNombre());
	m_LCUser.SetItemText(pos, 1, Usuario->GetDescripcion());
	for(int i=0; i<m_LCGrupos.GetItemCount(); i++) {
		if(m_LCGrupos.GetItemData(i)==Usuario->GetGrupo()) {
			m_LCUser.SetItemText(pos, 2, m_LCGrupos.GetItemText(i, 0));
			break;
		}
	}
	m_LCUser.SetItemData(pos, (DWORD_PTR)Usuario);
}

void CFormUser::FillLista(int grupo) {

	if( grupo==m_nCurrentGroup)
		return;

	m_nCurrentGroup=grupo;
	CUsuario *info;

	m_LCUser.DeleteAllItems();

	for( int i=0; i<m_pUsuarios.GetCount(); i++) {
		info= (CUsuario *)m_pUsuarios[i];
		if( info && info->GetID() && info->GetGrupo()==m_nCurrentGroup) 
			AddLista(info);
	}
	m_LCUser.AutoSizeColumns();
}

void CFormUser::OnNMDblclkListgrupos(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1)
		return;
	FillLista(m_LCGrupos.GetItemData(pNMListView->iItem));
	*pResult = 0;
}


void CFormUser::OnNMDblclkListusuarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	OnEditar();
	*pResult = 0;
}

void CFormUser::OnNMRclickListusuarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	CNewMenu menu;

	menu.CreatePopupMenu();
	menu.InsertMenu(0, MF_BYPOSITION | MF_STRING, 22000, "&Editar");
	menu.InsertMenu(1, MF_BYPOSITION | MF_STRING, 22001, "&Cambiar Contraseña");
	menu.InsertMenu(2, MF_BYPOSITION | MF_STRING, 22002, "E&liminar");
	
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CFormUser::OnSize(UINT nType, int cx, int cy)
{
	CConectorFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_LCGrupos.GetSafeHwnd())) {
		CRect rect;
		m_LCGrupos.MoveWindow(5, 5, cx/4-5, cy-5);
		m_BNuevo.GetWindowRect(rect);
		m_BNuevo.MoveWindow(cx-rect.Width()-10, cy-rect.Height()-10,rect.Width(), rect.Height());
		m_LCUser.MoveWindow(cx/4+5, 5, 3*cx/4-5, cy-rect.Height()-20);
	}
}

void CFormUser::OnNMRclickListgrupos(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;

	CNewMenu menu;

	menu.CreatePopupMenu();
	
	menu.InsertMenu(0, MF_BYPOSITION | MF_STRING, 32000, "&Ver");
	menu.InsertMenu(1, MF_BYPOSITION | MF_STRING, 32001, "&Nuevo");
	
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CFormUser::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if(bActivate) {
		AfxGetMainWnd()->SendMessage(WM_USER+5000,3, (LPARAM) (const char *)"administrador\\usuarios.htm");
		AfxGetMainWnd()->SendMessage(WM_USER+5000,4, (LPARAM) (const char *)"administrador\\nuevo.htm");
	}
	CConectorFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
