#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CGruposView form view

class CGruposView : public CConectorFormView
{
	DECLARE_DYNCREATE(CGruposView)

protected:
	DWORD_PTR		m_dwCurrentGrupo;
	CGruposView();           // protected constructor used by dynamic creation
	virtual ~CGruposView();
	LRESULT OnMySQLMessage(WPARAM wParam,LPARAM lParam);
public:
	enum { IDD = IDD_GRUPOSVIEW };
	enum {
		query_grupos=1,
		query_permisos,
	};
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
	CListCtrlXP m_LCGrupo;
	CEditXP m_ENombre;
	CEditXP m_EDescripcion;
	CListCtrlXP m_LCPermisos;
	CButtonXP m_BCancelar;
	CButtonXP m_BAceptar;
	virtual void OnInitialUpdate();
	afx_msg void OnNMDblclkLgrupo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickLgrupo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMClickLpermisos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeydownLpermisos(NMHDR *pNMHDR, LRESULT *pResult);
	void OnEditar();
	void OnEliminar();
	afx_msg void OnLvnColumnclickLpermisos(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnListaNotifiacion(WPARAM wParam, LPARAM lParam);
};
