#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CMiscelaneosView form view

class CMiscelaneosView : public CConectorFormView
{
	DECLARE_DYNCREATE(CMiscelaneosView)

protected:
	CMiscelaneosView();           // protected constructor used by dynamic creation
	virtual ~CMiscelaneosView();
	DWORD	m_nReservado;
	DWORD	m_dwCurrentID;

	enum {
		query_elementos=1,
		query_insertar
	};
public:
	enum { IDD = IDD_MISCELANEOSVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CMyListCtrl m_LCTipo;
	CMyListCtrl m_LCElementos;
	CEditXP m_ENombre;
	CEditXP m_EDescripcion;
	CButtonXP m_BCancelar;
	CButtonXP m_BAceptar;
	afx_msg void OnBnClickedBaceptar();
	afx_msg void OnBnClickedBcancelar();
	afx_msg void OnNMDblclkLelementos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickLelementos(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMDblclkLtipo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickLtipo(NMHDR *pNMHDR, LRESULT *pResult);
	void OnClickTipo();
	void OnEliminar();
	void OnEditar();
	CStatic m_SNombre;
	CStatic m_SDescripcion;
};


