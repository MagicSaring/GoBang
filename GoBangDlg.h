
// GoBangDlg.h : header file
//

#pragma once
#include "Table.h"

// CGoBangDlg dialog
class CGoBangDlg : public CDialogEx
{
// Construction
public:
	CDialog *m_pDlg;
	CTable m_Table;
	CGoBangDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOBANG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMenuAbout();
	afx_msg void OnClickedBtnBack();
	afx_msg void OnMenuPcfirst();
	afx_msg void OnMenuPlayerfirst();
};
