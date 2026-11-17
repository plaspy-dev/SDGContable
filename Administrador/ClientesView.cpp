// ClientesView.cpp : implementation file
//

#include "stdafx.h"
#include "Administrador.h"
#include "ClientesView.h"


// CClientesView

IMPLEMENT_DYNCREATE(CClientesView, CConectorFormView)

CClientesView::CClientesView()
	: CConectorFormView(CClientesView::IDD)
{
	m_dwCurrentID=0;
}

CClientesView::~CClientesView()
{
}

void CClientesView::DoDataExchange(CDataExchange* pDX)
{
	CConectorFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LENTIDADES, m_LCEntidades);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_ESERVICIOS, m_EDescripcion);
	DDX_Control(pDX, IDC_ENIT, m_ENit);
	DDX_Control(pDX, IDC_ETELEFONO, m_ETelefono);
	DDX_Control(pDX, IDC_EDIRECCION, m_EDireccion);
	DDX_Control(pDX, IDC_EEMAIL, m_EEMail);
	DDX_Control(pDX, IDC_ECONTACTO, m_EContacto);
}

BEGIN_MESSAGE_MAP(CClientesView, CConectorFormView)
	ON_NOTIFY(NM_DBLCLK, IDC_LENTIDADES, &CClientesView::OnNMDblclkLentidades)
	ON_NOTIFY(NM_RCLICK, IDC_LENTIDADES, &CClientesView::OnNMRClickLentidades)
	ON_BN_CLICKED(IDOK, &CClientesView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CClientesView::OnBnClickedCancel)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_COMMAND(3000, OnEditar)
	ON_COMMAND(3001, OnEliminar)
END_MESSAGE_MAP()


// CClientesView diagnostics

#ifdef _DEBUG
void CClientesView::AssertValid() const
{
	CConectorFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CClientesView::Dump(CDumpContext& dc) const
{
	CConectorFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CClientesView message handlers

void CClientesView::OnInitialUpdate()
{
	AddAnchor(IDC_LENTIDADES,TOP_LEFT,CSize(100,100));

	AddAnchor(IDC_ENOMBRE, BOTTOM_LEFT);
	AddAnchor(IDC_SNOMBRE, BOTTOM_LEFT);
	AddAnchor(IDC_ESERVICIOS, BOTTOM_LEFT);
	AddAnchor(IDC_SSERVICIOS, BOTTOM_LEFT);
	AddAnchor(IDC_ENIT, BOTTOM_LEFT);
	AddAnchor(IDC_ETELEFONO, BOTTOM_LEFT);
	AddAnchor(IDC_EDIRECCION, BOTTOM_LEFT);
	AddAnchor(IDC_SNIT, BOTTOM_LEFT);
	AddAnchor(IDC_STELEFONO, BOTTOM_LEFT);
	AddAnchor(IDC_SDIRECCION, BOTTOM_LEFT);
	AddAnchor(IDC_SEMAIL, BOTTOM_LEFT);
	AddAnchor(IDC_EEMAIL, BOTTOM_LEFT);
	AddAnchor(IDC_SCONTACTO, BOTTOM_LEFT);
	AddAnchor(IDC_ECONTACTO, BOTTOM_LEFT);

	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	CConectorFormView::OnInitialUpdate();

	GetDocument()->SetTitle("Clientes");

	m_LCEntidades.AddColumn("Documento");
	m_LCEntidades.AddColumn("Nombre");
	m_LCEntidades.AddColumn("Dirección");
	m_LCEntidades.AddColumn("Teléfono");
	m_LCEntidades.AddColumn("E-Mail");
	m_LCEntidades.AddColumn("Contacto");
	m_LCEntidades.AddColumn("Descripción");
	m_LCEntidades.SetOrdenable(true);

	CQuery query("cliente");
	query+="id";
	query+="nombre";
	query+="descripcion";
	query+="identificacion";
	query+="telefono";
	query+="direccion";
	query+="email";
	query+="contacto";
	m_pConector->Query(query.GetSelect("estado IS NULL OR estado!=-1"), query_clientes, this);
	m_BAceptar.EnableWindow(m_pUsuario3->CanInsert(this));
}

LRESULT CClientesView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage mess(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(mess.GetID()==query_clientes) {
				int pos= m_LCEntidades.AddItem(mess["identificacion"]);
				m_LCEntidades.SetItemText(pos, 1, mess["nombre"]);
				m_LCEntidades.SetItemText(pos, 2, mess["direccion"]);
				m_LCEntidades.SetItemText(pos, 3, mess["telefono"]);
				m_LCEntidades.SetItemText(pos, 4, mess["email"]);
				m_LCEntidades.SetItemText(pos, 5, mess["contacto"]);
				m_LCEntidades.SetItemText(pos, 6, mess["descripcion"]);
				m_LCEntidades.SetItemData(pos, atoi(mess["id"]));
			} else if(mess.GetID()==query_insertar) {
				for(int i=0; i<m_LCEntidades.GetItemCount(); i++) {
					if(m_LCEntidades.GetItemData(i)==0) {
						m_LCEntidades.SetItemData(i,atoi(mess[0]));
						break;
					}
				}
			}
			break;
		case THREAD_END:
			if(mess.GetID()==query_clientes) {
				m_LCEntidades.Mensaje="No se encontraron clientes";
				m_LCEntidades.AutoSizeColumns();
			}
			break;
	}
	return 0;
}

void CClientesView::OnEditar() {

	if(m_LCEntidades.SelectedIndex==-1)
		return;
	int index=m_LCEntidades.SelectedIndex;
	m_dwCurrentID= m_LCEntidades.GetItemData(index);
	m_ENit.Text=m_LCEntidades.GetItemText(index,0);
	m_ENombre.Text= m_LCEntidades.GetItemText(index, 1);
	m_EDireccion.Text= m_LCEntidades.GetItemText(index, 2);
	m_ETelefono.Text= m_LCEntidades.GetItemText(index, 3);
	m_EEMail.Text=m_LCEntidades.GetItemText(index,4);
	m_EContacto.Text=m_LCEntidades.GetItemText(index,5);
	m_EDescripcion.Text=m_LCEntidades.GetItemText(index,6);
	m_BAceptar.SetWindowText("Editar");
	m_BAceptar.EnableWindow(m_pUsuario3->CanUpdate(this));
	m_ENombre.SetFocus();
}

void CClientesView::OnEliminar() {

	if(!m_pUsuario3->CanDelete(this)) 
		return;
	if(m_LCEntidades.SelectedIndex==-1)
		return;
	CString temp;
	if(m_LCEntidades.GetSelectedCount()==1) 
		temp= "¿Esta seguro de eliminar '"  +  m_LCEntidades.GetItemText(m_LCEntidades.SelectedIndex, 1) + "'?"; 
	else temp.Format("¿Esta seguro de eliminar los %d elementos seleccionados?",m_LCEntidades.GetSelectedCount());
	if(MessageBox(temp, NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {
		CWaitCursor wait;
		while(m_LCEntidades.SelectedIndex!=-1) {
			temp.Format("UPDATE cliente SET estado=-1 WHERE id='%d'", m_LCEntidades.GetItemData(m_LCEntidades.SelectedIndex));
			m_pConector->Query(temp);
			m_LCEntidades.DeleteItem(m_LCEntidades.SelectedIndex);
		}
	}
}

void CClientesView::OnNMDblclkLentidades(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	OnEditar();
	*pResult = 0;
}

void CClientesView::OnNMRClickLentidades(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if(pNMItemActivate->iItem==-1)
		return;
	CNewMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, 3000, "Editar");
	menu.AppendMenu(MF_STRING, 3001, "Eliminar");
	menu.SetDefaultItem(3000, FALSE);

	POINT point;
	GetCursorPos( &point );
	menu.TrackPopupMenu( TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CClientesView::OnBnClickedOk()
{
	CString temp;
	CQuery query("cliente");

	if(m_ENombre.Text.IsEmpty()) {
		SetErrorEx(&m_ENombre, "El nombre no puede estar en blanco", 2, "miscelaneos");
		return;
	}

	query["nombre"]= m_ENombre.Text;
	query["descripcion"]=m_EDescripcion.Text;
	query["identificacion"]=m_ENit.Text;
	query["telefono"]=m_ETelefono.Text;
	query["direccion"]=m_EDireccion.Text;
	query["email"]= m_EEMail.Text;
	query["contacto"]= m_EContacto.Text;

	int pos;
	if(m_dwCurrentID) {
		for(int i=0; i<m_LCEntidades.GetItemCount(); i++) {
			if(m_LCEntidades.GetItemData(i)==m_dwCurrentID) {
				pos=i;
				break;
			}
		}
		temp.Format("id='%d'", m_dwCurrentID);
		m_pConector->Query(query.GetUpdate(temp));
	} else {
		pos= m_LCEntidades.AddItem("");
		m_pConector->Query(query.GetInsert(), query_insertar, this);
	}
	m_LCEntidades.SetItemText(pos, 0, m_ENit.Text);
	m_LCEntidades.SetItemText(pos, 1, m_ENombre.Text);
	m_LCEntidades.SetItemText(pos, 2, m_EDireccion.Text);
	m_LCEntidades.SetItemText(pos, 3, m_ETelefono.Text);
	m_LCEntidades.SetItemText(pos, 4, m_EEMail.Text);
	m_LCEntidades.SetItemText(pos, 5, m_EContacto.Text);
	m_LCEntidades.SetItemText(pos, 6, m_EDescripcion.Text);
	OnBnClickedCancel();
}

void CClientesView::OnBnClickedCancel()
{
	m_BAceptar.EnableWindow(m_pUsuario3->CanInsert(this));
	m_dwCurrentID=0;
	m_EContacto.Text="";
	m_EEMail.Text="";
	m_ENombre.Text="";
	m_EDescripcion.Text="";
	m_ENit.Text="";
	m_ETelefono.Text="";
	m_EDireccion.Text="";
	m_BAceptar.SetWindowText("Nuevo");
	m_ENombre.SetFocus();
}
