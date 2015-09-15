// Filename: MainDlg.h
// 18-Nov-2013 SSC copy right.
#pragma once

#include <map>
#include "WndEvent.h"
#include <list>


// Forward class declarations.
class CWndDlg;
class CGroupControl;
class CWndControl;
class CWndCheckBox;
class CWndTextBox;
class CWndComboBox;
class CTabControl;
class CGameEngine;
class CEventHandler;
class CCharacter;
class CGUIMapping;


const UINT WM_ITEM_SELECTED_CHANGE = WM_USER + 0x103;
const UINT WM_BTN_CLICK = WM_USER + 0x106;
const UINT WM_ITEM_CHECKED_CHANGE = WM_USER + 0x107;


// MainDlg

class __declspec(dllexport) CMainDlg : public CWnd, public IWndEventHandler
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg();
	virtual ~CMainDlg();
	void BuildFromXml(const CString& xmlFile);
	void BuildListControl(CListCtrl& ctrl);
	//Gán thông tin đối tượng lên giao diện
	void MappingObj();
    // IWndEventHandler overrides.
    virtual void HandleWndEvent(const CWndEvent& ev);
	LRESULT OnBtnExitClick(WPARAM wParam, LPARAM lParam);

protected:
	virtual void PostNcDestroy();
	
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnItemStateChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnItemCheckedChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnTabSelectionChange(WPARAM wParam, LPARAM lParam);
	//GameEngine Handle
	LRESULT OnRoleChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnRoleOut(WPARAM wParam, LPARAM lParam);
	//LRESULT OnMapChanged(WPARAM wParam, LPARAM lParam);

	LRESULT OnCallBack(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	//Get role info
	CGameEngine* m_engine;
	CCharacter* m_clsCurChar;
	
	// Control dialog.
	CWndDlg* m_controlDlg;
	// Tab window.
	CTabControl* m_tabWnd;	
	
private:
	bool CreateTabWnd();
	bool CreateControlDlg();
	void UpdateControlDlg();

    void SaveFocus();
    void RestoreFocus();

    void DeleteGroups();
    void DeleteControls();
	//Add data to list control
	int AddData( int row,int col, const CString str);
	
	// Current window with focus.
    HWND m_hWndFocus;

    // Top-level control groups.
    std::list<CGroupControl*> m_topLevelGroupsList;

    // All controls.
    std::list<CWndControl*> m_allControlsList;
	
	// List control.
	CListCtrl* m_listCtrl;

	//Event
	CEventHandler* events;

	//Mapping
	CGUIMapping* m_mapping;
	std::map<CString,CGUIMapping*> m_mLstMap;

	
};