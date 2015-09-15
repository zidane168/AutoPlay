// Filename: WndDlg.h
// 18-Nov-2013 SSC copy right.

#pragma once

#include <list>

// Forward class declarations.
class CWndContainer;
class CWndControl;


// CWndDlg class

class CWndDlg : public CDialog
{
	public:
    // Constructor.
    __declspec(dllimport) CWndDlg(CWnd* pParent = NULL);
    __declspec(dllimport) virtual ~CWndDlg();

    // Managing dynamic controls.
    __declspec(dllimport) void         AddControl(CWndControl* pControl);
   __declspec(dllimport)  void         AddControls(std::list<CWndControl*>& controlList);
    __declspec(dllimport) void         RemoveControl(CWndControl* pControl);
    __declspec(dllimport) void         RemoveAllControls();
    __declspec(dllimport) CWndControl* GetControl(const CString& controlName);
   __declspec(dllimport)  void         GetControls(std::list<CWndControl*>& controlList) const;

protected:
	virtual void PostNcDestroy();
    
    // ClassWizard generated virtual function overrides.
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
    // Generated message map functions.
    afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	// For dynamic controls.
    CWndContainer* m_wndContainer;
};


