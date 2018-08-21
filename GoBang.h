
// GoBang.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGoBangApp:
// See GoBang.cpp for the implementation of this class
//

class CGoBangApp : public CWinApp
{
public:
	CGoBangApp();

	TCHAR m_szIni[MAX_PATH];
	int m_nWin;
	int m_nDraw;
	int m_nLost;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGoBangApp theApp;