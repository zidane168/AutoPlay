#pragma once


// CWndSkill dialog

class CWndSkill : public CDialog
{
	DECLARE_DYNAMIC(CWndSkill)

public:
	CWndSkill(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWndSkill();

// Dialog Data
	enum { IDD = IDD_FRM_SKILL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
