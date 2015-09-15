#pragma once

#include <map>
#include "../SSC_AutoGame/Resource.h"
class CMainDlg;
// CWndPriority dialog

class CWndPriority : public CDialog
{
	DECLARE_DYNAMIC(CWndPriority)

public:
	CWndPriority(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWndPriority();
	virtual void OnCancel();

	afx_msg void OnBtnUpClick();
	afx_msg void OnBtnDownClick();

// Dialog Data
	enum { IDD = IDD_PRIORITYCFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CMainDlg* m_wndParent;
	byte* m_arrByte;
	std::map<int,CString> m_mElement;
	std::map<int,CString> m_mFaction;
};
