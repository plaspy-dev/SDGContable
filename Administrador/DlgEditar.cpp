// DlgEditar.cpp : implementation file
//

#include "stdafx.h"
# include "Administrador.h"
#include "DlgEditar.h"
# include "DlgPasswd.h"


IMPLEMENT_DYNAMIC(CDlgEditar, CDialog)
CDlgEditar::CDlgEditar(CUsuario3 *usuario, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditar::IDD, pParent)
{
	m_pUsuario3= usuario;
	m_pActual= NULL;
	m_dwGrupo=0;
}

CDlgEditar::~CDlgEditar()
{
}

void CDlgEditar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITNOMBRE, m_ENombre);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_BUTTONCONTRASENA, m_BContrasena);
	DDX_Control(pDX, IDC_CGRUPO, m_CBGrupo);
	DDX_Control(pDX, IDC_ENOMBRECOMPLETO, m_ENombreCompleto);
}


BEGIN_MESSAGE_MAP(CDlgEditar, CDialog)
	ON_EN_CHANGE(IDC_EDITNOMBRE, OnEnChangeEditnombre)
	ON_BN_CLICKED(IDC_BUTTONCONTRASENA, OnBnClickedButtoncontrasena)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CGRUPO, &CDlgEditar::OnCbnSelchangeCgrupo)
END_MESSAGE_MAP()


// CDlgEditar message handlers

void CDlgEditar::OnEnChangeEditnombre()
{
	CString temp;
	SetWindowText(m_ENombre.Text);
	m_BAceptar.EnableWindow(((m_dwGrupo && m_pUsuario3->CanUpdate("CFormUser")) || (!m_dwGrupo && m_pUsuario3->CanInsert("CFormUser")) ) && (m_ENombre.GetWindowTextLength() && m_CBGrupo.SelectedIndex!=-1));
}

void CDlgEditar::OnBnClickedButtoncontrasena()
{
	CDlgPasswd dlg;
	if( dlg.DoModal()==IDOK) 
		m_szContrasena= dlg.m_szContrasena;
}

BOOL CDlgEditar::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( m_pActual) {
		m_ENombre.SetWindowText(m_pActual->GetNombre());
		SetWindowText(m_pActual->GetNombre());
	} else {
		SetWindowText("Nuevo Usuario");
	}

	for(int i=0; i<m_saNombres->GetCount(); i++) 
		m_CBGrupo.SetItemData(m_CBGrupo.AddString(m_saNombres->GetAt(i)), m_uaIDS->GetAt(i));
	m_CBGrupo.SelectedData= m_dwGrupo;
	m_ENombreCompleto.Text=m_szNombreCompleto;
	m_ENombre.SetLimitText(255);
	OnEnChangeEditnombre();
	return TRUE;
}

void CDlgEditar::OnBnClickedOk()
{
	CString temp;
	m_ENombre.GetWindowText(m_szNombre);
	m_dwGrupo= m_CBGrupo.SelectedData;
	m_szNombreCompleto= m_ENombreCompleto.Text;

	if( m_szContrasena.IsEmpty() || (m_pActual && m_pActual->GetNombre()!=m_szNombre)) {
		m_szContrasena.Empty();
		OnBnClickedButtoncontrasena();
		if( m_szContrasena.IsEmpty())
			return;
	}
	OnOK();
}

void CDlgEditar::OnCbnSelchangeCgrupo()
{
	OnEnChangeEditnombre();
}
