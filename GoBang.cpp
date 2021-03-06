
// GoBang.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GoBang.h"
#include "GoBangDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGoBangApp

BEGIN_MESSAGE_MAP(CGoBangApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGoBangApp construction

CGoBangApp::CGoBangApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CGoBangApp object

CGoBangApp theApp;


// CGoBangApp initialization

BOOL CGoBangApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	//获取配置文件位置
	::GetModuleFileName(NULL, m_szIni, MAX_PATH);
	lstrcpy(&m_szIni[lstrlen(m_szIni) - 3], _T("ini"));

	//读取战绩统计
	m_nWin = ::GetPrivateProfileInt(_T("Stats"), _T("Win"), 0, m_szIni);
	m_nDraw = ::GetPrivateProfileInt(_T("Stats"), _T("Draw"), 0, m_szIni);
	m_nLost = ::GetPrivateProfileInt(_T("Stats"), _T("Lost"), 0, m_szIni);


	//读取战绩统计

	//注册棋牌窗口类
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpfnWndProc = ::DefWindowProc;
	wc.lpszClassName = _T("ChessTable");
	wc.lpszMenuName = NULL;
	wc.style = 0;
	AfxRegisterClass(&wc);



	CGoBangDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

