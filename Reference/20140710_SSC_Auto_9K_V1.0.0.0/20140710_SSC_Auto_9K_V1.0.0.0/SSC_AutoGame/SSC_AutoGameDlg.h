// SSC_AutoGameDlg.h : header file
//

#pragma once
#include "afxcmn.h"

#define WM_TRAY_MESSAGE (WM_USER + 104)

class CMainDlg;

// CSSC_AutoGameDlg dialog
class CSSC_AutoGameDlg : public CDialog
{
// Construction
public:
	CSSC_AutoGameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SSC_AUTOGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//System tray
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCallBack(WPARAM wParam, LPARAM lParam);
	void OnLinkClick(NMHDR *pNMHDR, LRESULT *pResult) ;
	void HideToTray();
	void OnExit();
	void OnShow();

	DECLARE_MESSAGE_MAP()
public:
	CMainDlg* m_mainDlg;
	CListCtrl m_listCwnd;

	CString m_sName;
	
	afx_msg void OnHdnItemchangedListRole(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();

	//System tray
	CMenu m_menu;
	NOTIFYICONDATA m_TrayData;
};
