// Template.h: archivo de encabezado principal para la aplicación Template
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // Símbolos principales
# include <IOFile/IOFile.h>

# define GRUPO_ADMINISTRADOR	1


class CTemplateApp : public CWinAppEx
{
public:
	CTemplateApp();

// Reemplazos
public:
	virtual BOOL InitInstance();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	HWND		RegisterApp(HWND mainframe);

// Implementación
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	afx_msg void OnFileSave();
};

extern CTemplateApp theApp;