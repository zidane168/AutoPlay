#pragma once

#include "Markup.h"
#include <map>
#include <vector>
#include "UserData.h"




#define COUNT_TIME			30 //Time check lag
#define CHAT_TIME			6000 //Time auto chat
#define ELEMENT_SIZE		5 
#define AWARD_SIZE			6 
#define FACTION_SIZE		10
#define MSG_SIZE			2






class CNPC;
class XMLSerialized;
// CCharacter

class   __declspec(dllexport)  CCharacter 
{

public:
	//Ham thông tin
	CCharacter();
	virtual ~CCharacter();
	void Init();
	CString GetName();
	bool InitInfo();
	void GetInfo();
	//Ham chuc nang luu load du lieu
	bool LoadSkills();
	bool LoadPoint();
	//bool LoadMap();
	void LoadElement();
	void LoadFaction();
	void LoadSkillCfg();
	void LoadBuffCfg();
	void SaveSkillCfg(CString sName = L"Skills");
	void SavePoint();
	void SaveInfo();
	void AddNPC(int mapID,CString sName,int iCorX,int iCorY);
	POS* GetNPC(int mapID,CString sName);
	//ham chuyen doi
	int GetDistance(COOR);
	int GetDistance(POS);
	void ConvertMapToScreen(COOR*);
	void ConvertScreenToMap(COOR*);
	//Ham chuc nang thuc thi
	void Reset();
	void Search();
	bool SearchNPC();
	void GetCaptain();
	void DoFight();
	void DoBuff();
	bool IsLag();
	void MoveTo();
	bool MoveTo(int ,int );
	bool MovingToTarget();
	void RideHorse();
	void SetTarget();
	void SetAutoFight();
	void SetSkill(int);
	bool DoRightSkill(int iSkill,int iTarget,int iDelay);
	bool GoingtoLead();
	int	 GetPkTarget();	
	bool CheckSupport();
	void KeyPress(int);
	void AutoChat();
	void AutoEat();
	void BackToCity();
	bool MovingToTrainPoint(bool bNext=false);
	bool GoingToPoint();
	//bool CheckMapChange();
	bool FindingAWay(int iDes,int iX,int iY,bool clickLink=false);
	bool GoingToTrain();
	bool LoadTrain(std::map<int,CString> &lst);
	bool ChooseMenu(int iMenuIndex); //(x:Index tính từ dòng cuối lên, bắt đầu từ 0 | >=10x: Index tính từ trên xuống)
	bool ChooseAward();
	bool GetLinkPos(CString szLinkText);
	bool ClickOnLink();
	bool OpenGoBackRock();	
	int GetItemInPack(CString sName);
	CString GetItemNameByIndex(int iIndex);
	void ShowMessage(CString sMsg);
//Thong tin nhan vat	
	HANDLE m_hHandle;
	HWND m_hwnd;
	DWORD m_dwProId;
	int m_iIndex;
	CString m_szName;
	CString t_szName; //tên tạm dùng để truy vấn khi gamer out
	CString m_szTitle;
	CString m_szFamily;
	CString m_sServerName;
	int m_iCurHP;
	int m_iMaxHP;
	int m_iCurMP;
	int m_iMaxMP;
	int m_iTdX;
	int m_iTdY;
	int	m_iCurentTarget;
	int m_iCurMapId;
	bool m_bIsRiding;
	int m_iFaction;
	int m_iParty;
	int m_iColor;
	int m_iLevel;
	int m_iState; //1: Stand, 2: Run, 3: Sit, 7: Attack, 15:Die	
	int m_iFight; //1: tu dong chien dau, 0: ko chien dau
	byte m_siPkState; //0: luyện công | 1: Trận doanh | 2: Gia tộc, bang | 3: Đồ sát
	byte m_siSafe; //0: trong thành| 1: ngoài thành
	int m_iPkTarget;//index đang bị đánh
	int m_iActiveSkill;
	std::map <CString,SKILL_DATA*> m_mSkills;
	byte* m_arrElement;
	byte* m_arrFaction;
	
	

//Thong tin tinh nang
	bool	m_bIsHooking;
	int		m_iTargetIdx;
	int		m_iRange;
	int		m_iRangePoint;
	int		m_iPriority; //0:khoảng cách | 1: Ngũ hành | 2: Môn phái |3: Lượng HP
	bool	m_bPkFollowLead;//Đánh theo đội trưởng
	bool	m_bFollowLead;//theo sau đội trưởng
	bool	m_bAutoRide; //tự lên xuốn ngựa
	bool	m_bPkFirst; //Ưu tiên đánh người
	bool	m_bBlackList;//Danh sách đen
	bool	m_bFightToDie;//Đánh tới chết
	bool	m_bNotFollow; //Không đuổi theo
	int		m_iDistance; //Khoảng cách theo sau
	bool	m_bAutoGoto;//Tự động tìm đến
	bool	m_bAutoRevival;//Tự động hồi sinh
	bool	m_bAutoFight;//Tự đánh
	int		m_iKeyFight;//Đánh bằng phím
	int		m_iHpLimit;//Giơi hạn phục hồi HP
	int		m_iKeyHP;//phím phục hồi HP
	int		m_iKeyMP;//phím phục hồi HP
	bool	m_bAutoBackToTrain;//tự quay lại bãi quái
	bool	m_bQuickAward; //quay về nhận thưởng nhanh (dùng hồi thành thạch)
	bool	m_bQuestInCave;//lịch luyện trong thiên tâm cốc
	int		m_iTrainMap;
	CAPTAIN m_cLead;//Đội trưởng	
	byte	siCurMoster;
	bool	bIsRunning;
	//int		iOldMap;
	POS		cTarget;


	bool	m_bAutoEat;
	CString	GetMapName(int mapId);
	int		GetMapId(CString mapName);

	
	//========NghiaLP===========
	////////tab bãi quái/////////////
	

	int		GetNumberBag();
	bool	PickBag();
	bool	m_bPickBag;
	

	//////////tab tổ đội//////////////////
	//////Nghia -20141703-Tu dong party//////////
	//int     m_iKeyParty;      //Phím mở tổ đội(P)
	//DWORD   m_dwCheckParty;	  //Trạng thái check party 0_không đồng ý 1_Đồng ý
	bool	m_bInterParty;  //Dong y vao doi
	bool	m_bCreateParty;	 //Lap to doi

	std::vector <CString> m_vMemberParty;      //Luu danh sach thanh vien moi vao to doi

	CString m_szNameParty;    //Tên đội nhan vat minh muon vao doi
	CString GetNameParty(); //Kiểm tra hộp thoại mời party và lấy tên người mời party

	void InterParty();			//Vào đội của người khác (được mời)

	void CreateParty();
	void InviteParty();//mời tổ đội theo danh sach
	void SaveMemberInvite();
	//========luu load du lieu
	void LoadMemberParty();
	bool IsMember(CString sName);
	int GetPartySize();
	void AutoPartyCheck(byte check);
	
	CString ConvertLower(CString);
	
	//==================Van Tieu===================
	int GetNPCInfo(CString);
	void GotoHorse();
	//int Attacked(int);
	int TotalLineMenu();
	//=====20140421========
	void GetLocation();
	bool ReceiveAward();
	bool ReceiveEscort();
	bool ProcessEscort();
	//====20140428======
	bool ClickRecived();
	CString FindNPC(CString);

	//====20140603=======
	CString GetNameMap(CString);
	
	////=======Dieu Doi============
	bool OpenDialogQuest();
	bool ChooseQuest(int);
	bool SendTeamDoQuest();
	CString GetBaseText(int);
	int		ConvertToInt(int);
	

	
	////========20140609======
	bool SendTeamGoTo();
	//======Uu tien lam nhiem vu
	void PriorityQuest();
	bool CheckFinishQuest(CString);
	
	void GetDoQuest();   //dang lam nv nao
	//==========Bien Dieu Doi================
	bool	m_bSendTeamDoQuest;
	int		m_iKindBC;
	bool	m_bBC;
	bool	m_bTVP;
	bool	m_bVT;
	bool	m_bTN;
	bool	m_bTVPhai;

	bool	m_bWaiting;   //Trang thai dang dieu doi

	int		m_iQuesting; //dang lam nv: 0: ko co nhiem vu; 1: Dieu Doi; 2: Van Tieu; 3: Lich luyen

	//===============kết thúc================


	//==========Bien Van tieu==============
	bool	bMenuSel;   //lay so luong offset
	bool    m_bEscort;		//Check chon nhiem vu van tieu
	int		m_iEscort;		//Chon loai hinh van tieu
	int		m_iCurEscort;//Bước nhiệm vụ đang thực hiện (0: chưa nhận, 1: đã nhận, 2: tìm đến mục tiêu,3: thực hiện,4: chuyen toi nguoi uy thac, 5:trả nhiệm vụ)
	int		m_iProsessType;    //1: Tới nguoi uy thac, 2: Đang hộ tống, 3: hoàn thành, 4:Toi dich tram
	bool	m_bReciveAdward;
	CAPTAIN m_cHorse;//THONG TIN VAN TIEU
	CString m_szState;
	int		m_iBackHorse_X;
	int		m_iBackHorse_Y;
	int		m_iBackHorse_MapID;
	

	//===============kết thúc================

	//Nhiem vu
	int		m_iQuestNum;//So lan thuc hien nhiem vu (0: chua nhan, >0: nhiem vu dang nhan, <0: da hoan thanh du so lan)
	int		m_iCurQuest;//Bước nhiệm vụ đang thực hiện (0: chưa nhận, 1: đã nhận, 2: tìm đến mục tiêu,3: thực hiện,4:trả nhiệm vụ)
	int		m_iQuestType; //1:NPC, 2: Item, 3:Monster
	CString m_szQuestTarget;
	bool	m_bAutoQuest;
	void	LoadAward();
	CString	GetQuestText(int iIndex);
	CString	GetQuestText(CString szTitle);
	int		GetQuestIndex(CString szTitle);
	int		GetAwardIndex(CString szTitle);
	bool	ReceiveQuest(); 
	bool	ProcessQuest(); 
	bool	ReceiveAward(CString szText); 
	int		AskNPC(CString szName,bool clickLink=false);//0: chưa mở menu, 1:menu chọn, 2:menu chọn phần thưởng
	bool	FightMonster(CString szText);
	bool	UseQuestItem();	
	//DWORD	m_dwQuestTextAddress;
	//DWORD	m_dwAwardTextAddress;
	byte*	m_arrAward;
	std::map<int,CString> m_mAward;




	std::map<CString,POS*> m_mNPCQuest;//m_mDailyQuest;

////////////SangDH--20140602--Fix lỗi dùng hồi thành thạch khi làm nhiệm vụ đánh quái
//							--Fix lỗi tìm đánh quái lúc làm nhiệm vụ
	int	 GetHitMeTarget();
	int	SearchNPCByName(CString sName);
	int CompareString(CString sStr1,CString sStr2, bool exact=false);//Trả về % dự trùng lặp từ trong chuỗi
	static CString ConvertString(CString str);
	bool IsInterval(int ID,int iDelay);
	void RemoveInterval(int ID);
	DWORD dwGameBaseAddress;
	typedef std::map <int,time_t> TIMER;

	TIMER m_mDelay;





	///Danh sách
	typedef std::map <int,SKILL_CFG*> ListSkillConfig;
	ListSkillConfig m_mSkill;
	ListSkillConfig m_mBuff;
	std::map <int,time_t> m_mSkillDelay;
	std::map<CString,int> mResult;
	std::vector <COOR> m_lMonsters;
	//CMarkup* xmlMap;
	CNPC*	m_npc;
	XMLSerialized*	serialize;
	int		m_iOS;//yyxx (yy: 61-win7|51-winxp, xx: 64|32) 

	//time_t	tMoveDelay;
	
//Thong tin chua xac dinh
	
	



//	void Recover();
//	
//	
//	

//	
//	

//	
//	void Quit();

//	
//	
//	bool CheckSkillDelay(bool);
//
//
//	

//	
//
//	
//	
//
//
//	void PickItem();
//	bool SearchItem();
//	void BuffHP();
//	void BuffMP();

//	int m_iRangeFight;

//	int m_iItemIdx;

//	//std::map <int,SKILL_CFG*> m_mBuff;	
//	
//	
//	bool	m_bAutoHP;
//	bool	m_bAutoMP;
//	

//	bool	m_bAutoPick;
//	
//	int		m_iLimit; //0: tất cả | 1: đánh người | 2: đánh quái

//	int		m_iLimitHP;
//	int		m_iLimitMP;
//	
//	
//	
//	int		m_iKeyMP;
//	int		m_iKeyHP;
//	int		m_iDelayHP;
//	int		m_iDelayMP;
//	
private:
	int		iLag; //Index của NPC bị lag, bỏ qua khi dò tìm
	bool	bBuff;
	int		iNpcHP;
	int		iCountTime;

//	bool	bIsRunning;
//	bool	bIsClick;
//	
//	COOR	cLastClick;
//	
	
	
	

private:
	/*bool	IsMoving();
	bool	IsClick();*/
	
	bool CheckHotKey(byte shcutKey);

	int		iDelayChat;

	DWORD dwGameInfoAddress;
	DWORD dwMapAddress;

	DWORD dwShortcutBaseAddress;

	DWORD dwDoFight;
	
};



