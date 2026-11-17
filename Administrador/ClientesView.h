#pragma once
#include "afxwin.h"



// CClientesView form view

class CClientesView : public CConectorFormView
{
	DECLARE_DYNCREATE(CClientesView)
	DWORD_PTR m_dwCurrentID;

	enum {
		query_clientes=1,
		query_insertar
	};
protected:
	CClientesView();           // protected constructor used by dynamic creation
	virtual ~CClientesView();

public:
	enum { IDD = IDD_CLIENTESVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	CMyListCtrl m_LCEntidades;
	CEditXP m_ENombre;
	CButtonXP m_BAceptar;
	CButtonXP m_BCancelar;
	afx_msg void OnNMDblclkLentidades(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickLentidades(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
	void OnEditar();
	void OnEliminar();
public:
	CEditXP m_EDescripcion;
	CEditXP m_ENit;
	CEditXP m_ETelefono;
	CEditXP m_EDireccion;
	CEditXP m_EEMail;
	CEditXP m_EContacto;
};


