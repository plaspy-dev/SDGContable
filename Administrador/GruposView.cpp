// GruposView.cpp : implementation file
//

#include "stdafx.h"
#include "Administrador.h"
#include "GruposView.h"


// CGruposView

IMPLEMENT_DYNCREATE(CGruposView, CConectorFormView)

CGruposView::CGruposView()
	: CConectorFormView(CGruposView::IDD)
{
	m_dwCurrentGrupo=0;
}

CGruposView::~CGruposView()
{
}

void CGruposView::DoDataExchange(CDataExchange* pDX)
{
	CConectorFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LGRUPO, m_LCGrupo);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_EDESCRIPCION, m_EDescripcion);
	DDX_Control(pDX, IDC_LPERMISOS, m_LCPermisos);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDOK, m_BAceptar);
}

BEGIN_MESSAGE_MAP(CGruposView, CConectorFormView)
	ON_NOTIFY(NM_DBLCLK, IDC_LGRUPO, &CGruposView::OnNMDblclkLgrupo)
	ON_NOTIFY(NM_RCLICK, IDC_LGRUPO, &CGruposView::OnNMRClickLgrupo)
	ON_BN_CLICKED(IDOK, &CGruposView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGruposView::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_LPERMISOS, &CGruposView::OnNMClickLpermisos)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LPERMISOS, &CGruposView::OnLvnKeydownLpermisos)
	ON_COMMAND(2000, OnEditar)
	ON_COMMAND(2001, OnEliminar)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LPERMISOS, &CGruposView::OnLvnColumnclickLpermisos)
	ON_MESSAGE(WM_LISTANOTIFICACIONEX, OnListaNotifiacion)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
END_MESSAGE_MAP()


// CGruposView diagnostics

#ifdef _DEBUG
void CGruposView::AssertValid() const
{
	CConectorFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGruposView::Dump(CDumpContext& dc) const
{
	CConectorFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGruposView message handlers

void CGruposView::OnInitialUpdate()
{
	AddAnchor(IDC_LGRUPO, TOP_LEFT,BOTTOM_LEFT);
	
	AddAnchor(IDC_SNOMBRE, TOP_LEFT);
	AddAnchor(IDC_SDESCRIPCION, TOP_LEFT);
	AddAnchor(IDC_ENOMBRE, TOP_LEFT,TOP_RIGHT);
	AddAnchor(IDC_EDESCRIPCION, TOP_LEFT,TOP_RIGHT);

	AddAnchor(IDCANCEL, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);

	AddAnchor(IDC_LPERMISOS, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_SPERMISOS, TOP_LEFT, BOTTOM_RIGHT);
	

	CConectorFormView::OnInitialUpdate();
	GetDocument()->SetTitle("Grupos");

	m_LCGrupo.AddColumn("Grupo");
	m_LCGrupo.AddColumn("Descripción");
	m_LCGrupo.SetItemText(m_LCGrupo.AddItem("Administradores"), 1, "Grupo de administradores");
	m_LCGrupo.SetOrdenable();

	m_LCPermisos.AddColumn("Ventana");
	m_LCPermisos.AddColumn("Administrador");
	m_LCPermisos.AddColumn("Borrado");
	m_LCPermisos.AddColumn("Edición");
	m_LCPermisos.AddColumn("Inserción");
	m_LCPermisos.AddColumn("Impresión");
	m_LCPermisos.AddColumn("Lectura");

	m_LCPermisos.SetOrdenable();
	
	m_LCPermisos.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	CQuery grupos("grupo");

	grupos+="id";
	grupos+="nombre";
	grupos+="descripcion";

	m_pConector->Query(grupos.GetSelect(), query_grupos, this);

	CTreeCtrlXP *nav= &((CMDIFrameWndXP *) AfxGetMainWnd())->GetNavigateBar()->m_Tree;
	HTREEITEM parent=nav->GetRootItem();
	while(parent) {
		HTREEITEM item= nav->GetChildItem(parent);
		while(item) {
			int pos= m_LCPermisos.AddItem(nav->GetItemText(parent) + " - " + nav->GetItemText(item));
			m_LCPermisos.SetItemData(pos, nav->GetItemData(item));
			item= nav->GetNextSiblingItem(item);
		}
		parent= nav->GetNextSiblingItem(parent);
	}
	m_LCPermisos.AutoSizeColumns();
	m_BAceptar.EnableWindow(m_pUsuario3->CanInsert(this));
}

void CGruposView::OnNMDblclkLgrupo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	OnEditar();
	*pResult = 0;
}

void CGruposView::OnEditar() {

	int index=m_LCGrupo.SelectedIndex;
	if(index==-1)
		return;
	OnBnClickedCancel();
	m_ENombre.Text= m_LCGrupo.GetItemText(index, 0);
	m_EDescripcion.Text= m_LCGrupo.GetItemText(index, 1);
	m_dwCurrentGrupo=m_LCGrupo.GetItemData(index);

	if(!m_LCGrupo.GetItemData(index)) {

		for(int i=0;i<m_LCPermisos.GetItemCount(); i++) {
			m_LCPermisos.SetCheck(i, true);
			for(int j=1;j<m_LCPermisos.GetColumnCount(); j++) {
				m_LCPermisos.SetItemText(i, j, "X");
			}
		}

		MessageBox("El grupo administrador es de solo lectura.", NULL, MB_ICONINFORMATION);
		m_BAceptar.EnableWindow(false);
		return;
	}

	m_BAceptar.EnableWindow(m_pUsuario3->CanUpdate(this));

	CString temp;
	CQuery query("permisos");

	query+="clase";
	query+="permiso";

	temp.Format("grupo='%d'", m_LCGrupo.GetItemData(index));

	m_pConector->Query(query.GetSelect(temp), query_permisos, this);
}

void CGruposView::OnEliminar() {
	
	if(!m_pUsuario3->CanDelete(this)) 
		return;
	int index=m_LCGrupo.SelectedIndex;
	if(index==-1)
		return;
	if(!m_LCGrupo.GetItemData(index)) {
		MessageBox("El grupo administrador no se puede eliminar.", NULL, MB_ICONERROR);
		return;
	}
	CString temp;
	temp.Format("DELETE FROM permisos WHERE grupo='%d'", m_LCGrupo.GetItemData(index));
	m_pConector->Query(temp);
	temp.Format("DELETE FROM grupo WHERE id='%d'", m_LCGrupo.GetItemData(index));
	m_pConector->Query(temp);	
	m_LCGrupo.DeleteItem(index);
}

void CGruposView::OnNMRClickLgrupo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	if(pNMItemActivate->iItem!=-1) {
		
		CNewMenu menu;

		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 2000, "&Editar");
		menu.AppendMenu(MF_STRING, 2001, "&Eliminar");
		menu.SetDefaultItem(0, TRUE);

		POINT point;
		GetCursorPos( &point );
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
	}
	*pResult = 0;
}

LRESULT CGruposView::OnMySQLMessage(WPARAM wParam,LPARAM lParam) {

	CMySQLMessage query(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(query.GetID()==query_grupos) {
				int pos= m_LCGrupo.AddItem(query["nombre"]);
				m_LCGrupo.SetItemText(pos, 1, query["descripcion"]);
				m_LCGrupo.SetItemData(pos, atoi(query["id"]));
			} else if(query.GetID()==query_permisos) {
				for(int i=0; i<m_LCPermisos.GetItemCount(); i++) {
					CMultiDocTemplateXP *temp=(CMultiDocTemplateXP *)m_LCPermisos.GetItemData(i);
					if(temp->GetViewClassName()==query["clase"]) {
						int permiso= atoi(query["permiso"]);
						if(permiso&1)
							m_LCPermisos.SetItemText(i, 6, "X");
						if(permiso&2)
							m_LCPermisos.SetItemText(i, 5, "X");
						if(permiso&4)
							m_LCPermisos.SetItemText(i, 4, "X");
						if(permiso&8)
							m_LCPermisos.SetItemText(i, 3, "X");
						if(permiso&16)
							m_LCPermisos.SetItemText(i, 2, "X");
						if(permiso&32)
							m_LCPermisos.SetItemText(i, 1, "X");
						break;
					}
				}
			}
			break;
		case QUERY_END:
			if(query.GetID()==query_grupos) {
				m_LCGrupo.AutoSizeColumns();
			}
			break;
	}
	return 0;
}

void CGruposView::OnBnClickedOk()
{
	CQuery query("grupo");
	CString temp;
	int pos=-1;

	if(m_ENombre.Text.IsEmpty()) {
		SetErrorEx(&m_ENombre, "Por favor ingrese un nombre.", 2, "GruposView");
		return;
	}

	query["nombre"]=m_ENombre.Text;
	query["descripcion"]= m_EDescripcion.Text;

	if(m_dwCurrentGrupo) {
		temp.Format("id='%d'", m_dwCurrentGrupo);
		m_pConector->Query(query.GetUpdate(temp));
		for(int i=0; i<m_LCGrupo.GetItemCount(); i++) {
			if(m_LCGrupo.GetItemData(i)==m_dwCurrentGrupo) {
				pos=i;
				break;
			}
		}
	} else {

		CStringArray valores;
		if(m_pConector->Find(query.GetInsert(), this, &valores)<0) {
			SetErrorEx(NULL, valores[0], 3, "mysql");
			return;
		}
		m_dwCurrentGrupo= atoi(valores[0]);
		pos= m_LCGrupo.AddItem("");
		m_LCGrupo.SetItemData(pos, m_dwCurrentGrupo);
	}

	CString id;
	id.Format("%d", m_dwCurrentGrupo);

	temp.Format("DELETE FROM permisos WHERE grupo='%d'", m_dwCurrentGrupo);
	// eliminamos los anteriores
	m_pConector->Query(temp);

	for(int i=0; i<m_LCPermisos.GetItemCount(); i++) {

		CQuery permisos("permisos");
		int permiso=0;
		CMultiDocTemplateXP *temporal= (CMultiDocTemplateXP *)m_LCPermisos.GetItemData(i);

		if(!m_LCPermisos.GetItemText(i, 6).IsEmpty())
			permiso|= 1; // lectura
		if(!m_LCPermisos.GetItemText(i, 5).IsEmpty())
			permiso|= 3; // impresion
		if(!m_LCPermisos.GetItemText(i, 4).IsEmpty())
			permiso|= 5; // insertar
		if(!m_LCPermisos.GetItemText(i, 3).IsEmpty())
			permiso|= 9; // editar
		if(!m_LCPermisos.GetItemText(i, 2).IsEmpty())
			permiso|= 31; // borrar
		if(!m_LCPermisos.GetItemText(i, 1).IsEmpty())
			permiso|= 63; // administrar

		temp.Format("%d", permiso);

		permisos["grupo"]= id;
		permisos["clase"]= temporal->GetViewClassName();
		permisos["permiso"]= temp;

		m_pConector->Query(permisos.GetInsert());
	}
		
	m_LCGrupo.SetItemText(pos, 0, m_ENombre.Text);
	m_LCGrupo.SetItemText(pos, 1, m_EDescripcion.Text);

	OnBnClickedCancel();
}

void CGruposView::OnBnClickedCancel()
{
	m_EDescripcion.Text="";
	m_ENombre.Text="";
	for(int i=0; i<m_LCPermisos.GetItemCount(); i++) {
		m_LCPermisos.SetCheck(i, false);
		for(int j=1; j<m_LCPermisos.GetColumnCount(); j++) {
			m_LCPermisos.SetItemText(i, j, "");
		}
	}
	m_dwCurrentGrupo=0;
	m_ENombre.SetFocus();
	m_BAceptar.EnableWindow(m_pUsuario3->CanInsert(this));
}

void CGruposView::OnNMClickLpermisos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE *>(pNMHDR);
	if(pNMItemActivate->iItem!=-1) {
		if(pNMItemActivate->iSubItem>0) {
			if(m_LCPermisos.GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem).IsEmpty()) {
				for(int i=pNMItemActivate->iSubItem; i<m_LCPermisos.GetColumnCount(); i++) {
					m_LCPermisos.SetItemText(pNMItemActivate->iItem, i, "X");
				}
				for(int i=1; i<m_LCPermisos.GetColumnCount(); i++) {
					if(m_LCPermisos.GetItemText(pNMItemActivate->iItem, i).IsEmpty()) {
						*pResult=0;
						return ;
					}
				}
				m_LCPermisos.SetCheck(pNMItemActivate->iItem, 1);
			} else {
				m_LCPermisos.SetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem, "");
				m_LCPermisos.SetCheck(pNMItemActivate->iItem, 0);
			}
		} else {
			for(int i=1; i<m_LCPermisos.GetColumnCount(); i++) {
				if(m_LCPermisos.GetCheck(pNMItemActivate->iItem))
					m_LCPermisos.SetItemText(pNMItemActivate->iItem, i, "X");
				else m_LCPermisos.SetItemText(pNMItemActivate->iItem, i, "");
			}
		}
	}
	*pResult = 0;
}

void CGruposView::OnLvnKeydownLpermisos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if(pLVKeyDow->wVKey==VK_SPACE) {
		int index=m_LCPermisos.SelectedIndex;
		if(index!=-1) {
			for(int i=1; i<m_LCPermisos.GetColumnCount(); i++) {
				if(!m_LCPermisos.GetCheck(index))
					m_LCPermisos.SetItemText(index, i, "X");
				else m_LCPermisos.SetItemText(index, i, "");
			}
		}
	}
	*pResult = 0;
}

LRESULT CGruposView::OnListaNotifiacion(WPARAM wParam, LPARAM lParam) {

	int col= LOWORD(wParam);
	int row= (signed short)HIWORD(wParam);
	if(lParam==m_LCPermisos.GetDlgCtrlID()) {
		if(row==-1) {
			for(int i=0; i<m_LCPermisos.GetItemCount(); i++) {
				if(m_LCPermisos.GetItemText(i, col).IsEmpty())
					m_LCPermisos.SetItemText(i, col, "X");
				else m_LCPermisos.SetItemText(i, col, "");
			}
		}		
	}
	return 0;
}

void CGruposView::OnLvnColumnclickLpermisos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}
