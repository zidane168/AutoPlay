#pragma once


class CCharacter
{
public:
	CCharacter(void);
	virtual ~CCharacter(void);
	void Init(void);
	bool Move(HWND hWnd,LPARAM);
	bool MoveAway(HWND hWnd,int iMapIndx);
	int SetTarget(int );
	bool SetSkill(int iSkillId);
	bool DoRightSkill(HWND,int iSkillId);
	bool BackToCity(HWND hWnd);
	
	bool Click(DWORD);
	bool RideHorse(HWND  );
	bool Recover(HWND  );
	bool KeyPress(HWND , int );
	bool Quit();
	CString GetName();
	bool OpenMap(HWND  );
	bool GetWorldMapAddress();
	bool ChooseMap(HWND,int mapIndx);

	int OpenMenu(HWND);
	bool OpenFeatureDialog(HWND hWnd,int iIndex,bool open=true);//index tinh tu 0 (0: nhan vat, 1: tui,...)
	bool OpenDialog( DWORD add,bool open=true);
	bool SelectedMenu(HWND,int menuIndx);
	bool ClickOnItem(HWND hwnd,int blockIndx);
	bool ClickOnItemLink(HWND hwnd,int blockIndx);
	bool ClickOnLink(HWND hwnd,int Indx);
	bool ChooseAward(HWND hwnd,int Indx);


	//Bien
	bool m_bAutoFight;

	DWORD m_dwBase;
	DWORD m_dwNpcBase;
	DWORD m_dwBackToCityBase;
	DWORD m_dwWorldMap;
	DWORD m_dwWorldMapButton;
	DWORD m_dwWorldMapCity;
	DWORD m_dwSceneMap;
	DWORD m_dwMenu;
	DWORD m_dwLink;
	DWORD m_dwPackage;
	DWORD m_dwMapAddress;


private:
	bool bLinkSel;//xu ly tam thoi chuc nang nhan thuong xong ko di chuyen khi click link
	bool bMenuSel;
	bool bMapSel;

	int		iOS;


	/////=============NghiaLP==============
public:
	void GetCheckPartyAddress(HWND hwnd);
	bool AgreeParty(HWND hwnd,bool checked);
	void CreatePartyInList(HWND hWnd,DWORD dwMenuBase);
	void GetButtonPartyAddress();
	void CheckAutoParty(HWND hWnd,byte check);
	//===========Van Tieu==============
	bool RecivedAward(HWND);
	//==========Dieu Doi==============
	bool OpenDialogQuest();
	bool ChooseQuest(int);
	DWORD GetBaseText(int);

	/////20140611/// Base Lich Su Kien======
	DWORD CheckFinishQuest(int);

	bool OpenListEvent();
private:
	DWORD dwCheckInviteParty;
	DWORD dwAgreeButtonParty;
	DWORD dwDenyButtonParty;
	DWORD dwBaseSelectMenu;
	//=========================================

	//=======Dieu Doi===========
	DWORD dw_GetBase;
	bool bChooseQuest;
	bool bSendTeam;

	DWORD dwCheckFinish;


	
};
