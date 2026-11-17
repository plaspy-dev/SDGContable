// TemplateView.cpp: implementación de la clase CTemplateView
//

#include "stdafx.h"
#include "Template.h"

#include "TemplateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTemplateView

IMPLEMENT_DYNCREATE(CTemplateView, CConectorFormView)

BEGIN_MESSAGE_MAP(CTemplateView, CConectorFormView)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_ENUMERO, &CTemplateView::OnEnKillfocusEnumero)
	ON_CBN_SELCHANGE(IDC_CDOCUMENTO, &CTemplateView::OnCbnSelchangeCdocumento)
	ON_CBN_SELCHANGE(IDC_CNOMBRE, &CTemplateView::OnCbnSelchangeCnombre)
END_MESSAGE_MAP()

// Construcción o destrucción de CTemplateView

CTemplateView::CTemplateView()
	: CConectorFormView(CTemplateView::IDD)
{
	
}

CTemplateView::~CTemplateView()
{
}

void CTemplateView::DoDataExchange(CDataExchange* pDX)
{
	CConectorFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENUMERO, m_ENumero);
	DDX_Control(pDX, IDC_CDOCUMENTO, m_CBDocumento);
	DDX_Control(pDX, IDC_CNOMBRE, m_CBNombre);
	DDX_Control(pDX, IDC_EDIRECCION, m_EDireccion);
	DDX_Control(pDX, IDC_ETELEFONO, m_ETelefono);
	DDX_Control(pDX, IDC_LITEMS, m_LCItems);
	DDX_Control(pDX, IDC_EOBSERVACIONES, m_EObservaciones);
	DDX_Control(pDX, IDC_ESUBTOTAL, m_ESubtotal);
	DDX_Control(pDX, IDC_EDESCUENTOI, m_EDescuento);
	DDX_Control(pDX, IDC_EIVA, m_EIVA);
	DDX_Control(pDX, IDC_ERETENCION, m_ERetencion);
	DDX_Control(pDX, IDC_ETOTAL, m_ETotal);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDOK, m_BAceptar);
}

BOOL CTemplateView::PreCreateWindow(CREATESTRUCT& cs)
{
	
	return CConectorFormView::PreCreateWindow(cs);
}

void CTemplateView::OnInitialUpdate()
{
	CConectorFormView::OnInitialUpdate();
	GetDocument()->SetTitle("Facturas");

	m_LCItems.SetEdit(m_LCItems.AddColumn("Cantidad", CListCtrlXP::tipocol_numero), CListCtrlXP::tipocol_numero);
	m_LCItems.SetEdit(m_LCItems.AddColumn("Concepto"));
	m_LCItems.SetEdit(m_LCItems.AddColumn("Valor Unitario", CListCtrlXP::tipocol_double), CListCtrlXP::tipocol_double);
	m_LCItems.SetEdit(m_LCItems.AddColumn("Valor Total", CListCtrlXP::tipocol_double), CListCtrlXP::tipocol_double);

	m_LCItems.AddItem("");

	m_EObservaciones.InitMultiline();
	m_EObservaciones.Mensaje="Observaciones";
}


// Diagnósticos de CTemplateView

#ifdef _DEBUG
void CTemplateView::AssertValid() const
{
	CConectorFormView::AssertValid();
}

void CTemplateView::Dump(CDumpContext& dc) const
{
	CConectorFormView::Dump(dc);
}
#endif //_DEBUG


// Controladores de mensaje de CTemplateView

void CTemplateView::OnDestroy()
{
	CConectorFormView::OnDestroy();
}

LRESULT CTemplateView::OnMySQLMessage(LPARAM lParam, WPARAM wParam) {

	return 0;
}

void CTemplateView::OnEnKillfocusEnumero()
{
	
}

void CTemplateView::OnCbnSelchangeCdocumento()
{
	
}

void CTemplateView::OnCbnSelchangeCnombre()
{
	
}
