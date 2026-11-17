#pragma once
#include "afxwin.h"
# include "resource.h"
// CDlgEditar dialog

class CDlgEditar : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditar)
	CUsuario3 *m_pUsuario3;
public:
	CDlgEditar(CUsuario3 *usuario, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditar();

// Dialog Data
	enum { IDD = IDD_DIALOGUSUARIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_szContrasena;
	CString	m_szNombreCompleto;
	CUsuario *m_pActual;
	CStringArray	*m_saNombres;
	CStringArray	*m_saDescripciones;
	CUIntArray		*m_uaIDS;
	DWORD			m_dwGrupo;
	CString			m_szNombre;

	CEditXP		m_ENombre;
	CButtonXP	m_BAceptar;
	CButtonXP	m_BCancelar;
	CButtonXP	m_BContrasena;
	afx_msg void OnEnChangeEditnombre();
	afx_msg void OnBnClickedButtoncontrasena();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CComboBoxXP m_CBGrupo;
	afx_msg void OnCbnSelchangeCgrupo();
	CEditXP m_ENombreCompleto;
};
