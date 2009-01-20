// MiscelaneosView.cpp : implementation file
//

#include "stdafx.h"
# include "resource.h"
#include "Administrador.h"
#include "MiscelaneosView.h"


// CMiscelaneosView

IMPLEMENT_DYNCREATE(CMiscelaneosView, CConectorFormView)

CMiscelaneosView::CMiscelaneosView()
	: CConectorFormView(CMiscelaneosView::IDD)
{
	m_nReservado=0;
	m_dwCurrentID=0;
}

CMiscelaneosView::~CMiscelaneosView()
{
}

void CMiscelaneosView::DoDataExchange(CDataExchange* pDX)
{
	CConectorFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LTIPO, m_LCTipo);
	DDX_Control(pDX, IDC_LELEMENTOS, m_LCElementos);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_EDESCRIPCION, m_EDescripcion);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_SNOMBRE, m_SNombre);
	DDX_Control(pDX, IDC_SDESCRIPCION, m_SDescripcion);
}

BEGIN_MESSAGE_MAP(CMiscelaneosView, CConectorFormView)
	ON_BN_CLICKED(IDC_BACEPTAR, &CMiscelaneosView::OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BCANCELAR, &CMiscelaneosView::OnBnClickedBcancelar)
	ON_NOTIFY(NM_DBLCLK, IDC_LELEMENTOS, &CMiscelaneosView::OnNMDblclkLelementos)
	ON_NOTIFY(NM_RCLICK, IDC_LELEMENTOS, &CMiscelaneosView::OnNMRClickLelementos)
	ON_NOTIFY(NM_DBLCLK, IDC_LTIPO, &CMiscelaneosView::OnNMDblclkLtipo)
	ON_NOTIFY(NM_RCLICK, IDC_LTIPO, &CMiscelaneosView::OnNMRClickLtipo)
	ON_COMMAND(2000, OnClickTipo)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_COMMAND(3000, OnEditar)
	ON_COMMAND(3001, OnEliminar)
END_MESSAGE_MAP()


// CMiscelaneosView diagnostics

#ifdef _DEBUG
void CMiscelaneosView::AssertValid() const
{
	CConectorFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiscelaneosView::Dump(CDumpContext& dc) const
{
	CConectorFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiscelaneosView message handlers

void CMiscelaneosView::OnInitialUpdate()
{
	AddAnchor(IDC_LTIPO,TOP_LEFT,CSize(-1, 100));
	AddAnchor(IDC_LELEMENTOS,TOP_LEFT, CSize(100,100));
	AddAnchor(IDC_ENOMBRE, BOTTOM_LEFT);
	AddAnchor(IDC_EDESCRIPCION, BOTTOM_LEFT);
	AddAnchor(IDC_SNOMBRE, BOTTOM_LEFT);
	AddAnchor(IDC_SDESCRIPCION, BOTTOM_LEFT);
	AddAnchor(IDC_BCANCELAR, BOTTOM_RIGHT);
	AddAnchor(IDC_BACEPTAR, BOTTOM_RIGHT);

	CConectorFormView::OnInitialUpdate();

	GetDocument()->SetTitle("Misceláneos");

	m_LCTipo.AddColumn("Tipo");
	m_LCTipo.AutoSizeColumns();

	/*m_LCTipo.SetItemData(m_LCTipo.AddItem("Tipo de contrato"), 3);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Sectores"), 4);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Evaluación bienes tangibles"), 5);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Evaluación Servicios"), 6);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Cuentas bancarias"), 7);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Estado de requisición"), 8);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Tipo comité"), 9);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Fuente de no conformidades"), 10);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Estado de no conformidades"), 11);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Aspectos de evaluación"), 12);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Tipo de acción"), 13);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Fuente de la acción"), 14);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Tipo de novedad"), 15);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Estados civiles"), 16);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Habilidades"), 17);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Tipo propuesta"), 18);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Clasificación UG"), 19);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Tipo producto"), 20);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Clase de verificación"), 21);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Presupuesto oficial"), 22);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Costos previos"), 23);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Requisitos propuesta"), 24);
	m_LCTipo.SetItemData(m_LCTipo.AddItem("Preguntas auditoria"), 25);*/

	m_LCElementos.AddColumn("Nombre");
	m_LCElementos.AddColumn("Descripción");
	m_LCElementos.AutoSizeColumns();
	m_LCElementos.SetOrdenable(true);

	m_LCTipo.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	OnClickTipo();
	m_BAceptar.EnableWindow(m_pUsuario3->CanInsert(this));
}

void CMiscelaneosView::OnBnClickedBaceptar()
{
	CQuery query("shared");
	if(m_ENombre.Text.IsEmpty()) {
		SetErrorEx(&m_ENombre, "El nombre no puede estar en blanco", 2, "miscelaneos");
		return;
	}
	query["nombre"]= m_ENombre.Text;
	query["descripcion"]= m_EDescripcion.Text;
	CString temp;
	temp.Format("%d", m_nReservado);
	query["nReservado"]=temp;
	int pos;
	if(m_dwCurrentID) {
		for(int i=0; i<m_LCElementos.GetItemCount(); i++) {
			if(m_LCElementos.GetItemData(i)==m_dwCurrentID) {
				pos=i;
				break;
			}
		}
		temp.Format("id='%d'", m_dwCurrentID);
		m_pConector->Query(query.GetUpdate(temp));
	} else {
		pos= m_LCElementos.AddItem("");
		m_pConector->Query(query.GetInsert(), query_insertar, this);
	}
	m_LCElementos.SetItemText(pos, 0, m_ENombre.Text);
	m_LCElementos.SetItemText(pos, 1, m_EDescripcion.Text);
	OnBnClickedBcancelar();
}

void CMiscelaneosView::OnBnClickedBcancelar()
{
	m_BAceptar.EnableWindow(m_pUsuario3->CanInsert(this));
	m_dwCurrentID=0;
	m_ENombre.Text="";
	m_EDescripcion.Text="";
	m_BAceptar.SetWindowText("Nuevo");
	m_ENombre.SetFocus();
}

void CMiscelaneosView::OnEliminar() {

	if(!m_pUsuario3->CanDelete(this)) 
		return;
	if(m_LCElementos.SelectedIndex==-1)
		return;
	CString temp;
	if(m_LCElementos.GetSelectedCount()==1) 
		temp= "¿Esta seguro de eliminar '"  +  m_LCElementos.GetItemText(m_LCElementos.SelectedIndex, 0) + "'?"; 
	else temp.Format("¿Esta seguro de eliminar los %d elementos seleccionados?",m_LCElementos.GetSelectedCount());
	if(MessageBox(temp, NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {
		CWaitCursor wait;
		while(m_LCElementos.SelectedIndex!=-1) {
			temp.Format("UPDATE shared SET szReservado=-1 WHERE id='%d'", m_LCElementos.GetItemData(m_LCElementos.SelectedIndex));
			m_pConector->Query(temp);
			m_LCElementos.DeleteItem(m_LCElementos.SelectedIndex);
		}
	}
}

void CMiscelaneosView::OnEditar() {

	if(m_LCElementos.SelectedIndex==-1)
		return;
	int index=m_LCElementos.SelectedIndex;
	m_dwCurrentID= m_LCElementos.GetItemData(index);
	m_ENombre.Text= m_LCElementos.GetItemText(index, 0);
	m_EDescripcion.Text= m_LCElementos.GetItemText(index, 1);
	m_BAceptar.SetWindowText("Editar");
	m_BAceptar.EnableWindow(m_pUsuario3->CanUpdate(this));
	m_ENombre.SetFocus();
}

void CMiscelaneosView::OnNMDblclkLelementos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	OnEditar();
	*pResult = 0;
}

void CMiscelaneosView::OnNMRClickLelementos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE) pNMHDR;
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


LRESULT CMiscelaneosView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage query(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(query.GetID()==query_elementos) {
				int pos= m_LCElementos.AddItem(query["nombre"]);
				m_LCElementos.SetItemText(pos, 1, query["descripcion"]);
				m_LCElementos.SetItemData(pos, atoi(query["id"]));
			} else if(query.GetID()==query_insertar) {
				for(int i=0; i<m_LCElementos.GetItemCount(); i++) {
					if(m_LCElementos.GetItemData(i)==0) {
						m_LCElementos.SetItemData(i, atoi(query[0]));
						break;
					}
				}
			}
			break;
		case THREAD_END:
			if(query.GetID()==query_elementos) {
				for(int i=0; i<m_LCTipo.GetItemCount(); i++) {
					if(m_LCTipo.GetItemData(i)==m_nReservado) {
						m_LCElementos.Mensaje="No se encontraron '" + m_LCTipo.GetItemText(i,0) + "'";
						m_LCElementos.AutoSizeColumns();
						break;
					}
				}
			}
			break;
	}
	return 0;
}

void CMiscelaneosView::OnClickTipo() {

	if(m_LCTipo.SelectedIndex==-1)
		return;
	DWORD_PTR tipo=m_LCTipo.GetItemData(m_LCTipo.SelectedIndex);
	if(tipo==m_nReservado)
		return;
	m_nReservado= tipo;
	OnBnClickedBcancelar();
	m_LCElementos.DeleteAllItems();
	while(m_LCElementos.GetColumnCount())
		m_LCElementos.DeleteColumn(0);

	GetDocument()->SetTitle("Misceláneos - " + m_LCTipo.GetItemText(m_LCTipo.SelectedIndex,0));
	GetDocument()->SetTitle("Misceláneos - " + m_LCTipo.GetItemText(m_LCTipo.SelectedIndex,0));
	m_LCElementos.Mensaje="Cargando '" + m_LCTipo.GetItemText(m_LCTipo.SelectedIndex,0) + "'";

	switch(m_nReservado) {
		case 7:
			m_LCElementos.AddColumn("Banco");
			m_LCElementos.AddColumn("Cuenta");
			m_SNombre.SetWindowText("Banco");
			m_SDescripcion.SetWindowText("Cuenta");
			break;
		default:
			m_LCElementos.AddColumn("Nombre");
			m_LCElementos.AddColumn("Descripción");
			m_SNombre.SetWindowText("Nombre");
			m_SDescripcion.SetWindowText("Descripción");
			break;
	}

	CQuery query("shared");
	query+="id";
	query+="nombre";
	query+="descripcion";

	CString temp;
	temp.Format("nReservado='%d' AND (szReservado IS NULL OR szReservado!=-1)", m_nReservado);
	m_pConector->Query(query.GetSelect(temp), query_elementos, this);
}

void CMiscelaneosView::OnNMDblclkLtipo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE )pNMHDR;
	OnClickTipo();
	*pResult = 0;
}

void CMiscelaneosView::OnNMRClickLtipo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE )pNMHDR;
	if(pNMItemActivate->iItem==-1)
		return;
	CNewMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, 2000, "Abrir");
	menu.SetDefaultItem(2000, FALSE);

	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu( TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
	
	*pResult = 0;
}
