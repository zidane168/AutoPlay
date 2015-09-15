/*
20140611 - Thay đổi code các danh sách chứa role, handle và hwnd

*/


#pragma once

#include <map>
#include <vector>



const UINT UM_ROLE_INFO_CHANGE	 =	WM_USER + 0x102;
const UINT UM_ROLE_REMOVE		 =	WM_USER + 0x105;
const UINT UM_MAP_CHANGED		 =	WM_USER + 0x104;

class CCharacter;
// CGameEngine

class  __declspec(dllexport) CGameEngine: public CWnd
{
	DECLARE_DYNAMIC(CGameEngine)

private:
	//structure for passing to the controlling function
	typedef struct THREADPARAM
	{
		CGameEngine*    _this;
		CCharacter* _char;
	} THREADPARAM;

public:
	typedef std::map<CString,CCharacter*> ListRole;
	typedef std::map<int,CString> ListHwnd;

	CGameEngine();
	virtual ~CGameEngine();
	void GetRoleList();
	void RemoveRoleList();
	bool InjectRole(CString sName);
	bool UnhookRole(CString sName);
	bool CheckValid();
	bool bIsExit;
	bool bIsValid;
	void SetNotifyWnd(CWnd* notifyWnd);
	ListHwnd m_lstCharID; //Chứa danh sách mapping giữa RoleName và HWND
	CCharacter* GetCharacter(CString sName);
	CCharacter* GetAllCharacter();
	
	int iOS; //yyxx (yy: 61-win7|51-winxp, xx: 64|32) 

	
	
DECLARE_MESSAGE_MAP()
private:
	bool		ReadFile(const CString& xmlFile);
	//void		CALLBACK OnTimer ( HWND hWnd,UINT nMsg,UINT_PTR nIDEvent,DWORD dwTime );

	CWnd*		m_notifyWnd;
	CCharacter* m_char;	
	CString sRole;
	//CString sServerName;


	

	ListRole m_lstRole; //Chứa danh sách các role
		
	

	static UINT InfoChanged( LPVOID pParam );
	static UINT AddRoleList( LPVOID pParam );

	


};

//static HWND m_Hwnd;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
BOOL IsMainWindow(HWND handle);


