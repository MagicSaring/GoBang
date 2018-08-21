
// GoBangDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GoBang.h"
#include "GoBangDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGoBangDlg dialog



CGoBangDlg::CGoBangDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GOBANG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGoBangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGoBangDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_ABOUT, &CGoBangDlg::OnMenuAbout)
	ON_BN_CLICKED(IDC_BTN_BACK, &CGoBangDlg::OnClickedBtnBack)
	ON_COMMAND(ID_MENU_PCFirst, &CGoBangDlg::OnMenuPcfirst)
	ON_COMMAND(ID_MENU_PlayerFirst, &CGoBangDlg::OnMenuPlayerfirst)
END_MESSAGE_MAP()


// CGoBangDlg message handlers

BOOL CGoBangDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_pDlg = NULL;
	CRect rect(0, 0, 200, 200);
	m_Table.CreateEx(WS_EX_CLIENTEDGE, _T("ChessTable"), NULL, WS_VISIBLE | WS_BORDER | WS_CHILD,
		CRect(0, 0, 480, 509), this, IDC_TABLE);

	// 设置双方姓名
	SetDlgItemText(IDC_ST_ME, m_Table.m_strMe);
	SetDlgItemText(IDC_ST_ENEMY, _T("计算机"));

	// 禁用“再玩”和“离开”
	CMenu *pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_MENU_PLAYAGAIN, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_MENU_LEAVE, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
	m_Table.Clear(TRUE);
	GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_TABLE)->SetFocus();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGoBangDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGoBangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGoBangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGoBangDlg::OnMenuAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

//悔棋
void CGoBangDlg::OnClickedBtnBack()
{
	// TODO: Add your control notification handler code here
	m_Table.Back();
}

//计算机先走
void CGoBangDlg::OnMenuPcfirst()
{
	// TODO: Add your command handler code here
	GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);
	m_Table.SetColor(1);
	m_Table.Clear(FALSE);
	m_Table.SetGameMode(1);
}

//玩家先走
void CGoBangDlg::OnMenuPlayerfirst()
{
	// TODO: Add your command handler code here
	GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);
	m_Table.Accept(1);
}
