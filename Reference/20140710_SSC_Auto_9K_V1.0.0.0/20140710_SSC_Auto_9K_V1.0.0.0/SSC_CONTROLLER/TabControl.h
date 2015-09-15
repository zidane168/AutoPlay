// Filename: TabControl.h
// 18-Nov-2013 SSC copy right.

#pragma once

#include <map>

class CGroupControl;
const UINT WM_TAB_SELECTION_CHANGE = WM_USER + 0x101;
// CTabControl

class CTabControl : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabControl)

public:
	__declspec(dllexport) CTabControl();
	__declspec(dllexport) virtual		~CTabControl();
	__declspec(dllexport) void		AddTabs( CGroupControl* controlGroup);
	__declspec(dllexport) void		SetNotifyWnd(CWnd* notifyWnd);
	__declspec(dllexport) int			GetRowCount();
	__declspec(dllexport) int			GetCurSel();
	__declspec(dllexport) CGroupControl* GetSelectedGroup();
	 int			m_tabCount;

protected:
	virtual void PostNcDestroy();
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()
	void         OnTabSelChanged(NMHDR* pNMHDR, LRESULT* pResult);

private:
    bool CreateTabCtrl(LPCREATESTRUCT lpCreateStruct);
    void GetTabRect(CRect& rect);
	CGroupControl* CTabControl::MapItemToGroup(int tabIndex);
	CTabCtrl*	m_tabCtrl;   
    int			m_tabMargin;
	CFont		font;
	CWnd*      m_notifyWnd;
	std::map<int,CGroupControl*> m_itemToGroupMap;
};


