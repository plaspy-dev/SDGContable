// TemplateView.h: interfaz de la clase CTemplateView
//


#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CTemplateView : public CConectorFormView
{
protected: // Crear sólo a partir de serialización
	CTemplateView();
	DECLARE_DYNCREATE(CTemplateView)

public:
	enum{ IDD = IDD_TEMPLATE_FORM };

// Operaciones
public:

// Reemplazos
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	virtual void OnInitialUpdate(); // Se llama la primera vez después de la construcción

// Implementación
public:
	virtual ~CTemplateView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignación de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	CEditXP m_ENumero;
	CComboBoxXP m_CBDocumento;
	CComboBoxXP m_CBNombre;
	CEditXP m_EDireccion;
	CEditXP m_ETelefono;
	CListCtrlXP m_LCItems;
	CEditXP m_EObservaciones;
	CEditXP m_ESubtotal;
	CEditXP m_EDescuento;
	CEditXP m_EIVA;
	CEditXP m_ERetencion;
	CEditXP m_ETotal;
	CButtonXP m_BCancelar;
	CButtonXP m_BAceptar;

	LRESULT OnMySQLMessage(LPARAM lParam, WPARAM wParam);
	afx_msg void OnEnKillfocusEnumero();
	afx_msg void OnCbnSelchangeCdocumento();
	afx_msg void OnCbnSelchangeCnombre();
};


