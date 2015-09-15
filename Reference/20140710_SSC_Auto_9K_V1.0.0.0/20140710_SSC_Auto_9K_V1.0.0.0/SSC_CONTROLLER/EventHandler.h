// Filename: EventHandler.h 
// 13-Dec-2013 SSC copy right.

#pragma once

#include <map>
#include <list>

// Forward class declarations.
class CWndDlg;
class CGroupControl;
class CWndControl;
class CWndCheckBox;
class CWndTextBox;
class CWndComboBox;
class CMainDlg;
class CWndSkillCfg;
class CWndPriority;


static CWndSkillCfg* m_wndSkillCfgDlg;
static CWndPriority* m_wndPriorityDlg;
// CEventHandler

class CEventHandler 
{
public:
	CEventHandler ();
	virtual ~CEventHandler  ();
	//Event
	typedef void (*function)(CWndControl*,CMainDlg*);
	std::map<CString, function> events;	
	
public:
	void AddEventHandler(CString eventName,CEventHandler::function func);
	void RemoveEventHandler(CString eventName);
	void ClearEventHandler();
	CEventHandler::function RaiseEvent(CString eventName);
	//Event handle
	static void SetPriority(CWndControl* ,CMainDlg*);
	static void BtnConfigSkill(CWndControl* ,CMainDlg*);
	static void BtnConfigBuff(CWndControl* ,CMainDlg*);
	static void BtnConfigPriority(CWndControl* ,CMainDlg*);
	//static void CmbFollow(CWndControl* ,CMainDlg*);
	static void ChkFightToDie(CWndControl* ,CMainDlg*);
	static void ChkAutoFight(CWndControl* ,CMainDlg*);

	/////////////tab bãi quái//////////////////////
	static void AddMonsterCoor(CWndControl* ,CMainDlg*);
	static void RemoveMonsterCoor(CWndControl* ,CMainDlg*);
	static void UpMonsterCoor(CWndControl* ,CMainDlg*);
	static void DownMonsterCoor(CWndControl* ,CMainDlg*);
	static void RemoveAllMonsterCoor(CWndControl* sender,CMainDlg* parent);
	//static void PkOnMove(CWndControl* sender,CMainDlg* parent);
	//static void RangePoint(CWndControl* sender,CMainDlg* parent);
	static void LoadTrain(CWndControl* sender,CMainDlg* parent);
	static void SavePoint(CWndControl* sender,CMainDlg* parent);

	//tab nhiệm vụ
	static void UpAward(CWndControl* ,CMainDlg*);
	static void DownAward(CWndControl* ,CMainDlg*);
	static void QuestInCaveCheck(CWndControl* ,CMainDlg*);

	///////tab party_NghiaLP
	static void AutoPartyChecked(CWndControl* sender,CMainDlg* parent);

	static void AddMemberParty(CWndControl* ,CMainDlg*);
	static void RemoveMemberParty(CWndControl* ,CMainDlg*);
	static void RemoveAllMember(CWndControl* ,CMainDlg*);
	static void SaveMemberParty(CWndControl* ,CMainDlg*);
};


