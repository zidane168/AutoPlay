/*
20140613 - SangDH UPDATE game v2.36.1, khong thay doi nhiu: them 1 offset(158->159) ham ClickOnItem, offset(160->161) hamGetWorldMapAddress
	+ them ham GetDoQuest(): xet trang thai lam nv, trong file GameEngine.cpp them phan Lich Luyen vao Chung pphan Lich Luyen va dieu doi
20140612-SangDH-Hàm IsInterval(): hàm cho phép gán thời gian delay cho 1 hành động dựa vào ID (true: tạm dừng | false: tiếp tục)
	+ RemoveInterval(): xóa interval
	+ Thay đổi delay cho các hàm
	+ thay đổi hàm SearchNPCByName(): Search đúng NPC khi làm nhiệm vụ lịch luyện 
20140611-SangDH-Thay đổi cách lấy base Address hàm Init()
	+ Thêm hàm ConvertString() chuyển chuỗi sang Unicode
//cap nhat base file Process.cpp
//them mot so doan code hoan thien fix lõi nho: RecieveEscort(), GotoHorse
//20140605 == Them code trong ham GoToHourse() xu ly nv chet quay lai cho cu, nhiem vu that bai quay ve nhan nv khac
//20140604 == Them tinh nang dieu doi
// Character.cpp : implementation file
//
//Interval: 1: di chuyen | 2: di chuyen cung map | 3: hồi thành | 4: mời party | 5: check đồng ý mời party
*/
#include "stdafx.h"
#include <math.h>
#include "Character.h"
#include "GameInfo.h"
#include "NPC.h"
#include "Item.h"
#include "Serialize.h"
#include <string>
char* chatMsg[]  = {
	"Da co auto tai autongaokiem.com",
	"download auto tai autongaokiem.com",
	"Auto PK tai autongaokiem.com",
	"Dang su dung auto PK tai autongaokiem.com",
	"Da cap nhat tinh nang train quai"
};

char* MpFood[]  = {
	"Bánh Ngô",
	"Màn Thầu",
	"Bánh Hấp",
	"Bánh Bí Ngô",
	"Bánh Bao"
};

// CCharacter
const UINT WM_HOOK_WRITE = RegisterWindowMessage(L"WM_HOOK_WRITE");

using namespace std;

CCharacter::CCharacter()
{
	m_iOS = 0;
	m_hHandle = NULL;
	m_hwnd = NULL;
	m_szName = L"";
	t_szName = L"";
	m_szTitle=L"";
	m_szFamily = L"";
	m_sServerName=L"";
	m_iCurHP = 0;
	m_iMaxHP = 0;
	m_iCurMP = 0;
	m_iMaxMP = 0;
	m_iCurentTarget = 0;
	m_iCurMapId=0;
	m_siPkState =0;
	m_iState =0;
	m_iFaction = 0;
	m_iParty=0;
	m_iColor=0;
	m_iLevel=0;
	m_iActiveSkill=0;
	m_iPkTarget=0;
	//=============================
	m_iFight = 0;
	//Thong tin tinh nang
	m_bIsHooking = false;
	m_iPriority=0;
	m_iRange = 500;
	m_iRangePoint=1000;
	m_bPkFollowLead = false;
	m_bFollowLead = false; //true;
	m_bAutoRide=true;
	m_bPkFirst=false;
	m_bBlackList=false;
	m_bFightToDie = false;
	m_bNotFollow = false;	 
	m_iDistance=200; 
	m_bAutoGoto=false;
	m_bAutoRevival=true;
	m_bAutoFight = true;
	m_iKeyFight=0;
	m_iHpLimit=50;
	m_iKeyHP=0;
	m_iKeyMP=0;
	m_bAutoEat = false;
	siCurMoster = 0;
//	iOldMap=0;
	bIsRunning = false;
	m_bAutoBackToTrain = true;
	m_bQuickAward = false;	
	m_iTrainMap=0;
	m_mSkill.clear();
	m_mBuff.clear();

	m_bAutoQuest=false;
	m_bQuestInCave = false;
	m_iQuestNum=0;
	m_iCurQuest=0;
	m_mNPCQuest.clear();
//	m_dwQuestTextAddress=0;
//	m_dwAwardTextAddress=0;
	m_arrAward = new byte[AWARD_SIZE+1];
	m_arrAward[0] = AWARD_SIZE ; //index 0 is size of array
	m_mAward.clear();

	iLag=0;
	m_npc = NULL;
	iNpcHP=0;
	iCountTime=COUNT_TIME;
	iDelayChat = 0;
	m_iTargetIdx = 0;
	serialize = NULL;
	m_arrElement = new byte[ELEMENT_SIZE+1];
	m_arrElement[0] = ELEMENT_SIZE ; //index 0 is size of array
	m_arrFaction = new byte[FACTION_SIZE+1];
	m_arrFaction[0] = FACTION_SIZE ; //index 0 is size of array


	//////////NghiaLP//////////////
	m_bPickBag=false;
	m_bInterParty=false;
	m_szNameParty=L"";

	m_bCreateParty = false;
	m_vMemberParty.clear();
	//========Van Tieu==========
	bMenuSel = false;
	m_bEscort = false;
	m_iEscort = 0;
	m_iCurEscort = 0;
	m_iProsessType = 0; 
	m_bReciveAdward = false;
	m_szState = L"";
	m_iBackHorse_MapID = 0;
	m_iBackHorse_X = 0;
	m_iBackHorse_Y = 0;

	//========Dieu Doi===========
	m_bSendTeamDoQuest = false;
	m_bBC = false;
	m_iKindBC = 0;
	m_bTVP = false;
	m_bVT = false;
	m_bTN = false;
	m_bTVPhai = false;
	m_bWaiting = false;

	m_iQuesting = 0;
}

CCharacter::~CCharacter()
{
	m_iOS = 0;
	m_hHandle = NULL;
	m_hwnd = NULL;
	m_szName = L"";
	t_szName = L"";
	m_szTitle=L"";
	m_szFamily = L"";
	m_sServerName=L"";
	m_siSafe =0;
	m_iCurHP = 0;
	m_iMaxHP = 0;
	m_iCurMP = 0;
	m_iMaxMP = 0;
	m_iCurentTarget=0;
	m_iCurMapId=0;
	m_iFaction = 0;
	m_iParty=0;
	m_iColor=0;
	m_siPkState =0;
	m_iLevel=0;
	m_iActiveSkill=0;
	m_iPkTarget=0;
	m_iFight = 0;

	m_bIsHooking = false;
	m_iPriority=0;
	m_iRangePoint=0;
	m_iRange = 0;
	delete m_npc;
	m_npc = NULL;
	m_iTargetIdx = 0;
	m_dwProId = 0;
	m_mSkill.clear();
	m_mBuff.clear();
	m_bPkFollowLead = false;
	m_bAutoRide=false;
	m_bFollowLead = false; //////////////////
	m_bPkFirst=false;
	m_bBlackList=false;
	m_bFightToDie = false;
	m_bNotFollow = false;
	m_iDistance=0; 
	m_bAutoGoto=false;
	m_bAutoRevival=false;
	m_bAutoFight = false;
	m_iKeyFight=0;
	m_iHpLimit=0;
	m_iKeyHP=0;
	m_iKeyMP=0;
	m_bAutoEat = false;

	m_bAutoQuest=false;
	m_bQuestInCave = false;
	m_iQuestNum=0;
	m_iCurQuest=0;
	m_mNPCQuest.clear();
//	m_dwQuestTextAddress=0;
//	m_dwAwardTextAddress=0;
	m_mAward.clear();

	m_mSkills.clear();
	m_mBuff.clear();
	m_mSkillDelay.clear();
	m_lMonsters.clear();
	if(serialize!=NULL)
	delete serialize;
	serialize = NULL;
	delete []m_arrElement;
	m_arrElement=NULL;
	delete []m_arrFaction;
	m_arrFaction=NULL;	
	delete []m_arrAward;
	m_arrAward=NULL;	

	iLag=0;
	iNpcHP=0;
	iCountTime=0;
	iDelayChat=0;
	siCurMoster = 0;
//	iOldMap=0;
	bIsRunning = false;
	m_bAutoBackToTrain=false;
	m_bQuickAward = false;
	m_iTrainMap=0;

	//////////NghiaLP//////////////
	m_bPickBag=false;
	m_bInterParty=false;
	m_szNameParty=L"";
	m_bCreateParty = false;
	m_vMemberParty.clear();

	//========Van Tieu==========
	m_bEscort = false;
	m_iEscort = 0;
	m_iCurEscort = 0;
	m_iProsessType = 0; 
	m_bReciveAdward = false;
	m_szState = L"";

	m_iBackHorse_MapID = 0;
	m_iBackHorse_X = 0;
	m_iBackHorse_Y = 0;
	
	//========Dieu Doi===========
	m_bSendTeamDoQuest = false;
	m_bBC = false;
	m_iKindBC = 0;
	m_bTVP = false;
	m_bVT = false;
	m_bTN = false;
	m_bTVPhai = false;
	m_bWaiting = false;

	m_iQuesting = 0;
	
}

void CCharacter::Init(){
	DWORD dwPlayerIndexAddress;
	//Get Base Address win64
	//if(m_iOS==WIN_XP_64||m_iOS==WIN_7_64)
	//	ReadProcessMemory(m_hHandle, (void*)GAME_BASE_ADDRESS, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	////Get Base Address win32
	//else{
	//	if(m_iOS==WIN_XP_32){
	//		ReadProcessMemory(m_hHandle, (void*)GAME_BASE_ADDRESS_XP_32, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	//	}
	//	else if(m_iOS==WIN_7_32){
	//		ReadProcessMemory(m_hHandle, (void*)GAME_BASE_ADDRESS_32, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	//	}
	//	dwGameBaseAddress = dwGameBaseAddress + GAME_OFFSET_32;
	//	ReadProcessMemory(m_hHandle, (void*)dwGameBaseAddress, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	//}
	////Đọc thông tin
	//dwGameBaseAddress = dwGameBaseAddress + GAME_OFFSET_1;
	//ReadProcessMemory(m_hHandle, (void*)dwGameBaseAddress, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	//
	//
	//Get current map


	dwMapAddress = dwGameBaseAddress + GAME_OFFSET_MAPID;
	ReadProcessMemory(m_hHandle, (void*)dwMapAddress, &dwMapAddress, (DWORD)sizeof(dwMapAddress), 0);
	dwMapAddress = dwMapAddress + NPC_MAP;
	//Get player Index
	dwPlayerIndexAddress = dwGameBaseAddress + GAME_OFFSET_INDEX;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerIndexAddress, &dwPlayerIndexAddress, (DWORD)sizeof(dwPlayerIndexAddress), 0);
	dwPlayerIndexAddress = dwPlayerIndexAddress + NPC_INDEX;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerIndexAddress, &dwPlayerIndexAddress, (DWORD)sizeof(dwPlayerIndexAddress), 0);
	m_iIndex = dwPlayerIndexAddress;
	
	//Get player Info address
	dwGameInfoAddress = dwGameBaseAddress + GAME_OFFSET_INFO;
	ReadProcessMemory(m_hHandle, (void*)dwGameInfoAddress, &dwGameInfoAddress, (DWORD)sizeof(dwGameInfoAddress), 0);
	dwGameInfoAddress = dwGameInfoAddress + GAME_OFFSET_DATASIZE * dwPlayerIndexAddress;
	ReadProcessMemory(m_hHandle, (void*)dwGameInfoAddress, &dwGameInfoAddress, (DWORD)sizeof(dwGameInfoAddress), 0);

	//get server name
	DWORD dwServerName = dwGameBaseAddress + GAME_OFFSET_NAME;	
	ReadProcessMemory(m_hHandle, (void*)dwServerName, &dwServerName, (DWORD)sizeof(dwServerName), 0);
	dwServerName = dwServerName + GAME_SERVER_OFFSET;
	
	char sValue[NPC_NAME_SIZE];
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(m_hHandle, (void*)dwServerName, &sValue, (DWORD)sizeof(sValue), 0);
	m_sServerName  = (CString)sValue;
	
	//string converStr( );
	wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( m_sServerName.GetString() ));
	m_sServerName = wsResult.c_str();
	
	//Chien Dau(Send F)
	dwDoFight = dwGameBaseAddress + NPC_STATEFIGHT;	
	//get map ID
//	ReadProcessMemory(m_hHandle, (void*)dwMapAddress, &iOldMap, (DWORD)sizeof(iOldMap), 0);

	GetName();	
}

/************************************************************************
ReadCharacterName - 01/07/2014
	Trả về giá trị tương ứng => DWORD PTR SS:[dwAddress]
return:
	- CString: tên nhân vật tương ứng với handle
*************************************************************************/
CString CCharacter::GetName()
{
	// Current Player
	DWORD dwPlayerName = dwGameBaseAddress + GAME_OFFSET_NAME;	
	ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &dwPlayerName, (DWORD)sizeof(dwPlayerName), 0);
	dwPlayerName = dwPlayerName + NPC_NAME;
	
	char sValue[NPC_NAME_SIZE];
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &sValue, (DWORD)sizeof(sValue), 0);
	m_szName = (CString)sValue;
	
	//string converStr( );
	wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( m_szName.GetString() ));
	m_szName = wsResult.c_str();

	if(m_szName != L"")
		t_szName = m_szName;
	// Get npc title
	dwPlayerName = dwGameInfoAddress + NPC_TITLE;
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &sValue, (DWORD)sizeof(sValue), 0);
	m_szTitle = (CString)sValue;
	wsResult          =XMLSerialized::UTF8ToUnicode( (string)CW2A( m_szTitle.GetString() ));
	m_szTitle = wsResult.c_str();

	return m_szName;
}

void CCharacter::GetInfo()
{
	char sValue[NPC_NAME_SIZE];
	
	// CurrentHP
	DWORD dwPlayerInfo= dwGameInfoAddress + NPC_CURRENT_HP;;

	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iCurHP, (DWORD)sizeof(m_iCurHP), 0);

	// CurrentMP
	dwPlayerInfo = dwGameInfoAddress + NPC_CURRENT_MP;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iCurMP, (DWORD)sizeof(m_iCurMP), 0);

	// MaxHP
	dwPlayerInfo = dwGameInfoAddress + NPC_MAX_HP;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iMaxHP, (DWORD)sizeof(m_iMaxHP), 0);

	// MaxMP
	dwPlayerInfo = dwGameInfoAddress + NPC_MAX_MP;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iMaxMP, (DWORD)sizeof(m_iMaxMP), 0);

//	 Get target Index
	dwPlayerInfo = dwGameInfoAddress + NPC_TARGET;
	 ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iCurentTarget, (DWORD)sizeof(m_iCurentTarget), 0);

	//Get coor
	dwPlayerInfo = dwGameInfoAddress + NPC_COOR_X;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iTdX, (DWORD)sizeof(m_iTdX), 0);
	dwPlayerInfo = dwGameInfoAddress + NPC_COOR_Y;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iTdY, (DWORD)sizeof(m_iTdY), 0);

	// Get ride horse state
	dwPlayerInfo = dwGameInfoAddress + NPC_RIDE_HORSE;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_bIsRiding, (DWORD)sizeof(m_bIsRiding), 0);

	// Get npc faction
	dwPlayerInfo = dwGameInfoAddress + NPC_FACTION;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iFaction, (DWORD)sizeof(m_iFaction), 0);

	// Get npc faction
	dwPlayerInfo = dwGameInfoAddress + NPC_TITLE;
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &sValue, (DWORD)sizeof(sValue), 0);
	m_szTitle = (CString)sValue;
	wstring wsResult          =XMLSerialized::UTF8ToUnicode( (string)CW2A( m_szTitle.GetString() ));
	m_szTitle = wsResult.c_str();
	// Get npc Level
	dwPlayerInfo = dwGameInfoAddress + NPC_LEVEL;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iLevel, (DWORD)sizeof(m_iLevel), 0);

	// Get npc state
	dwPlayerInfo = dwGameInfoAddress + NPC_STATE;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iState, (DWORD)sizeof(m_iState), 0);
	// Get npc PK state
	dwPlayerInfo = dwGameInfoAddress + NPC_PK_STATE;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_siPkState, (DWORD)sizeof(m_siPkState), 0);

	// Get active skill
	dwPlayerInfo = dwGameInfoAddress + NPC_ACTIVE_SKILL;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iActiveSkill, (DWORD)sizeof(m_iActiveSkill), 0);

	// Get family
	dwPlayerInfo = dwGameInfoAddress + NPC_FAMILY;	
	memset(sValue, 0x0, (sizeof(sValue)));
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, sValue, (DWORD)(sizeof(sValue)), 0);
	m_szFamily = (CString)sValue;
	// Get fight status
	dwPlayerInfo = dwGameInfoAddress + NPC_SAFE;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_siSafe, (DWORD)sizeof(m_siSafe), 0);
	// Get party
	dwPlayerInfo = dwGameInfoAddress + NPC_PARTY;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iParty, (DWORD)sizeof(m_iParty), 0);
	// Get name color
	dwPlayerInfo = dwGameInfoAddress + NPC_NAME_COLOR;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iColor, (DWORD)sizeof(m_iColor), 0);

	//Get current map
	ReadProcessMemory(m_hHandle, (void*)dwMapAddress, &m_iCurMapId, (DWORD)sizeof(m_iCurMapId), 0);
}

void CCharacter::AddNPC(int mapID,CString sName,int iCorX,int iCorY){
	POS *pos = new POS();
	pos->_iX = iCorX;
	pos->_iY = iCorY;
	pos->_iMapId = mapID;
	CString id;
	id.Format(L"%d"+sName,mapID);
	m_mNPCQuest.insert(make_pair(id,pos));
}
POS* CCharacter::GetNPC(int mapID,CString sName){
	CString id;
	id.Format(L"%d"+sName,mapID);
	std::map<CString,POS*>::iterator item;
	item = m_mNPCQuest.find(id);
	if(item==m_mNPCQuest.end())
		return NULL;
	return item->second;
}

bool CCharacter::InitInfo(){
	////Tạo đối tượng mapping
	serialize = new XMLSerialized(L"Character\\" + m_szName + L".xml");
	//tạo đối tượng NPC
	m_npc = new CNPC(m_hHandle,dwGameBaseAddress);

	//Get thông tin cơ bản
	GetInfo();
	//Khởi tạo danh sách ngủ hành
	LoadElement();
	//khởi tạo danh sách môn phái
	LoadFaction();
	//Load danh sách skill
	LoadSkills();
	//Load giải thưởng
	LoadAward();
	//Load tọa độ bãi quái
	LoadPoint();
	//load các skill & buff 
	LoadSkillCfg();

	LoadBuffCfg();
	LoadMemberParty();
	

	//Add npc nhiem vu
	
	AddNPC(180,L"Tâm Hà",673,389); //Tuong Duong
	AddNPC(176,L"Tâm Hà",737,357); //Thanh Do
	AddNPC(179,L"Tâm Hà",709,359); //bien kinh
	AddNPC(30,L"Tâm Hà",673,389); //Duong Chau
	AddNPC(178,L"Tâm Hà",676,360); //Phuong tuong
	AddNPC(285,L"Tâm Hà",230,436); //Thien Tam Coc

	AddNPC(180,L"Lâm Thiên Tâm",1017,376); //Tuong Duong
	AddNPC(176,L"Lâm Thiên Tâm",1096,332); //Thanh Do
	AddNPC(179,L"Lâm Thiên Tâm",1062,332); //bien kinh
	AddNPC(30,L"Lâm Thiên Tâm",1033,370); //Duong Chau
	AddNPC(178,L"Lâm Thiên Tâm",1032,338); //Phuong tuong

	AddNPC(180,L"Thẩm Chấn Uy",352,284); //Tuong Duong
	AddNPC(176,L"Thẩm Chấn Uy",439,249); //Thanh Do
	AddNPC(179,L"Thẩm Chấn Uy",411,248); //bien kinh
	AddNPC(30,L"Thẩm Chấn Uy",383,279); //Duong Chau
	AddNPC(178,L"Thẩm Chấn Uy",395,251); //Phuong tuong
	
	AddNPC(180,L"Nghiêm Nhiếp Ảnh",499,389); //Tuong Duong
	AddNPC(176,L"Nghiêm Nhiếp Ảnh",550,361); //Thanh Do
	AddNPC(179,L"Nghiêm Nhiếp Ảnh",524,350); //bien kinh
	AddNPC(30,L"Nghiêm Nhiếp Ảnh",507,392); //Duong Chau
	AddNPC(178,L"Nghiêm Nhiếp Ảnh",520,349); //Phuong tuong

	

//Tạo thông tin lưu file
	serialize->Mapping(L"Info",L"", TagType::Root,DataType::None,NULL);
	serialize->Mapping(L"Name",L"/Info",TagType::Attribute,DataType::String,(DWORD)&m_szName);
	serialize->Mapping(L"Range",L"/Info",TagType::Element,DataType::Int,(DWORD)&m_iRange);
	serialize->Mapping(L"Priority",L"/Info",TagType::Element,DataType::Int,(DWORD)&m_iPriority);
	serialize->Mapping(L"Element",L"/Info",TagType::Element,DataType::ByteArray,(DWORD)m_arrElement);
	serialize->Mapping(L"Faction",L"/Info",TagType::Element,DataType::ByteArray,(DWORD)m_arrFaction);
	serialize->Mapping(L"PkFollowLead",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bPkFollowLead);
	//serialize->Mapping(L"FollowLead",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bFollowLead);
	serialize->Mapping(L"AutoRide",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bAutoRide);
	serialize->Mapping(L"PeopleFirst",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bPkFirst);
	serialize->Mapping(L"BlackList",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bBlackList);
	serialize->Mapping(L"FightToDie",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bFightToDie);
	serialize->Mapping(L"NotFollow",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bNotFollow);
	serialize->Mapping(L"DistanceFollow",L"/Info",TagType::Element,DataType::Int,(DWORD)&m_iDistance);
	serialize->Mapping(L"GoTo",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bAutoGoto);
	serialize->Mapping(L"Revival",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bAutoRevival);
	serialize->Mapping(L"AutoFight",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bAutoFight);
	serialize->Mapping(L"KeyFight",L"/Info",TagType::Element,DataType::Int,(DWORD)&m_iKeyFight);
	serialize->Mapping(L"HpRestore",L"/Info",TagType::Element,DataType::None,NULL);
	serialize->Mapping(L"value",L"/Info/HpRestore",TagType::Attribute,DataType::Int,(DWORD)&m_iHpLimit);
	serialize->Mapping(L"key",L"/Info/HpRestore",TagType::Attribute,DataType::Int,(DWORD)&m_iKeyHP);
	

	/////////tab bãi quái//////
	serialize->Mapping(L"AutoEat",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bAutoEat);
	serialize->Mapping(L"MpRestore",L"/Info",TagType::Element,DataType::Int,(DWORD)&m_iKeyMP);
	serialize->Mapping(L"AutoGoBack",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bAutoBackToTrain);
	/////========NghiaLP====20140401================Nhat tui
	serialize->Mapping(L"PickBag",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bPickBag);
	///========ket thuc===============

	///////Tab nhiem vu///////////////////
	serialize->Mapping(L"AutoQuest",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bAutoQuest);
	serialize->Mapping(L"Award",L"/Info",TagType::Element,DataType::ByteArray,(DWORD)m_arrAward);
	serialize->Mapping(L"QuickAward",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bQuickAward);
	serialize->Mapping(L"QuestInCave",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bQuestInCave);

	//======Van Tieu===========
	serialize->Mapping(L"Escort",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bEscort);
	
	//==============Dieu Doi==============================
	serialize->Mapping(L"SendTeam",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bSendTeamDoQuest);
	serialize->Mapping(L"BC",L"/Info/SendTeam",TagType::Attribute,DataType::Bool,(DWORD)&m_bBC);
	serialize->Mapping(L"KindBiCanh",L"/Info",TagType::Element,DataType::Int,(DWORD)&m_iKindBC);
	serialize->Mapping(L"TVP",L"/Info/SendTeam",TagType::Attribute,DataType::Bool,(DWORD)&m_bTVP);
	serialize->Mapping(L"VT",L"/Info/SendTeam",TagType::Attribute,DataType::Bool,(DWORD)&m_bVT);
	serialize->Mapping(L"TN",L"/Info/SendTeam",TagType::Attribute,DataType::Bool,(DWORD)&m_bTN);
	serialize->Mapping(L"TVPhai",L"/Info/SendTeam",TagType::Attribute,DataType::Bool,(DWORD)&m_bTVPhai);



	//////Tab party/////////////////
	serialize->Mapping(L"AutoParty",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bInterParty);
	serialize->Mapping(L"LeaderName",L"/Info",TagType::Element,DataType::String,(DWORD)&m_szNameParty);

	serialize->Mapping(L"CreateParty",L"/Info",TagType::Element,DataType::Bool,(DWORD)&m_bCreateParty);

	//load thong tin tu file len doi tuong
	serialize->Deserialize();
	return true;
}


void CCharacter::SaveInfo(){
	if(serialize!=NULL)
		serialize->SaveInfo();
}



void CCharacter::SaveSkillCfg(CString sName)
{
	
	CMarkup xml=serialize->OpenFile();
	xml.FindElem();
	xml.IntoElem();
	if(xml.FindElem(sName)){
		xml.RemoveElem();
	}

	xml.AddElem(sName);
	xml.IntoElem();
	std::map<int,SKILL_CFG*> lst_cfg;
	if(sName==L"Skills")
		lst_cfg = m_mSkill;
	else
		lst_cfg = m_mBuff;
	std::map<int,SKILL_CFG*>::iterator skill_cfg;
	SKILL_CFG* objSkilCfg;
	for(skill_cfg=  lst_cfg.begin(); skill_cfg != lst_cfg.end(); skill_cfg ++){
		objSkilCfg = skill_cfg->second;
		xml.AddElem(L"Skill");
		xml.AddAttrib(L"Id",objSkilCfg->m_iId);
		xml.AddAttrib(L"Range",objSkilCfg->m_iRange);
		xml.AddAttrib(L"Delay",objSkilCfg->m_iDelay);
	}

	serialize->SaveFile(&xml);

	
}
CString CCharacter::GetMapName(int mapId){
	CMarkup xmlMap;
	xmlMap = XMLSerialized::OpenFile(L"Map.cfg");
	if(xmlMap.GetDoc().GetLength()==0)
		return false;
    xmlMap.ResetPos();
	//xmlMap->FindElem(L"Maps");
	if (!xmlMap.FindElem(_T("Maps")) )
        return false;
	
	if(xmlMap.IntoElem()){
		while(xmlMap.FindElem()){
			if(_tstoi(xmlMap.GetAttrib(_T("id")))==mapId){
				return xmlMap.GetAttrib(_T("name"));
			}
		}
	}
	return L"";
}
int CCharacter::GetMapId(CString mapName){
	CMarkup xmlMap;
	xmlMap = XMLSerialized::OpenFile(L"Map.cfg");
	if(xmlMap.GetDoc().GetLength()==0)
		return false;
    xmlMap.ResetPos();
	//xmlMap->FindElem(L"Maps");
	if (!xmlMap.FindElem(_T("Maps")) )
        return false;
	
	if(xmlMap.IntoElem()){
		while(xmlMap.FindElem()){
			if(xmlMap.GetAttrib(_T("name"))==mapName){
				return _tstoi(xmlMap.GetAttrib(_T("id")));
			}
		}
	}
	return -1;
}

void CCharacter::SavePoint()
{
	CMarkup xml=serialize->OpenFile();
	CString sName = L"Maps";	
	xml.FindElem();
	xml.IntoElem();
	if(xml.FindElem(sName)){
		xml.IntoElem();
		while(xml.FindElem(L"Map")){
			int curMap = m_iCurMapId;
			if(m_iCurMapId != m_iTrainMap && m_iTrainMap!=0)
				curMap = m_iTrainMap;
			if(_ttoi( xml.GetAttrib(L"Id"))== curMap){
				xml.RemoveElem();
				break;
			}
		}
		xml.OutOfElem();
	}
	else
	{
		xml.AddElem(sName);
	}

	if(m_lMonsters.size()==0){
		serialize->SaveFile(&xml);
		return;
	}
	xml.IntoElem();
	xml.AddElem(L"Map");
	xml.AddAttrib(L"Id",m_iCurMapId);
	xml.AddAttrib(L"Name",GetMapName(m_iCurMapId));
	xml.AddAttrib(L"Range",m_iRangePoint);
	xml.IntoElem();
	std::vector<COOR>::iterator coor_item;
	m_iTrainMap=m_iCurMapId;
	for(coor_item=  m_lMonsters.begin(); coor_item != m_lMonsters.end(); coor_item ++){
		xml.AddElem(L"Point");
		xml.AddAttrib(L"x",coor_item->_iX);
		xml.AddAttrib(L"y",coor_item->_iY);
	}
	serialize->SaveFile(&xml);
}


bool CCharacter::LoadSkills(){
	CMarkup xml= XMLSerialized::OpenFile(L"Skill.cfg");
	if(xml.GetDoc().GetLength()==0)
		return false;
	if(xml.FindElem(L"Skills")){
		xml.IntoElem();
		while(xml.FindElem()){
			if(_tstoi(xml.GetAttrib(_T("id")))==m_iFaction){//tìm theo Id của môn phái
				xml.IntoElem();
				while(xml.FindElem()){//Đọc thông tin từng skill
					SKILL_DATA* skill = new SKILL_DATA();
					skill->m_iId = _tstoi(xml.GetAttrib(_T("id")));
					skill->m_iLevel = _tstoi(xml.GetAttrib(_T("level")));
					skill->m_sName = xml.GetAttrib(_T("name"));
					skill->m_iType= _tstoi(xml.GetAttrib(_T("type")));
					CString sDelay = xml.GetAttrib(_T("delay"));
					if(sDelay==L"")
						skill->m_iDelay = 0;
					else
						skill->m_iDelay = _tstoi(sDelay);
					CString sRange = xml.GetAttrib(_T("range"));
					if(sRange==L"")
						skill->m_iRange = 0;
					else
						skill->m_iRange = _tstoi(sRange);

					std::map<CString,SKILL_DATA*>::iterator result = m_mSkills.find(skill->m_sName);
					if(result==m_mSkills.end())//Nếu chưa có thì load skill
						m_mSkills.insert(make_pair(skill->m_sName,skill));
					else{
						if(skill->m_iLevel <= m_iLevel)//load skill theo level
							m_mSkills[skill->m_sName]=skill;
					}

				}
				return true;
			}
		}
	}
	return false;
}

void CCharacter::LoadSkillCfg(){
	CMarkup xml=serialize->OpenFile();
	if(xml.GetDoc().GetLength()==0)
		return;
	if(serialize->FindPath(L"/Info/Skills",&xml)){
		int i=0;
		m_mSkill.clear();
		while(xml.FindElem()){//Đọc thông tin từng skill
			SKILL_CFG* skilCfg = new SKILL_CFG();
			skilCfg->m_iId = _tstoi(xml.GetAttrib(_T("Id")));
			skilCfg->m_iRange = _tstoi(xml.GetAttrib(_T("Range")));
			skilCfg->m_iDelay = _tstoi(xml.GetAttrib(_T("Delay")));
			m_mSkill.insert(make_pair( i,skilCfg));
			i++;
		}
	}
}
void CCharacter::LoadBuffCfg(){
	CMarkup xml=serialize->OpenFile();
	if(xml.GetDoc().GetLength()==0)
		return;
	if(serialize->FindPath(L"/Info/Buffs",&xml)){
		int i=0;
		m_mBuff.clear();
		while(xml.FindElem()){//Đọc thông tin từng skill
			SKILL_CFG* skilCfg = new SKILL_CFG();
			skilCfg->m_iId = _tstoi(xml.GetAttrib(_T("Id")));
			skilCfg->m_iRange = _tstoi(xml.GetAttrib(_T("Range")));
			skilCfg->m_iDelay = _tstoi(xml.GetAttrib(_T("Delay")));
			m_mBuff.insert(make_pair( i,skilCfg));
			i++;
		}
	}
}
bool CCharacter::LoadPoint()
{
	m_lMonsters.clear();
	if(m_iTrainMap==0)
		return false;
	CMarkup xml=serialize->OpenFile();
	if(xml.GetDoc().GetLength()==0)
		return false;
	CString sName = L"Maps";
	xml.FindElem();
	xml.IntoElem();
	if(!xml.FindElem(sName)){
		return false;
	}		
	xml.IntoElem();
	
	while(xml.FindElem(L"Map")){
		if(_ttoi( xml.GetAttrib(L"Id"))== m_iTrainMap){
			m_iRangePoint = _ttoi(xml.GetAttrib(L"Range"));
			xml.IntoElem();
			while(xml.FindElem(L"Point")){
				COOR cCoor;
				cCoor._iX = _ttoi(xml.GetAttrib(L"x"));
				cCoor._iY = _ttoi(xml.GetAttrib(L"y"));
				m_lMonsters.push_back( cCoor);
			}
			return true;
		}
	}
	return false;
}
bool CCharacter::LoadTrain(std::map<int,CString> &lst){
	CMarkup xml= serialize->OpenFile();
	if(xml.GetDoc().GetLength()==0)
		return false;
	CString sName = L"Maps";
	xml.FindElem();
	xml.IntoElem();
	if(!xml.FindElem(sName)){
		return false;
	}		
	xml.IntoElem();
	lst.clear();
	int i=0;
	while(xml.FindElem(L"Map")){
		lst.insert(make_pair(i,xml.GetAttrib(L"Id")));
		i++;
	}
	return true;
}
void CCharacter::LoadElement(){
	m_arrElement[1]=1;
	m_arrElement[2]=2;
	m_arrElement[3]=3;
	m_arrElement[4]=4;
	m_arrElement[5]=5;
}
void CCharacter::LoadFaction(){
	m_arrFaction[1]=1;
	m_arrFaction[2]=2;
	m_arrFaction[3]=3;
	m_arrFaction[4]=4;
	m_arrFaction[5]=5;
	m_arrFaction[6]=6;
	m_arrFaction[7]=7;
	m_arrFaction[8]=8;
	m_arrFaction[9]=9;
	m_arrFaction[10]=10;
}
void CCharacter::LoadAward(){
	m_mAward.insert(make_pair(1,L"Kinh Nghiệm"));
	m_mAward.insert(make_pair(2,L"Vàng Khóa"));
	m_mAward.insert(make_pair(3,L"Bạc Khóa"));
	m_mAward.insert(make_pair(4,L"Cống Hiến Sư Môn"));
	m_mAward.insert(make_pair(5,L"Truyện Ký"));
	m_mAward.insert(make_pair(6,L"Thủy Tinh"));
	m_arrAward[1]=1;
	m_arrAward[2]=2;
	m_arrAward[3]=3;
	m_arrAward[4]=4;
	m_arrAward[5]=5;
	m_arrAward[6]=6;
}
void CCharacter::ConvertMapToScreen(COOR* cCoor){
	int _iXMap,_iYMap;	

	ReadProcessMemory(m_hHandle, (void*)(dwGameBaseAddress+GAME_OFFSET_MAP_X), &_iXMap, (DWORD)sizeof(_iXMap), 0);
	ReadProcessMemory(m_hHandle, (void*)(dwGameBaseAddress+GAME_OFFSET_MAP_Y), &_iYMap, (DWORD)sizeof(_iYMap), 0);
	
	cCoor->_iX=cCoor->_iX <<4;
	cCoor->_iY=cCoor->_iY <<5;

	cCoor->_iX += _iXMap;
	cCoor->_iY += _iYMap;
}
void CCharacter::ConvertScreenToMap(COOR* cCoor){
	int _iXMap,_iYMap;	

	ReadProcessMemory(m_hHandle, (void*)(dwGameBaseAddress+GAME_OFFSET_MAP_X), &_iXMap, (DWORD)sizeof(_iXMap), 0);
	ReadProcessMemory(m_hHandle, (void*)(dwGameBaseAddress+GAME_OFFSET_MAP_Y), &_iYMap, (DWORD)sizeof(_iYMap), 0);
	
	cCoor->_iX -= _iXMap;
	cCoor->_iY -= _iYMap;
	cCoor->_iX=cCoor->_iX >>4;
	cCoor->_iY=cCoor->_iY >>5;	
}


int CCharacter::GetDistance(COOR coor){
	int dX,dY,dZ;	
	dX = coor._iX - m_iTdX;
	dY =  coor._iY - m_iTdY;
	dZ = (int)sqrt((float)dX*dX+(float)dY*dY);
	return dZ;
}
int CCharacter::GetDistance(POS pos){
	int dX,dY,dZ;	
	dX = pos._iX - m_iTdX;
	dY =  pos._iY - m_iTdY;
	dZ = (int)sqrt((float)dX*dX+(float)dY*dY);
	return dZ;
}
void CCharacter::GetCaptain(){
	int iCaptainId=0;
	DWORD dwParty = dwGameBaseAddress + GAME_OFFSET_PARTY;	
	ReadProcessMemory(m_hHandle, (void*)(dwParty), &dwParty, (DWORD)sizeof(dwParty), 0);

	ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_CAPTAIN_ID), &iCaptainId, (DWORD)sizeof(iCaptainId), 0);

	m_cLead.m_iId = 0;
	if(iCaptainId==0)
		return;

	for (int i=0;i<PARTY_SIZE;i++){
		int indx = i * PARTY_DATA_SIZE;
		ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_MEM_ID+indx), &m_cLead.m_iId, (DWORD)sizeof(m_cLead.m_iId), 0);
		if(m_cLead.m_iId==iCaptainId){
			char sValue[NPC_NAME_SIZE];
			memset(sValue, 0x0, sizeof(sValue));
			ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_MEM_NAME+indx), &sValue, (DWORD)sizeof(sValue), 0);
			m_cLead.m_sName = (CString)sValue;			
			//string converStr( );
			wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( m_cLead.m_sName.GetString() ));
			m_cLead.m_sName = wsResult.c_str();

			
			ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_MEM_MAP+indx), &m_cLead.m_iCurMap, (DWORD)sizeof(m_cLead.m_iCurMap), 0);
			if(m_iCurMapId==m_cLead.m_iCurMap){
				ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_MEM_X+indx), &m_cLead.m_iCurX, (DWORD)sizeof(m_cLead.m_iCurX), 0);
				ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_MEM_Y+indx), &m_cLead.m_iCurY, (DWORD)sizeof(m_cLead.m_iCurY), 0);
			}
			return;
		}
	}
}
void CCharacter::Search(){
	int idZ,iTemp;
	CNPC npc(m_hHandle,dwGameBaseAddress);
	mResult.clear();
	for(int i=1;i<NPC_MAX_SIZE;i++){
		if(i==m_iIndex || i == iLag)//bỏ qua npc lag và nhân vật
			continue;

		m_npc->m_iIndex = i;
		m_npc->GetInfo();
		if( m_npc->m_szName==L"")//Không tìm thấy target bỏ qua
			continue;
		if( m_npc->m_iState==15 || m_npc->m_iCurHP<=0)//NPC đã chết
			continue;
		if(m_npc->m_iType == 5)//vòng xoáy
			continue;
		if(m_npc->m_iType == 3)//NPC
			continue;
		


		if(m_npc->m_iType==1){
			if(m_iParty==1 && m_npc->m_iParty==1){//cùng party bỏ qua
				continue;
			}
		}
		if(wcscmp( m_szName,m_szTitle)==0){ //ngoài chiến trường
			if(m_npc->m_iType==1){
				if (m_siPkState==0 || m_npc->m_siPkState==0 )//đang luyện công thì bỏ qua
					continue;				
				if(m_szFamily.GetLength()>0 && m_npc->m_szFamily.GetLength()>0){//nếu có gia tộc					
					CString FaName =m_szFamily.Left(5);
					if(m_npc->m_szFamily.Find(FaName)!=-1 )//Cùng gia tộc bỏ qua
						continue;
				}
				
			}
		}else{//trong chiến trường
			if(m_npc->m_iType==3)//bỏ qua npc
				continue;
			CString sTeam = m_szTitle.Left(5);
			if(m_npc->m_szName.Find(sTeam)!=-1 || m_npc->m_iColor == m_iColor)//Cùng phe bỏ qua
				continue;
		}

		//Tìm theo khoảng cách
		COOR coor;
		coor._iX = m_npc->m_iTdX;
		coor._iY = m_npc->m_iTdY;
		idZ = GetDistance(coor);
		if(idZ> m_iRange)
			continue;


		switch (m_npc->m_iType){
			case 0://Quái (monster)
				if( mResult[L"monster"]==0)//nếu chưa có trong danh sách thì thêm vào
					mResult[L"monster"]=i;
				else{
					npc.m_iIndex = mResult[L"monster"];
					npc.GetInfo();
					coor._iX = npc.m_iTdX;
					coor._iY = npc.m_iTdY;
					iTemp = GetDistance(coor);
					if(idZ<iTemp)
						mResult[L"monster"]=i;
				}
				//Lượng máu
				if( mResult[L"hpMonster"]==0)//nếu chưa có trong danh sách thì thêm vào
					mResult[L"hpMonster"]=i;
				else{
					npc.m_iIndex = mResult[L"hpMonster"];
					npc.GetInfo();
					if(m_npc->m_iCurHP < npc.m_iCurHP)
						mResult[L"hpMonster"]=i;
				}
				break;
			case 1://Người 
				CString element; //Ngũ hành
				element.Format(L"Ele%d",m_npc->m_iElement);
				if( mResult[element]==0)
					mResult[element]=i;
				else{
					npc.m_iIndex = mResult[element];
					npc.GetInfo();
					coor._iX = npc.m_iTdX;
					coor._iY = npc.m_iTdY;
					iTemp = GetDistance(coor);
					if(idZ<iTemp)
						mResult[element]=i;
				}
				CString faction; //Môn phái
				faction.Format(L"Fac%d",m_npc->m_iFaction);
				if( mResult[faction]==0)
					mResult[faction]=i;
				else{
					npc.m_iIndex = mResult[faction];
					npc.GetInfo();
					coor._iX = npc.m_iTdX;
					coor._iY = npc.m_iTdY;
					iTemp = GetDistance(coor);
					if(idZ<iTemp)
						mResult[faction]=i;
				}
				//Lượng máu
				if( mResult[L"hp"]==0)//nếu chưa có trong danh sách thì thêm vào
					mResult[L"hp"]=i;
				else{
					npc.m_iIndex = mResult[L"hp"];
					npc.GetInfo();
					if(m_npc->m_iCurHP < npc.m_iCurHP)
						mResult[L"hp"]=i;
				}
				//Khoang cach
				if( mResult[L"people"]==0)//nếu chưa có trong danh sách thì thêm vào
					mResult[L"people"]=i;
				else{
					npc.m_iIndex = mResult[L"people"];
					npc.GetInfo();
					coor._iX = npc.m_iTdX;
					coor._iY = npc.m_iTdY;
					iTemp = GetDistance(coor);
					if(idZ<iTemp)
						mResult[L"people"]=i;
				}
				break;
		}
		if( mResult[L"distance"]==0)//nếu chưa có trong danh sách thì thêm vào
			mResult[L"distance"]=i;
		else{
			npc.m_iIndex = mResult[L"distance"];
			npc.GetInfo();
			coor._iX = npc.m_iTdX;
			coor._iY = npc.m_iTdY;
			iTemp = GetDistance(coor);
			if(idZ<iTemp)
				mResult[L"distance"]=i;
		}
	}
}

bool CCharacter::SearchNPC(){
	if(!m_siSafe)//nếu trong thành thì ko tìm
		return false;
	//if(bIsRunning)//đang di chuyển đến bãi quái
	//	return false;
/////////Nếu đánh theo đội trưởng và không phải đang làm nv/////////////
	if(m_bPkFollowLead&&m_bFollowLead&&!m_bAutoQuest){
		m_npc->m_iIndex = GetPkTarget();
		if(m_npc->m_iIndex==0)
			return false;
		m_npc->GetInfo();
		if(m_npc->m_iState==15 || m_npc->m_iCurHP<=0)
			m_npc->m_iIndex=0;
		if(m_npc->m_iIndex>NPC_MAX_SIZE)
			m_npc->m_iIndex=0;
		if(m_npc->m_iIndex!=0)
			return true;
		return false;
	}

//////////////Tự tìm đánh///////////////////
	CNPC npc(m_hHandle,dwGameBaseAddress);
	//COOR coor;
	//int iTemp=0;
	
	Search();
	if(mResult.size()==0)
		return false;
	
	//Xet uu tien
	switch(m_iPriority){
		case 0://khoang cach
			if(mResult[L"people"]==0){
				m_npc->m_iIndex = mResult[L"monster"];
				return true;
			}
			if(mResult[L"monster"]==0){
				m_npc->m_iIndex = mResult[L"people"];
				return true;
			}
			if(m_bPkFirst){//Uu tien danh nguoi trc
				m_npc->m_iIndex = mResult[L"people"];
				return true;
			}

			m_npc->m_iIndex = mResult[L"distance"];
			return true;
		case 1://ngu hanh
			for(int i=1;i<=ELEMENT_SIZE;i++){
				CString element;
				element.Format(L"Ele%d",m_arrElement[i]);
				if(mResult[element]>0){
					m_npc->m_iIndex = mResult[element];
					return true;
				}
			}
			m_npc->m_iIndex = mResult[L"monster"];
			return true;
		case 2://mon phai
			for(int i=1;i<=FACTION_SIZE;i++){
				CString faction;
				faction.Format(L"Fac%d",m_arrFaction[i]);
				if(mResult[faction]>0){
					m_npc->m_iIndex = mResult[faction];
					return true;
				}
			}
			m_npc->m_iIndex = mResult[L"monster"];
			return true;
		case 3://luong mau
			if(mResult[L"hp"]==0){
				m_npc->m_iIndex = mResult[L"hpMonster"];
				return true;
			}
			if(mResult[L"hpMonster"]==0){
				m_npc->m_iIndex = mResult[L"hp"];
				return true;
			}
			if(m_bPkFirst){//Uu tien danh nguoi trc
				m_npc->m_iIndex = mResult[L"hp"];
				return true;
			}
			
			m_npc->m_iIndex = mResult[L"hpMonster"];
			npc.m_iIndex =  mResult[L"hp"];
			npc.GetInfo();
			if(m_npc->m_iCurHP > npc.m_iCurHP)
				m_npc->m_iIndex = mResult[L"hp"];
			return true;
	}
	return false;
}

void CCharacter ::DoFight(){
	if(!m_bIsHooking)
		return;
	/*if(!m_bAutoFight && !m_bAutoQuest){
		return;
	}*/
	if(m_siSafe==0) //nếu trong thành thì bỏ qua
		return;
	if(m_iTargetIdx != m_npc->m_iIndex)
		iNpcHP = 0;
	/*m_iTargetIdx=0;
	SetTarget();*/
	
	
	m_npc->GetInfo();
	if(m_npc->m_szName==L"" ){
		return;
	}
	if(m_npc->m_iCurHP<=0 ){
		return;
	}
	

	if(IsLag())
	{
		return;
	}
	int iFight;
	ReadProcessMemory(m_hHandle, (void*)dwDoFight, &iFight, (DWORD)sizeof(iFight), 0);
	if(iFight == 0)
		::PostMessage(m_hwnd, WM_KEYDOWN, 70, 0x0);
	
	//std::map<int,SKILL_CFG*>::iterator skill_item;
	////Kiem tra ket qua tim kiem
	//
	//
	m_iTargetIdx = m_npc->m_iIndex;
	SetTarget();
	//
	//if(m_mSkill.size()==0){
	//	SKILL_DATA* data = m_mSkills.begin()->second;
	//	SetTarget();
	//	SetSkill(data->m_iId);
	//	return;
	//}
	//
	//for(skill_item=  m_mSkill.begin(); skill_item != m_mSkill.end(); skill_item ++){
	//	SKILL_CFG* objskill = skill_item->second;
	//	time_t timer;
	//	time(&timer);
	//	double seconds;
	//	seconds = difftime(timer,m_mSkillDelay[objskill->m_iId]);
	//	if(m_mSkillDelay[objskill->m_iId]==0 || seconds> objskill->m_iDelay){		
	//		if(m_iTargetIdx !=0){//skill pk
	//							
	//			COOR coor;
	//			coor._iX = m_npc->m_iTdX;
	//			coor._iY = m_npc->m_iTdY;
	//			if(objskill->m_iRange < GetDistance(coor)){//Nếu đang đánh thì kiểm tra tầm đánh
	//				if(m_iState!=2){
	//					MoveTo();
	//				}
	//				continue;
	//			}
	//			if(m_bIsRiding==1 && m_bAutoRide ) //đang trên ngựa thì xuong ngua
	//				RideHorse();
	//			SetSkill(objskill->m_iId);
	//			SetTarget();
	//			m_mSkillDelay[objskill->m_iId]=timer;
	//			//if(objskill->m_iDelay >0)
	//			Sleep(800);
	//		}		
	//	}
	//}	/*
	/*CString str;
	str.Format(L"%d",m_iTargetIdx);
		SetWindowTextA(m_hwnd,(LPCSTR)(LPCTSTR) str);*/
}
void CCharacter ::DoBuff(){
	if(!m_bIsHooking)
		return;
	
	if(m_bIsRiding) //nếu cưỡi ngựa thì bỏ qua
		return;
	if(!m_siSafe) //trong thành thì bỏ qua
		return;
	
	if(m_mBuff.size()==0){
		return;
	}
	if(m_iState==7)
		return;

	std::map<int,SKILL_CFG*>::iterator buff_item;
	//Kiem tra ket qua tim kiem
	
	
	for(buff_item=  m_mBuff.begin(); buff_item != m_mBuff.end(); buff_item ++){
		SKILL_CFG* objbuff = buff_item->second;
		time_t timer;
		time(&timer);
		
		double seconds;
		seconds = difftime(timer,m_mSkillDelay[objbuff->m_iId]);
		if(m_mSkillDelay[objbuff->m_iId]==0 || seconds> objbuff->m_iDelay+1){		
			if(DoRightSkill(objbuff->m_iId,m_iTargetIdx,1000))//nếu buff thành công thì gán lại delay
				m_mSkillDelay[objbuff->m_iId]=timer;		
		}
	}	
}
bool CCharacter::IsLag(){
	//m_npc->m_iIndex =m_iTargetIdx;
	if(m_npc==NULL || m_npc->m_iIndex == 0)
		return false;
		
	m_npc->GetInfo();
	if(iNpcHP != m_npc->m_iCurHP){
		iLag = 0;
		iCountTime= COUNT_TIME;
		iNpcHP = m_npc->m_iCurHP;
		return false;
	}
	iCountTime--;
	if(iCountTime<=0){
		iLag = m_npc->m_iIndex;
		m_iTargetIdx=0;
		iNpcHP =0;
		return true;
	}
	return false;
}

//Get Leader's target
int	 CCharacter::GetPkTarget(){
	if(m_cLead.m_iId == 0)
		return 0;
	//Get player Info address
	DWORD dwAddress = dwGameBaseAddress + GAME_OFFSET_INFO;
	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
	
	CNPC npc(m_hHandle,dwGameBaseAddress);
	int index =0; //m_cLead->m_iIndex;
	m_iPkTarget=0;
	for(int i=1;i<NPC_MAX_SIZE;i++){
		if(i==m_iIndex)
			continue;

		npc.m_iIndex = i;
		npc.GetInfo();
		if(npc.m_szName==m_cLead.m_sName){
			index = i;
			break;
		}
	}
	if(index==0)
		return 0;

	dwAddress = dwAddress + GAME_OFFSET_DATASIZE * index;
	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
	
	 //	 Get target PK Index
	dwAddress = dwAddress + NPC_PK_TARGET;
	ReadProcessMemory(m_hHandle, (void*)dwAddress, &m_iPkTarget, (DWORD)sizeof(m_iPkTarget), 0);
	return m_iPkTarget;
}

void CCharacter::RideHorse(){
	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_RIDE,0);
}
void CCharacter::MoveTo(){
	MoveTo(m_npc->m_iTdX,m_npc->m_iTdY);
}
bool CCharacter::MoveTo(int iNpcX,int iNpcY ){
	//20140418--SangDH--Cap nhat ham di chuyen moi
	/*LONG_PTR lparam;
	byte sign = 0;
	iNpcX=iNpcX- m_iTdX;
	iNpcY=iNpcY-m_iTdY;
	if(iNpcX<0 && iNpcY >0)
		sign = 1;
	else if(iNpcX>0 && iNpcY <0)
			sign = 2;
	else if(iNpcX<0 && iNpcY <0)
			sign = 3;*/
	/*lparam = sign * 100000000;
	lparam = lparam + (abs(iNpcX) * 10000);
	lparam = lparam + (abs(iNpcY)/2);
	bool result = (bool)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_MOVE,(LPARAM)lparam);*/

	//////////////////////////////////////////////////////////////
	//20140418--SangDH--Cap nhat ham di chuyen moi
	LONG_PTR lparam;
	lparam = ((iNpcX-0x10)>>5) * 10000;
	lparam = lparam + ((iNpcY-0x10)>>5);
	bool result = (bool)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_MOVE,(LPARAM)lparam);
	return result;
}
void CCharacter::SetTarget(){
	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_FIGHT,m_iTargetIdx);
}
void CCharacter::SetAutoFight(){
	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_AUTOFIGHT,m_bAutoFight);
}

void CCharacter::SetSkill(int iSkillId){	
	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_SKILL,iSkillId);
}
bool CCharacter::DoRightSkill(int iSkillId,int iTarget,int delay){
	//m_iTargetIdx=0;
	//SetTarget();
	//SetSkill(iSkillId);
	int iPar = iSkillId  + (10000 * iTarget);
	bool bResult = ::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_RIGHT_SKILL,iPar);
	Sleep(delay);
	return bResult;
}
bool CCharacter::GoingtoLead(){
	//từ bỏ mục tiêu
	m_iTargetIdx=0;
	SetTarget();
	//Lấy thông tin đội trưởng
	GetCaptain();
	if(m_cLead.m_iId ==0)
		return false;
	if(m_cLead.m_iCurMap != m_iCurMapId){//không cùng map với captain
		if(m_bAutoGoto)
			return FindingAWay(m_cLead.m_iCurMap,0,0);
	}
	else{
		bIsRunning=false;
		COOR coor;
		coor._iX =m_cLead.m_iCurX;
		coor._iY = m_cLead.m_iCurY;
		if(GetDistance(coor)<m_iDistance)
			return false;
		if(!m_bIsRiding && m_bAutoRide)
			RideHorse();
		MoveTo(coor._iX,coor._iY);
	}

	return false;
}
bool CCharacter::GoingToTrain(){
	//từ bỏ mục tiêu
	m_iTargetIdx=0;
	SetTarget();
	
	return FindingAWay(m_iTrainMap,0,0);
}
bool CCharacter::CheckSupport(){
	if(!m_bIsHooking)
		return false;
	if(m_siSafe==0) //nếu trong thành thì bỏ qua
		return false;
	if((m_iCurHP*100)/m_iMaxHP < m_iHpLimit){
		KeyPress(m_iKeyHP+48);
	}
	if(m_bAutoEat)
		if((m_iCurMP*100)/m_iMaxMP < 20){
			//KeyPress(m_iKeyMP+48);
			AutoEat();
		}
	return true;
}
void CCharacter::KeyPress(int iKey){
	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_KEYDOWN,iKey);
}

void CCharacter::AutoChat(){
	iDelayChat--;
	if(iDelayChat<0)
		iDelayChat=CHAT_TIME;
	if(CHAT_TIME==0 || iDelayChat!= CHAT_TIME)
		return;
	int rnd = rand()% MSG_SIZE;
	char* szChatMsg = chatMsg[rnd];
    ::PostMessage(m_hwnd, WM_KEYDOWN, VK_RETURN, 0x001C0001);
    ::PostMessage(m_hwnd, WM_KEYDOWN, VK_DOWN, 0x00500001);
    while (szChatMsg[0])
    {
        ::PostMessage(m_hwnd, WM_CHAR, LOBYTE(szChatMsg[0]), 0);
        szChatMsg++;
    }
    ::PostMessage(m_hwnd, WM_KEYDOWN, VK_RETURN, 0x001C0001);
}
void CCharacter::ShowMessage(CString sMsg){
	wchar_t* szChatMsg=sMsg.GetBuffer(sMsg.GetLength());
	sMsg.ReleaseBuffer();

    ::PostMessage(m_hwnd, WM_KEYDOWN, VK_RETURN, 0x001C0001);
    ::PostMessage(m_hwnd, WM_KEYDOWN, VK_DOWN, 0x00500001);
    while (szChatMsg[0])
    {
        ::PostMessage(m_hwnd, WM_CHAR, LOBYTE(szChatMsg[0]), 0);
        szChatMsg++;
    }
    ::PostMessage(m_hwnd, WM_KEYDOWN, VK_RETURN, 0x001C0001);
}


void CCharacter::BackToCity(){
	if(IsInterval(3,1))
		return;
	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_BACK_CITY,0);
	//Sleep(1000);
	Reset();
}
void CCharacter::AutoEat(){
	//Bánh Ngô, Màn Thầu, Bánh Hấp, Bánh Bí Ngô, Bánh Bao
	
	CString sName =(CString) MpFood[m_iKeyMP];

	int item = GetItemInPack(sName);
	if(item ==-1 )
		return;	
	bool result = ::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_ITEM_CLICK,item);

}
void  CCharacter::Reset(){
	bIsRunning=false;
	m_iCurQuest=0;
	m_iCurMapId=0;
}
bool CCharacter::MovingToTrainPoint(bool bNext){
	if(m_lMonsters.size()==0){
		bIsRunning=false;
		return false;
	}
	if(bNext)
		siCurMoster++;
	if(siCurMoster >= m_lMonsters.size())
		siCurMoster = 0;
	if(m_iTrainMap==m_iCurMapId)
		return GoingToPoint();
	/*COOR cor = m_lMonsters[siCurMoster];
	cTarget._iX = cor._iX;
	cTarget._iY = cor._iY;*/
	if(m_bAutoBackToTrain)
		FindingAWay(m_iTrainMap,0,0);
	return true;	
}
bool CCharacter::GoingToPoint(){
	//if(m_lMonsters.size() ==0)
	//	return false;
	//
	//if(!bIsRunning){
	int iCur = siCurMoster;
	
	COOR cCoor = m_lMonsters[iCur];
	if(GetDistance(cCoor)>m_iRangePoint || bIsRunning){
		cTarget._iX = cCoor._iX;
		cTarget._iY = cCoor._iY;
		//siCurMoster=iCur;
		//MovingToTarget();
		return MovingToTarget();
	}
	//}
	return false;
}

bool CCharacter::FindingAWay(int iDes,int iX,int iY,bool clickLink){

	//tự tìm đường trong cùng map
	if(m_iCurMapId == iDes){
		
		bIsRunning =false;
		cTarget._iX =iX;
		cTarget._iY =iY;
		cTarget._iMapId = iDes;
		if(cTarget._iX!=0&& cTarget._iY!=0 &&cTarget._iY<10000){
			COOR cCor;
			cCor._iX = iX;
			cCor._iY = iY;
			ConvertMapToScreen(&cCor);	
			cTarget._iX =cCor._iX;
			cTarget._iY =cCor._iY;
		}
		if(GetDistance(cTarget)<=50){
			bIsRunning = false;
			cTarget._iX =0;
			cTarget._iY =0;
			return false;
		}
		if(IsInterval(2,1))
			return true;		
		return MovingToTarget();
	}	

	//Set time delay
	
	if(m_iState==2 ){	
		return true;
	}

	/*time_t timer;
	time(&timer);
	double seconds;
	seconds = difftime(timer,tMoveDelay);
	if(seconds<3 && tMoveDelay>0)
		return true;
	time(&tMoveDelay);*/

	if(IsInterval(1,3))
		return true;
	
	if(clickLink){//nếu đang làm nhiệm vụ thì click link để di chuyển
		if(GetDistance(cTarget)<50){
			bIsRunning = false;
			return false;
		}
		bIsRunning = true;
		ClickOnLink();	
		return true;
	}
	CMarkup xmlMap;
	xmlMap = XMLSerialized::OpenFile(L"Map.cfg");
	if(xmlMap.GetDoc().GetLength()==0)
		return false;
    xmlMap.ResetPos();
	if (!xmlMap.FindElem(_T("Maps")) )
        return false;
	
	if(xmlMap.IntoElem()){
		while(xmlMap.FindElem()){
			if(_tstoi(xmlMap.GetAttrib(_T("id")))==iDes){
				int mapIndx = _tstoi(xmlMap.GetAttrib(_T("Index")));
				//time(&tMoveDelay);
				/*::PostMessage(m_hwnd, WM_KEYDOWN, VK_TAB, 0xc00f0001);
				Sleep(100);	*/			
				::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_MOVE_AWAY,mapIndx);
				//::PostMessage(m_hwnd, WM_KEYDOWN, VK_TAB, 0xc00f0001);
				bIsRunning = true;
				return true;
			}
		}
	}
	
	return false;
}

bool CCharacter::MovingToTarget(){
	//từ bỏ mục tiêu
	m_iTargetIdx=0;
	SetTarget();
	if(cTarget._iX ==0 && cTarget._iY ==0)
		return false;
	if(GetDistance(cTarget)<=50){
		bIsRunning = false;
		cTarget._iX =0;
		cTarget._iY =0;
		return false;
	}
	bIsRunning = true;
	////len ngua
	if(!m_bIsRiding && m_bAutoRide)
		RideHorse();
//	if(m_iState!=2 )
	return 	MoveTo(cTarget._iX,cTarget._iY);
}


//Thuc hien nhiem vu lich luyen
bool CCharacter::ChooseMenu(int iMenuIndex){
	Sleep(500);
	bool LResult= (bool)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_MENU_CLICK,iMenuIndex);
	return LResult;
}
//Chọn phần thưởng nhiệm vụ lịch luyện
bool CCharacter::ChooseAward(){
	Sleep(200);
	int award=1;
	for(int i=1;i<=m_arrAward[0];i++){
		award=GetAwardIndex(m_mAward[m_arrAward[i]]);
		if(award!=0)
			break;
	}
	if(award==0)
		award = 1;	
	bool LResult= (bool)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_AWARD_CLICK,award);
	
	return LResult;
}
int CCharacter::GetAwardIndex(CString szTitle){
	DWORD dwAddress ;
	CString szQuestText;
	DWORD m_dwAwardTextAddress;
	char sValue[300];
	//if(m_dwAwardTextAddress==NULL){
		dwAddress = dwGameBaseAddress + QUEST_AWARD_BASE_ADDRESS;
		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		for(int i=0;i<12;i++){
			if(i==0 || i==6|| i==7|| i==11)
				dwAddress = dwAddress + GAME_MENU_OFFSET_2;
			else
				dwAddress = dwAddress + GAME_MENU_OFFSET_1;
			ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);

		}		
		m_dwAwardTextAddress = dwAddress;		
	//}
			
	memset(sValue, 0x0, sizeof(sValue));

	for(int i=0;i<3;i++){
		if (m_iOS==WIN_XP_64 || m_iOS==WIN_7_64){
			if(i==0)
				dwAddress = m_dwAwardTextAddress +QUEST_AWARD_TEXT_1; 
			if(i==1)
				dwAddress = m_dwAwardTextAddress +QUEST_AWARD_TEXT_2; 
			if(i==2)
				dwAddress = m_dwAwardTextAddress +QUEST_AWARD_TEXT_3; 

			ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
			if(i>0)
				dwAddress = dwAddress +QUEST_AWARD_TEXT_SIZE_XP; 
		}
		else{
			dwAddress = m_dwAwardTextAddress +0x840; 
			ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
			dwAddress=dwAddress+ (i*QUEST_AWARD_TEXT_SIZE);		
		}


		ReadProcessMemory(m_hHandle, (void*)dwAddress, &sValue, (DWORD)sizeof(sValue), 0);
		szQuestText  = (CString)sValue;
		
		//string converStr( );
		wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( szQuestText.GetString() ));
		szQuestText = wsResult.c_str();

		if(szQuestText.MakeLower().Find(szTitle.MakeLower())>-1)
			return i+1;
	}
	return 0;

}
bool CCharacter::ClickOnLink(){
	int iIndex = -1;
	if(m_iQuesting == 2)
		iIndex = GetQuestIndex(L"<pic=58><color=yellow>Vận Tiêu");
	else if(m_iQuesting == 3)	
		iIndex = GetQuestIndex(L"<pic=60><color=yellow>Lịch Luyện");
	
	if(iIndex==-1)
		return false;
	
	bool LResult= (bool)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_QUEST_LINK_CLICK,iIndex);
	return LResult;
}
bool CCharacter::GetLinkPos(CString szLinkText){
	//<link=pos:Mã Hoành,69,52429,103513>
	//phân loại nhiệm vụ
	//======nếu nv lịch luyện
	if(m_iQuesting == 2)
	{
		m_iProsessType = 2;		//Dang Hộ tống
		if(szLinkText.Find(L"Về thành tìm Tổng Tiêu Đầu")!=-1)
			m_iProsessType = 3;		//Hộ tống hoàn thành
		else if(szLinkText.Find(L"1/")!=-1)
			m_iProsessType = 1;		//Toi cho nguoi uy thac nhận tiêu
	}
	else if(m_iQuesting == 3)
	{
		m_iQuestType = 3;
		if(szLinkText.Find(L"lời nhắn")!=-1)
			m_iQuestType = 1;
		else if(szLinkText.Find(L"dùng")!=-1)
			m_iQuestType = 2;		
	}
	int iStartIndx = szLinkText.Find(L"link=pos:");
	if(iStartIndx==-1)
		return false;
	iStartIndx+=9;
	
	int iEndIndx;
	iEndIndx =  szLinkText.Find(L",",iStartIndx);
	m_szQuestTarget = szLinkText.Mid(iStartIndx,iEndIndx-iStartIndx);
	iStartIndx = iEndIndx+1;

	iEndIndx =  szLinkText.Find(L",",iStartIndx);
	cTarget._iMapId =_ttoi( szLinkText.Mid(iStartIndx,iEndIndx-iStartIndx));
	iStartIndx = iEndIndx+1;

	iEndIndx =  szLinkText.Find(L",",iStartIndx);
	cTarget._iX =_ttoi( szLinkText.Mid(iStartIndx,iEndIndx-iStartIndx));
	iStartIndx = iEndIndx+1;

	iEndIndx =  szLinkText.Find(L">",iStartIndx);
	cTarget._iY =_ttoi( szLinkText.Mid(iStartIndx,iEndIndx-iStartIndx));
	iStartIndx = iEndIndx+1;
//	if(m_iQuestType !=3)
		//FindingAWay(cTarget._iMapId,cTarget._iX,cTarget._iY);
	return true;
	//int LResult= (int)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_LINK_CLICK,iBlockIndex);
	//return LResult!=0;

}
int	CCharacter::AskNPC(CString szName,bool clickLink){
	if(!clickLink){
		POS* pos;	
		pos = GetNPC(m_iCurMapId,szName);
		if(pos!=NULL){
			cTarget._iX = pos->_iX;
			cTarget._iY = pos->_iY;
			cTarget._iMapId= m_iCurMapId;
		}else{
			pos = GetNPC(179,szName);
			if(pos==NULL){
				return -1;
			}
			cTarget._iX = pos->_iX;
			cTarget._iY = pos->_iY;
			cTarget._iMapId= pos->_iMapId;
		}
	}
	//Dung Hoi thanh thach	
	if(m_bQuickAward){//){//có dùng hồi thành thạch
		if(OpenGoBackRock()){	//Nếu có hồi thành thạch
			if(ChooseMenu(100)){
				ChooseMenu(100);
				Sleep(10000);
			}	
			return -1;
		}			
	}
	if( FindingAWay(cTarget._iMapId,cTarget._iX,cTarget._iY,clickLink)){//chạy tới vị trí NPC
		return -1;
	}
//đã chạy tới NPC, tìm NPC
	CNPC npc(m_hHandle,dwGameBaseAddress);
	for(int i=1;i<NPC_MAX_SIZE;i++){
		if(i==m_iIndex)//bỏ qua nhân vật
			continue;
		npc.m_iIndex = i;
		npc.GetInfo();
		if( npc.m_szName==L"")//Không tìm thấy target bỏ qua
			continue;
		if( npc.m_iType!=3)//không phải NPC bỏ qua
			continue;
		if(npc.m_szName==szName){//lấy NPC
			
			bIsRunning=false;
			int LResult= (int)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_OPEN_MENU,0);
			if(LResult==0){
				m_iTargetIdx=i;
				SetTarget();
			}
			return LResult;
		}
	}
	return -1;
}
bool	CCharacter::FightMonster(CString szText){
	GetLinkPos(szText);
	if(GetDistance(cTarget)>1000){
		FindingAWay(cTarget._iMapId, cTarget._iX ,cTarget._iY,true);
		return false;
	}
	int iIndex = SearchNPCByName(m_szQuestTarget);
	if(iIndex>0){
		//m_iIndex
		m_npc->m_iIndex=iIndex;
		DoFight();
	}
	return true;
}
CString CCharacter::GetQuestText(int iIndex){
	if(iIndex>4)
		return L"";
	DWORD dwAddress ;
	CString szQuestText=L"";
	//CString szTitle = L"Giang Hồ Lịch Luyện";
	char sValue[300];
//	if(m_dwQuestTextAddress==NULL){
		dwAddress = dwGameBaseAddress + QUEST_BASE_ADDRESS;
		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		for(int i=0;i<9;i++){
			dwAddress = dwAddress + GAME_MENU_OFFSET_1;
			ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		}
		dwAddress = dwAddress + GAME_MENU_OFFSET_2;
		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		dwAddress = dwAddress + GAME_MENU_OFFSET_1;
		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		dwAddress = dwAddress + GAME_MENU_OFFSET_1;
		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		dwAddress = dwAddress + GAME_MENU_OFFSET_2;
		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		/////////////////////////////20150512_thay đổi địa chỉ block text dùng chung win7 và xp////////////////////////////
		for(int i=0;i<iIndex;i++){
			dwAddress = dwAddress + GAME_MENU_OFFSET_1;
			ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		}
		//if (m_iOS!=5164 && m_iOS!=5264 && iIndex>=2){//không phải winxp 64bit và là block 3 & 4 & 5
		//	dwAddress = dwAddress + GAME_MENU_OFFSET_1;
		//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		//	dwAddress = dwAddress + GAME_MENU_OFFSET_1;
		//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		//	if(iIndex>=3){ //block  4 & 5
		//		dwAddress = dwAddress + GAME_MENU_OFFSET_1;
		//		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
		//	}
		//}
		dwAddress = dwAddress + GAME_MENU_OFFSET_2;
		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
//		m_dwQuestTextAddress = dwAddress;		
//	}
		

	
	/////////////////////////////20150512_thay đổi địa chỉ block text dùng chung win7 và xp////////////////////////////
	//if (m_iOS==5164 || m_iOS==5264){//win XP64
	//	dwAddress = m_dwQuestTextAddress -0x30;
	//	if(iIndex!=1)
	//		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
	//	switch(iIndex){
	//		case 0:
	//			 dwAddress = dwAddress+ QUEST_TEXT_0;
	//			break;
	//		case 1:
	//			dwAddress = dwAddress + QUEST_TEXT_1;
	//			break;
	//		case 2:
	//			dwAddress = dwAddress + QUEST_TEXT_2;
	//			break;
	//		case 3:
	//			dwAddress = dwAddress + QUEST_TEXT_3;
	//			break;
	//		case 4:
	//			dwAddress = dwAddress + QUEST_TEXT_4;
	//			break;
	//	}
	//}
	//else{
	//	//dwAddress = m_dwQuestTextAddress -0x30 + (iIndex*QUEST_DATA_SIZE);
	//	dwAddress = m_dwQuestTextAddress -0x30;
	//	switch(iIndex){
	//		case 2:
	//		case 3:
	//			dwAddress = m_dwQuestTextAddress -0x30;
	//			break;
	//		case 4:
	//			dwAddress = dwAddress + QUEST_TEXT_1;
	//			break;
	//		default:
	//			dwAddress = m_dwQuestTextAddress -0x30 + (iIndex*QUEST_DATA_SIZE);
	//			break;
	//	}
	//}

	dwAddress = dwAddress + QUEST_TEXT_OFFSET;
	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);

	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(m_hHandle, (void*)dwAddress, &sValue, (DWORD)sizeof(sValue), 0);
	szQuestText  = (CString)sValue;
	
	//string converStr( );
	wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( szQuestText.GetString() ));
	szQuestText = wsResult.c_str();
	return szQuestText;
}
CString CCharacter::GetQuestText(CString szTitle){
	CString szQuestText;
	for(int i=0;i<5;i++){
		szQuestText = GetQuestText(i);
		
		if(szQuestText.Find(szTitle)>-1)
			return szQuestText;
	}
	return L"";
}
int CCharacter::GetQuestIndex(CString szTitle){
	CString szQuestText;
	for(int i=0;i<5;i++){
		szQuestText = GetQuestText(i);
		if(szQuestText.Find(szTitle)>-1)
			return i;
	}
	return -1;
}
bool CCharacter::UseQuestItem(){

	int iIndx = GetQuestIndex(L"<pic=60><color=yellow>Lịch Luyện");
	if(iIndx==-1)
		return false;
	int LResult= (int)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_QUEST_ITEM_CLICK,iIndx);
	return LResult!=0;
}
//Nhan nhiem vu
bool	CCharacter::ReceiveQuest(){
	if(m_iCurQuest<0)
		return false;
	if(m_iCurQuest!=0 && m_iCurQuest!=6)//Đã nhận nhiệm vụ, đang thực hiện
		return true;
	CString str=L"<pic=60><color=yellow>Lịch Luyện";
	if(m_iCurQuest==0){//nhận lần đầu tiên
		CString strText = GetQuestText(str);
		str = strText.Mid(str.GetLength(),6);
		if(strText!=L""){//nếu đã có nhiệm vụ			
			if(strText.Find(L"Nhiệm vụ đã xong")==-1){//chưa hoàn thành nhiệm vụ đã nhận
				ShowMessage(L"Dang thuc hien nhiem vu lich luyen" + str);
				m_iCurQuest=1;
				return true;
			}
			/*else{
				ShowMessage(L"Nhan thuong nhiem vu lich luyen " + str);
				m_iCurQuest=4;
				return true;
			}*/
		}
	}
	int menu =-1;

	//nếu làm nhiệm vụ trong thiêm tâm cốc
	if(m_bQuestInCave && m_iCurMapId!=285){
		menu = AskNPC(L"Lâm Thiên Tâm");
		if(menu!=1){
			return false;
		}
		if(!ChooseMenu(1)){//vào thiên tâm cốc
			return false;
		}
		//ShowMessage(L"Vao Thien Tam Coc");
	}


	//Nếu chưa nhận nhiệm vụ
	menu = AskNPC(L"Tâm Hà");
	if(menu==-1){//Chưa tìm dc NPC
		return false;
	}
	if(menu==2){//Nếu nhận thưởng
		ShowMessage(L"Quay ve tra nhiem vu nhan thuong");
		m_iCurQuest=4;
		return true;
	}
	if(menu==1){//nếu nhiệm vụ
		if(ChooseMenu(2)){//nhận nhiệm vụ
			m_iQuestNum++;
			m_iCurQuest=1;
			return true;
		}
	}
	return false;
}

bool CCharacter::ProcessQuest(){
	CString szQuestText;
	CString str=L"<pic=60><color=yellow>Lịch Luyện";
	szQuestText = GetQuestText(str);
	int menu;
	if(szQuestText==L""){//Chưa nhận được nhiệm vụ thì nhận lại
		ShowMessage(L"Quay ve nhan nhiem vu");
		m_iCurQuest = 0;
		return false;
	}
	if(m_iCurQuest == 4 || szQuestText.Find(L"Nhiệm vụ đã xong")!=-1){//Nếu đã hoàn thành nhiệm vụ thì nhận thưởng
		m_iCurQuest = 4;
		return ReceiveAward(szQuestText);
	}
	GetLinkPos(szQuestText);
	switch(m_iCurQuest){
		case 1:
			
			if(m_iQuestType == 3){//&&m_iCurMapId!= cTarget._iMapId){//Nhiệm vụ đánh quái
				//nếu làm nhiệm vụ trong thiêm tâm cốc
				
				if(m_bQuestInCave && m_iCurMapId!=285&& m_iCurMapId!=286&& m_iCurMapId!=287){
					menu = AskNPC(L"Lâm Thiên Tâm");
					if(menu!=1){
						return false;
					}
					if(!ChooseMenu(1)){//vào thiên tâm cốc
						return false;
					}
					ShowMessage(L"Vao Thien Tam Coc");
					return false;
				}
				//tìm NPC đưa đến bãi quái
				if(m_iCurMapId != cTarget._iMapId){
					int iIdx = 0;
					if(m_iCurMapId==285)
						iIdx = 1;
					menu = AskNPC(L"Tâm Hà");
					if(menu!=1 || !ChooseMenu(iIdx)){//Chọn quay lại bãi quái
						m_iCurQuest = 1;
						return false;
					}
				}
				
			}	
			str = szQuestText.Mid(str.GetLength(),6);
			ShowMessage(L"Bat dau thuc hien nhiem vu " + str);
			m_iCurQuest = 2;
			return false;
		case 2://di chuyen bằng cách click link
			if (!FindingAWay(cTarget._iMapId, cTarget._iX ,cTarget._iY,true))
				m_iCurQuest = 3;
			break;
		case 3://xử lý
			switch(m_iQuestType){
				case 1:
					if(AskNPC(m_szQuestTarget,true)==1){
						if(ChooseMenu(0))//đối thoại
							if(ChooseMenu(0)==false){//chưa đóng được menu
								m_iCurQuest = 3;
								return false;
							}
					}
					break;
				case 2:
					UseQuestItem();
					Sleep(5000);
					break;
				case 3:
					FightMonster(szQuestText);
					break;
			}
			break;
	}
	return false;
}

bool	CCharacter::ReceiveAward(CString szQuestText){
	GetLinkPos(szQuestText);
	//if (!FindingAWay(cTarget._iMapId, cTarget._iX ,cTarget._iY,true)){
//		ShowMessage(L"Nhan thuong nhiem vu lich luyen");
	if(AskNPC(m_szQuestTarget,true)==2){
		if(ChooseAward()){
			m_iCurQuest =6;				
			return true;
		}
		m_iCurQuest =4;				
		return false;
	}
	//}
	m_iCurQuest =0;
	return false;
}

//sangdh-20140601-kiểm tra khi bị đánh thì không dùng hồi thành thạch
bool	CCharacter::OpenGoBackRock(){
	if( m_siSafe==0 || m_iState>5 || m_iCurMapId==cTarget._iMapId ||m_iCurMapId== 286||m_iCurMapId== 287 ){//trong thành, cùng map ,đang đánh hoặc chết, đang trong thiên tâm tháp
		return false;
	}
	//đang đánh quái thì chạy 3s mới dùng hồi thành thạch	
	if(m_iQuestType==3){
		if(GetHitMeTarget()!=0)
			return false;
		else
			MoveTo(m_iTdX,m_iTdY);
	}
	
	//kiểm tra hồi thành thạch trong rương
	int item = GetItemInPack(L"Hồi Thành Thạch");
	if(item ==-1 )
		return false;
	
	int LResult= (int)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_ITEM_CLICK,item);
	return LResult!=0;
}


int	CCharacter::GetItemInPack(CString sName){
	DWORD dwbase  = dwGameBaseAddress + PACKAGE_CELL_ADDRESS;	
	ReadProcessMemory(m_hHandle, (void*)dwbase, &dwbase, (DWORD)sizeof(dwbase), 0);
	dwbase = dwbase + PACKAGE_OFFSET_1;	
	ReadProcessMemory(m_hHandle, (void*)dwbase, &dwbase, (DWORD)sizeof(dwbase), 0);
	DWORD dwTempAdd;
	int itemPos=-1;
	for(int i=0;i<100;i++){
		//moi 16 o tiep theo sau 36 o dau tien
		if(i<36){
			dwTempAdd =  dwbase + PACKAGE_OFFSET_2;	
			ReadProcessMemory(m_hHandle, (void*)dwTempAdd, &dwTempAdd, (DWORD)sizeof(dwTempAdd), 0);
			if(dwTempAdd==NULL)
				return -1;
			dwTempAdd = dwTempAdd + i*4;
		}
		else{
			dwTempAdd =  dwbase + PACKAGE_OFFSET_3 + ((int)((i-36)/16)*0x18);	
			ReadProcessMemory(m_hHandle, (void*)dwTempAdd, &dwTempAdd, (DWORD)sizeof(dwTempAdd), 0);
			if(dwTempAdd==NULL)
				return -1;
			dwTempAdd = dwTempAdd + ((i-36)%16)*4;
		}
		ReadProcessMemory(m_hHandle, (void*)dwTempAdd, &itemPos, (DWORD)sizeof(itemPos), 0);

		if(GetItemNameByIndex(itemPos).Find(sName)!=-1){
			return i;
		}
	}	
	return -1;
}

CString CCharacter::GetItemNameByIndex(int iIndex){
	//Get item index
	CString sItemName;
	DWORD dwbase = dwGameBaseAddress + ITEM_OFFSET_ADDRESS;	
	ReadProcessMemory(m_hHandle, (void*)dwbase, &dwbase, (DWORD)sizeof(dwbase), 0);
	dwbase = dwbase + ITEM_OFFSET_ADDRESS_1;	
	ReadProcessMemory(m_hHandle, (void*)dwbase, &dwbase, (DWORD)sizeof(dwbase), 0);

		char sValue[NPC_NAME_SIZE];
		//char sMember[SIZE_TEXT];
		memset(sValue, 0x0, sizeof(sValue));
		//memset(sMember, 0x0, sizeof(sMember));
		dwbase = dwbase + ITEM_NAME_OFFSET+((iIndex-1)* ITEM_DATA_SIZE);
		ReadProcessMemory(m_hHandle, (void*)dwbase, &sValue, (DWORD)sizeof(sValue), 0);

		sItemName = (CString)sValue;
		wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( sItemName.GetString() ));
		sItemName = wsResult.c_str();

	return sItemName;
}

////==================NghiaLP-20140401 tab bãi quái===========================
int CCharacter::GetNumberBag()
{
	DWORD dwbase = dwGameBaseAddress + OFFSET_PICK_BAG;	
	ReadProcessMemory(m_hHandle, (void*)dwbase, &dwbase, (DWORD)sizeof(dwbase), 0);

	return dwbase;
}
bool CCharacter::PickBag()
{
	if(m_bPickBag == false)
		return false;
	int iNumBag = GetNumberBag();
	if(iNumBag != 0)
	{
		::PostMessage(m_hwnd, WM_KEYDOWN, VK_SPACE, 0x00000000);
		if(iNumBag == 1)
			IsInterval(7,4);
	//	iNumBag = GetNumberBag();
		return true;
	}
	return false;
}

//===========================tab tổ đội==========================
void CCharacter::InterParty()
{
	if(!m_bInterParty )    //không đồng ý don xin to doi
		return;
		
	if(m_iParty==1)//nếu đã có party
		return;

	if(IsInterval(5,5))
		return;
	//if(dwCheckInviteParty == NULL)
	//	GetCheckInviteParty();  //lấy địa chỉ checkbox đồng ý
	//if(dwCheckInviteParty == NULL)
	//	return;
	//if(m_szNameParty == L"") {//Nếu đồng ý tất cả party
	//	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_CHECK_AUTO_PARTY,1);
	//}
	//else{
	//	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_CHECK_AUTO_PARTY,0);
	CString sName =GetNameParty();
	if(sName==L"")//không có lời mời
		return;
	if(m_szNameParty.MakeLower() == sName.MakeLower() || m_szNameParty == L"") //đúng người mời
	{
		::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_AGREE_PARTY,1);
		return;
	}
	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_AGREE_PARTY,0);
	//}	 
}

CString CCharacter::GetNameParty()
{
	DWORD base = dwGameBaseAddress + OFSET_OPEN_DIALOG_PARTY;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	if(base == NULL)
		return false;
	for(int i = 0; i<12; i++)
	{
		base = base + GAME_MENU_OFFSET_1;
		ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
		if(base == 0)
			return false;
	}
	DWORD dwPlayerName = base;
	base = base + OPEN_DIALOG_PARTY;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	byte bol = byte(base);
	//Kiểm tra hộp thoại mở hay đóng
	if (bol!= 162)
		return L"";

	//lấy tên người mời
	dwPlayerName = dwPlayerName + GAME_MENU_OFFSET_2;	
	ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &dwPlayerName, (DWORD)sizeof(dwPlayerName), 0);
	dwPlayerName = dwPlayerName + OFFSET_NAME_PARTY;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &dwPlayerName, (DWORD)sizeof(dwPlayerName), 0);

	char sValue[NPC_NAME_SIZE];
	//char sMember[SIZE_TEXT];
	memset(sValue, 0x0, sizeof(sValue));
	//memset(sMember, 0x0, sizeof(sMember));
	ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &sValue, (DWORD)sizeof(sValue), 0);
	
	//TachChuoi(sMember,sValue,'>','<');
		
	CString m_sName = (CString)sValue;
	int iStartIndx = m_sName.Find(L">")+1;
	int iEndIndx =  m_sName.Find(L"<",iStartIndx);
	m_sName=m_sName.Mid(iStartIndx,iEndIndx-iStartIndx);
	
	wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( m_sName.GetString() ));
	m_sName = wsResult.c_str();
	


	return m_sName;

}


void CCharacter::CreateParty()
{
	if(!m_bCreateParty )    //không đồng ý don xin to doi
		return;
	if(GetPartySize()>=6){
		return;
	}
	if(IsInterval(4,5))
		return;
	InviteParty();
	//dong y loi moi theo danh sach


}

void CCharacter::InviteParty()
{
			char sValue[NPC_NAME_SIZE];
			DWORD dwbaseMem = dwGameBaseAddress + OFSET_OPEN_DIALOG_PARTY;	
			ReadProcessMemory(m_hHandle, (void*)dwbaseMem, &dwbaseMem, (DWORD)sizeof(dwbaseMem), 0);
			
			dwbaseMem = dwbaseMem + GAME_MENU_OFFSET_1;	
			ReadProcessMemory(m_hHandle, (void*)dwbaseMem, &dwbaseMem, (DWORD)sizeof(dwbaseMem), 0);
			dwbaseMem = dwbaseMem + GAME_MENU_OFFSET_1;	
			ReadProcessMemory(m_hHandle, (void*)dwbaseMem, &dwbaseMem, (DWORD)sizeof(dwbaseMem), 0);

			dwbaseMem = dwbaseMem + OFFSET_SAVE_NAME;
			//memset(sValue, 0x0, sizeof(sValue));
			//strcpy(sValue,">:Hiáº¿pMi:<");
			if(sizeof(m_vMemberParty) == 0)
				return;
			std::vector<CString>::iterator name_member;
			//INV_PARTY* objmem;
			for(name_member = m_vMemberParty.begin(); name_member != m_vMemberParty.end(); name_member++ )
			{
				
				wstring str = (wstring)name_member->Trim();
				string sResult =XMLSerialized::UnicodeToUTF8(str);
				if(IsMember((CString)sResult.c_str()))//đã là thành viên
					continue;
				for(int i = 0; i < sResult.length() ; i++)
				{
					sValue[i] = (char)(LPCTSTR)sResult[i];
				}
				sValue[sResult.length()] ='\0';
				//DWORD base = OFFSET_SAVE_NAME;
				WriteProcessMemory(m_hHandle, (LPVOID*)dwbaseMem, &sValue, sizeof(sValue), 0);
				::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_INVITE_LIST,dwbaseMem);
			}
}
int CCharacter::GetPartySize(){
	int iCaptainId=0;
	DWORD dwParty = dwGameBaseAddress + GAME_OFFSET_PARTY;	
	ReadProcessMemory(m_hHandle, (void*)(dwParty), &dwParty, (DWORD)sizeof(dwParty), 0);

	ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_CAPTAIN_ID), &iCaptainId, (DWORD)sizeof(iCaptainId), 0);

	if(iCaptainId==0)//Không có đội trưởng
		return 0;
	int iSize=0;
	for (int i=0;i<PARTY_SIZE;i++){
		int indx = i * PARTY_DATA_SIZE;
		int iMemId;
		ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_MEM_ID+indx), &iMemId, (DWORD)sizeof(iMemId), 0);
		if(iMemId!=0)
			iSize++;
	}
	return iSize;
}
bool  CCharacter::IsMember(CString sName)
{
	int iCaptainId=0;
	DWORD dwParty = dwGameBaseAddress + GAME_OFFSET_PARTY;	
	ReadProcessMemory(m_hHandle, (void*)(dwParty), &dwParty, (DWORD)sizeof(dwParty), 0);

	ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_CAPTAIN_ID), &iCaptainId, (DWORD)sizeof(iCaptainId), 0);
	if(iCaptainId==0)//chua co doi
		return false;

	for (int i=0;i<PARTY_SIZE;i++){
		int indx = i * PARTY_DATA_SIZE;
		int iMemId;
		ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_MEM_ID+indx), &iMemId, (DWORD)sizeof(iMemId), 0);
		if(iMemId==0)
			continue;
		char sValue[NPC_NAME_SIZE];
		memset(sValue, 0x0, sizeof(sValue));
		ReadProcessMemory(m_hHandle, (void*)(dwParty + PARTY_MEM_NAME+indx), &sValue, (DWORD)sizeof(sValue), 0);
		CString m_sName = (CString)sValue;
		wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( m_sName.GetString()));
		m_sName = wsResult.c_str();

		if(m_sName==L"")
			return false;

		if(m_sName.MakeLower() == sName.MakeLower()){
			return true;
		}			
	}
	return false;
}
void CCharacter::SaveMemberInvite()
{
	CMarkup xml=serialize->OpenFile();
	CString sName = L"Party";	
	xml.FindElem();
	xml.IntoElem();
	if(xml.FindElem(sName)){
		xml.RemoveElem();
	}
	if(m_vMemberParty.size()==0){
		serialize->SaveFile(&xml);
		return;
	}
	xml.AddElem(sName);
	xml.IntoElem();
	std::vector<CString>::iterator name_member;
	//INV_PARTY* objmem;
	for(name_member = m_vMemberParty.begin(); name_member != m_vMemberParty.end(); name_member++ )
	{
		xml.AddElem(L"Member");
		xml.AddAttrib(L"Role",name_member->Trim());
	}
	serialize->SaveFile(&xml);
}


void CCharacter::LoadMemberParty()
{
	CMarkup xml=serialize->OpenFile();
	if(xml.GetDoc().GetLength()==0)
		return;
	//if(serialize->FindPath(L"/Info/ApplyMember", &xml))
	//	szNameMember = xml.GetElemContent();
	if(serialize->FindPath(L"/Info/Party",&xml)){
		m_vMemberParty.clear();
		while(xml.FindElem()){//Đọc thông tin từng skill
			//INV_PARTY memCfg;
			CString memCfg = xml.GetAttrib(_T("Role"));
			m_vMemberParty.push_back(memCfg);

		}
	}
}
void CCharacter::AutoPartyCheck(byte check){
	//0: uncheck đồng ý lời mời | 1: check đồng ý lời mời | 2: uncheck đồng ý đơn xin | 3: check đồng ý đơn xin
	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_CHECK_AUTO_PARTY,check);
}
//=============Kết Thúc===================================



//


//

//
//
//void CCharacter::Recover(){	
//	if(m_bIsRiding)//Xuống ngựa
//		RideHorse();
//	if(m_iState!=3)
//		::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_RECORVER,0);
//}

//
//
//void CCharacter::Quit(){
//	::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_QUIT,0);
//}
//
//bool CCharacter::MoveAway(){
//	//Từ bỏ mục tiêu
//	if(m_iTargetIdx !=0){
//		m_iTargetIdx = 0;
//		SetTarget();
//	}
//	
//	if(!m_bIsRiding)
//		RideHorse();
//	//Đang chạy thì không tìm tiếp
//	if(m_iState == 2)
//		return true;
//	
//	//Tìm đến 1 điểm trên map
//	xmlMap->ResetPos();
//	if ( !xmlMap->FindElem(_T("Maps")) )
//        return false;
//	if(xmlMap->IntoElem()){
//		while(xmlMap->FindElem()){
//			if(_tstoi(xmlMap->GetAttrib(_T("id")))==m_iCurMapId){
//				if(xmlMap->IntoElem()){
//					if(!xmlMap->FindElem(_T("NPC")))
//						xmlMap->FindElem(_T("Door"));
//						//Convert map coor to screen coor
//					COOR _coor;
//					_coor._iX = _tstoi(xmlMap->GetAttrib(_T("x")));
//					_coor._iY = _tstoi(xmlMap->GetAttrib(_T("y")));
//					ConvertMapToScreen(&_coor);
//					if(GetDistance(_coor)>50)//call move function					
//						MoveTo(_coor._iX,_coor._iY);
//					xmlMap->ResetPos();
//					return true;					
//				}
//			}
//		}
//		xmlMap->ResetPos();
//	}
//	return false;
//}


//bool CCharacter::SearchNPC(){	
//	UINT nIndexMin=0;
//	int dZ,dMin=m_iRange;	
//	m_npc->m_iIndex = 0;
//	for(int i=1;i<NPC_MAX_SIZE;i++){
//		if(i==m_iIndex || i == iLag)
//			continue;
//		m_npc->m_iIndex = i;
//		dZ = GetDistance();
//		if( m_npc->m_szName==L"" || m_npc->m_iType!=0){//Nếu không có NPC hoặc npc không phải quái thì bỏ qua
//			continue;
//		}
//		if (dZ < dMin && dZ != -1) {
//			dMin = dZ;
//			nIndexMin = i;
//		}	
//	}
//	if(nIndexMin>0){
//		m_npc->m_iIndex = nIndexMin;
//		m_iPeopleIdx = nIndexMin;
//		m_npc->GetInfo();
//		iNpcHP = 0;
//		return true;
//	}
//	return false;
//}


//
//
//
//
//

//
//	
//	//if(m_lMonsters.size()>0){//cập nhật bãi đánh quái
//	//	int iDis = GetDistance(m_lMonsters[siCurMoster]);
//	//	if(iDis<100){
//	//		siCurMoster++;
//	//		if(siCurMoster >= m_lMonsters.size())
//	//			siCurMoster = 0;
//	//	}
//	//	if(!m_bPkOnMove){//Neu khong danh tren duong thi chi danh quanh diem
//	//		COOR curPoint;
//	//		if(siCurMoster>0)
//	//			curPoint=(m_lMonsters[siCurMoster-1]);
//	//		else
//	//			curPoint=(m_lMonsters[m_lMonsters.size()-1]);
//	//		if(GetDistance(curPoint)>m_iRangePoint){
//	//			m_iTargetIdx==0;
//	//			SetTarget();
//	//			MoveToTarget(curPoint);
//	//			return;
//	//		}
//	//	}
//	//}
//	
//	//Nếu không có target đang chọn hoặc bị lag thì tìm target mới
//	
////	if(m_iTargetIdx==0 || IsLag()){
//////		bIsFighting = false;
////		
////		//m_npc = SearchNPC();
////		if(m_npc==NULL){//Nếu không tìm được target
////			if(m_iState==2)
////				return;
////			//Nếu có nhiều bãi quái thì chuyển sang bãi mới
////			if(m_lMonsters.size()>0 ){
////				if(siCurMoster >= m_lMonsters.size())
////					siCurMoster = 0;
////				COOR nextMons = m_lMonsters[siCurMoster];
////				if(!m_bPkOnMove){//Không chọn đánh quái trên đường đi
////					MoveToTarget(nextMons);
////				}
////				else{
////					MoveTo(nextMons._iX,nextMons._iY);
////				}
////
////			}
////			return;
////		}
////		iNpcHP = 0;
////	}
////	//Kiểm tra điều kiện 
////	int iSelSkill=m_arrSelSkill[1];
////	//check skill delay
////	if(CheckSkillDelay(true)){
////		iSelSkill=m_arrSelSkill[2];
////		if(CheckSkillDelay(false))
////			iSelSkill=m_arrSelSkill[3];
////	}
////
////	if(iSelSkill!= m_iActiveSkill)
////		SetSkill(iSelSkill);
////	
////		m_npc->GetInfo();
////		COOR coor;
////		coor._iX = m_npc->m_iTdX;
////		coor._iY = m_npc->m_iTdY;
////		if(m_iRangeFight < GetDistance(coor)){//Nếu đang đánh thì kiểm tra tầm đánh
////			if(m_iState!=2)
////				MoveTo();			
////		}
////		else {
////			if(m_bIsRiding && m_bAutoRide)//tự động xuống ngựa khi đánh
////				RideHorse();
////			m_iTargetIdx = m_npc->m_iIndex;
////			SetTarget();
////		}
//}
//

//bool CCharacter::CheckSkillDelay(bool iOnLeft){
//	byte delay;
//	DWORD dwAddress = dwGameBaseAddress + GAME_OFFSET_SKILL;
//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
//	for(int i=0;i<24;i++){
//		if(i==16)
//			dwAddress = dwAddress + GAME_OFFSET_SKILL_2;
//		else
//			dwAddress = dwAddress + GAME_OFFSET_SKILL_1;
//		ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
//	}
//	dwAddress = dwAddress + GAME_OFFSET_SKILL_2;
//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
//	dwAddress = dwAddress + GAME_OFFSET_SKILL_3;
//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
//	dwAddress = dwAddress + GAME_OFFSET_SKILL_4;
//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
//	dwAddress = dwAddress + GAME_OFFSET_SKILL_5;
//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
//	dwAddress = dwAddress + GAME_OFFSET_SKILL_6;
//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
//	dwAddress = dwAddress + GAME_OFFSET_SKILL_7;
//	if(iOnLeft)
//		dwAddress = dwAddress - GAME_OFFSET_SKILL_8;
//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &delay, (DWORD)sizeof(delay), 0);
//
//	return (delay>0 && delay <59);
//}
//bool CCharacter::IsMoving(){
//	if(!bIsRunning)
//		return false;
//	if(GetDistance(cTarget)<50){
//		bIsRunning = false;
//		return false;
//	}
//	return true;
//}


//
//bool CCharacter::IsClick(){
//	//lấy tọa độ click chuột so sánh với tọa độ cũ.
//	COOR cCurMouse;
//	//Get coor X
//	DWORD dwAddress = dwGameBaseAddress + GAME_OFFSET_MOUSE_X;
//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &cCurMouse._iX, (DWORD)sizeof(cCurMouse._iX), 0);
//	//Get coor Y
//	dwAddress = dwGameBaseAddress + GAME_OFFSET_MOUSE_Y;
//	ReadProcessMemory(m_hHandle, (void*)dwAddress, &cCurMouse._iY, (DWORD)sizeof(cCurMouse._iY), 0);
//	
//	if(cCurMouse._iX != cLastClick._iX || cCurMouse._iY != cLastClick._iY )
//	{
//		if(!bIsClick){
//			bIsClick=true;
//			Sleep(100);
//			return true;
//		}
//		if(m_iState==3||m_iState==1 && bIsClick){
//			cLastClick = cCurMouse;
//			bIsClick = false;
//		}
//		return true;
//	}
//	return false;
//}
//
//bool CCharacter::SearchItem(){
//	CItem* item;
//	for(int i=1;i<ITEM_MAX_SIZE;i++){
//		item = new CItem(m_hHandle,i);
//		item->GetInfo();
//		if(item->m_szName!=L""){
//			m_iItemIdx = i;
//			PickItem();
//		}
//		delete item;
//		item=NULL;		
//	}
//	return true;
//}
//
//
//
//bool CCharacter::CheckHotKey(byte shcutKey){
//	byte byHotKey;
//	DWORD dwshortcut = dwShortcutBaseAddress + KEY_SHORTCUT_SIZE * (shcutKey-1);
//	ReadProcessMemory(m_hHandle, (void*)dwshortcut, &byHotKey, (DWORD)sizeof(byHotKey), 0);
//	return byHotKey;
//}
////Tự bom máu
//void CCharacter::BuffHP(){
//	if(!CheckHotKey(m_iKeyHP))
//		return;
//	if(m_iKeyHP==-1)
//		return;
//	WPARAM key = 0x30 + m_iKeyHP;
//	::PostMessage(m_hwnd, WM_KEYDOWN, key, 1);
//}
//
//void CCharacter::BuffMP(){
//	if(!CheckHotKey(m_iKeyMP))
//		return;
//	if(m_iKeyMP==-1)
//		return;
//	WPARAM key = 0x30 + m_iKeyMP;
//	::PostMessage(m_hwnd, WM_KEYDOWN, key, 1);
//}
//
//
//void CCharacter::PickItem(){

//	DWORD dwPlayerInfo = dwGameBaseAddress + PLAYER_ITEM_TARGET;
//	WriteProcessMemory(m_hHandle,  (LPVOID*)dwPlayerInfo, &m_iItemIdx, sizeof(m_iItemIdx), 0);
//}
//
//





//======================Van Tieu========================


//void CCharacter::GetLocation(){
//	DWORD dwPlayerName = dwGameBaseAddress + QUEST_BASE_ADDRESS;	
//	ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &dwPlayerName, (DWORD)sizeof(dwPlayerName), 0);
//	for(int i = 0; i < 32; i++)
//	{
//		if(i == 26 || i == 27)
//		{
//			dwPlayerName = dwPlayerName + GAME_MENU_OFFSET_2;	
//			ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &dwPlayerName, (DWORD)sizeof(dwPlayerName), 0);
//		}
//		else{
//			dwPlayerName = dwPlayerName + GAME_MENU_OFFSET_1;	
//			ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &dwPlayerName, (DWORD)sizeof(dwPlayerName), 0);
//	
//		}
//	}
//	dwPlayerName = dwPlayerName + OFFSET_NAME_PARTY;	
//	ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &dwPlayerName, (DWORD)sizeof(dwPlayerName), 0);
//	//ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &dwPlayerName, (DWORD)sizeof(dwPlayerName), 0);
//
//	char sValue[SIZE_TEXT];
//	char sMember[SIZE_TEXT];
//	memset(sValue, 0x0, sizeof(sValue));
////	memset(sMember, 0x0, sizeof(sMember));
//	ReadProcessMemory(m_hHandle, (void*)dwPlayerName, &sValue, (DWORD)sizeof(sValue), 0);
//	
//	//TachChuoi(sMember,sValue,'>','<');
//		
//	m_szLocation = (CString)sValue;
//	
//	//string converStr( );
//	wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( m_szLocation.GetString() ));
//	m_szLocation = wsResult.c_str();
//	
//
//}

bool CCharacter::ClickRecived(){
	Sleep(200);
	bool LResult= (bool)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_CHOOSE_ADWARD,0);
	return LResult;
}
int CCharacter::TotalLineMenu()
{
	int total;
	DWORD base = dwGameBaseAddress + MENU_BASE_ADDRESS;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
		if(base == NULL)
			return 0;
		
	for(int i = 0; i<47; i++)
	{
		base = base + GAME_MENU_OFFSET_1;
		ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
		if(base == 0)
			return 0;
		
	}
	DWORD base47 = base + GAME_MENU_OFFSET_1;
	ReadProcessMemory(m_hHandle, (void*)base47, &base47, (DWORD)sizeof(base47), 0);
	DWORD base48 = base47 + GAME_MENU_OFFSET_1;
	ReadProcessMemory(m_hHandle, (void*)base48, &base48, (DWORD)sizeof(base48), 0);
	DWORD base49 = NULL;
	if(base48 != NULL)
	{
		base49 = base48 + GAME_MENU_OFFSET_1;
		ReadProcessMemory(m_hHandle, (void*)base49, &base49, (DWORD)sizeof(base49), 0);
	}
	//if(bMenuSel && m_iOS==6164)//21040510
	//{
	//	base = base + GAME_MENU_OFFSET_1;
	//	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	//}
	//bMenuSel=!bMenuSel;
	if(base49 != NULL)
	{
		base = base49 + GAME_MENU_OFFSET_2;
		ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	}
	else if(base48 != NULL)
	{
		base = base48 + GAME_MENU_OFFSET_2;
		ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	}
	else if(base47 != NULL)
	{
		base = base47 + GAME_MENU_OFFSET_2;
		ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	}
	base = base + GAME_MENU_OFFSET_1;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	base = base + GAME_MENU_OFFSET_1;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	
	base = base + GAME_MENU_OFFSET_2;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	base = base + GAME_MENU_OFFSET_2;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	base = base + GAME_MENU_OFFSET_1;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	base = base + GAME_MENU_OFFSET_1;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);	

	base = base + GAME_MENU_TOTAL_LINE;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	total = (int)base;

	return total;
}

int CCharacter::GetNPCInfo(CString szname)
{
	//==========NghiaLP====20141604====Van tieu
		//Get name Ton Hieu(thong tin nguoi van tieu)
	DWORD base = dwGameBaseAddress + GAME_OFFSET_INFO;
	ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
	DWORD dwnameHorse  = base;
	for(int i=1;i<NPC_MAX_SIZE;i++)
	{
		base  = dwnameHorse + GAME_OFFSET_DATASIZE * i;
		ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
		DWORD dwInf = base; 
		base = base + NPC_NAMESTATE;
		
		char sValue[SIZE_TEXT];
		memset(sValue, 0x0, sizeof(sValue));	
		ReadProcessMemory(m_hHandle, (void*)base, &sValue, (DWORD)sizeof(sValue), 0);
		CString szTonHieu = (CString)sValue;
		
		//string converStr( );
		wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( szTonHieu.GetString() ));
		szTonHieu = wsResult.c_str();
		

		
		if(szname == szTonHieu)
		{
			m_iTargetIdx = i;
			m_npc->m_iIndex = i;
			m_npc->GetInfo();

			m_cHorse.m_sName = szTonHieu;  // name
			m_cHorse.m_iCurMap = m_iCurMapId;   //Map
			
			DWORD dwX = dwInf + NPC_COOR_X;
			ReadProcessMemory(m_hHandle, (void*)dwX, &dwX, (DWORD)sizeof(dwX), 0);
			m_cHorse.m_iCurX = (int)dwX;   //toa do X
			
			DWORD dwY = dwInf + NPC_COOR_Y;
			ReadProcessMemory(m_hHandle, (void*)dwY, &dwY, (DWORD)sizeof(dwY), 0);
			m_cHorse.m_iCurY = (int)dwY;	//toa do Y

			//m_iBackHorse_MapID = m_iCurMapId;
			m_iBackHorse_X = m_cHorse.m_iCurX;
			m_iBackHorse_Y = m_cHorse.m_iCurY;

			return i;
		}
		
	}
	return 0;
}
//int CCharacter::Attacked(int TagetHorse)
//{
//	DWORD base = dwGameBaseAddress + GAME_OFFSET_INFO;
//	ReadProcessMemory(m_hHandle, (void*)dwGameBaseAddress, &base, (DWORD)sizeof(base), 0);
//	DWORD dwAtt = base;
//	int ack;
//	for(int i=1;i<NPC_MAX_SIZE;i++)
//	{
//		base  = dwAtt + GAME_OFFSET_DATASIZE * i;
//		ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
//		base  = base + NPC_PK_TARGET;
//		ReadProcessMemory(m_hHandle, (void*)base, &base, (DWORD)sizeof(base), 0);
//		
//		ack = (int)base;
//		if(ack == TagetHorse)
//			return i;
//	}
//	return 0;
//}
void CCharacter::GotoHorse()
{
	////từ bỏ mục tiêu
	//m_iTargetIdx=0;
	//SetTarget();
	////Lấy thông tin đội trưởng
	CString szname = L"Do <color=yellow>" + m_szName + L"<color> áp tải"; 
	int targetHorse = GetNPCInfo(szname);
	if(m_iBackHorse_MapID != m_iCurMapId){//không cùng map với tieu
		if(FindingAWay(m_iBackHorse_MapID, m_iBackHorse_X ,m_iBackHorse_Y))
				return;
	}
	COOR dist;
	if(m_iBackHorse_X != 0 && m_iBackHorse_Y!= 0)
	{
		dist._iX = m_iBackHorse_X;
		dist._iY = m_iBackHorse_Y;
		if(GetDistance(dist) > 800)
		{
			MoveTo(m_iBackHorse_X,m_iBackHorse_Y);
			return;
		}
	}
	if(targetHorse ==0){

		CString str = FindNPC(L"trạm ");
		if(str != L"")			//Tim thay dich tram
		{
			if(AskNPC(str,true)==1)
			{					
				Sleep(200);
				if(!ChooseMenu(0))
					return;
			}
			else 
				return;
				//MoveTo(m_iBackHorse_X,m_iBackHorse_Y);
		}
		/*else
			m_iCurEscort = 1;
			return;*/
	}
	
	COOR coor;
	coor._iX =m_cHorse.m_iCurX;
	coor._iY = m_cHorse.m_iCurY;
	
	/*if(!m_bIsRiding && m_bAutoRide)
		RideHorse();*/
	
	//m_iTargetIdx = GetNPCInfo(L"Cướp Tiêu");
	int LResult = (int)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_OPEN_MENU,0);
	if(LResult == 1)
		ChooseMenu(-1);
	if(GetNPCInfo(L"Cướp Tiêu") == 0)   //
	{
		if(GetDistance(coor)<m_iDistance)
			return;
		MoveTo(coor._iX,coor._iY);
	}
	else
		DoFight();

}


bool	CCharacter::ReceiveEscort(){

	/*if(CheckFinishQuest(L"vận tiêu(0)"))
	{
		ShowMessage(L"Da Van Tieu Xong!!!!!");
		return false;
	}*/
	/*if(m_iCurMapId == 77)
		MoveTo(51555,103036);*/
	if(m_iCurEscort<0 || !m_bEscort)
		return false;
	if((m_iCurEscort == 3 ||m_iCurEscort == 5) && m_bReciveAdward == false)//Đã nhận nhiệm vụ, tra nhiem vu
		return true;
	if(m_iCurEscort==0 || m_iCurEscort ==1)
	{		
		CString strText = GetQuestText(L"<pic=58><color=yellow>Vận Tiêu");
		if(strText!=L"")
		{
			
			GetLinkPos(strText);
			//lay doan link
			if(m_iCurEscort==1)   //
			{
				if(m_iProsessType == 1)
				{
					m_iCurEscort = 4;
					return false;
				}
			}
			else if(m_iBackHorse_MapID != m_iCurMapId || m_iCurMapId != cTarget._iMapId)
			{
				if(m_iProsessType == 2)
				{			//Toi nguoi UY thac
					m_iCurEscort=3;  // Dang lam NV(dang ho tong)
					return true;
				////	goBack=true;
				}else if(m_iProsessType == 3)
				{
					m_iCurEscort = 5;	//Giao nhiem vu 
					return true;
				}
				else
					m_iCurEscort = 4;	//O trong thanh nhan NV
			}
			else if(m_iProsessType == 3 && m_bReciveAdward == true)
			{		//da giao NV
				m_iCurEscort=1;	
				m_bReciveAdward == false;
				//return true;
			}
			else
			{
				m_iCurEscort = 3;
				m_bReciveAdward == false;
				return true;
			}
		}
		else
			m_iCurEscort = 1;
	}

	if(m_iCurEscort == 3 || m_iCurEscort == 5)
	{
		m_bReciveAdward = false;
		return false;
	}

	if(AskNPC(L"Thẩm Chấn Uy", false)==1)
	{
		if(m_iCurEscort == 1)
		{   
			Sleep(800);
			if(!ChooseMenu(3))
				return false;
			else
				m_iCurEscort = 4;
			if(TotalLineMenu() > 3)
				if(!ChooseMenu(3))
					return false;
			Sleep(500);
			//AutoChat(L"Đã nhận nhiệm vụ vận tiêu");
			m_bReciveAdward = false;
			if(ChooseMenu(1))
			{
				 //da nhan NV (chua giao NV)
				
				Sleep(1000);
				m_iCurEscort = 3;
				m_bReciveAdward == false;
				return true;
			}
			m_iCurEscort = 4;
			
		}
		if(m_iCurEscort == 4)
		{ //Duy Chuyen toi nguoi uy thac 
			Sleep(200);
			m_bReciveAdward == false;
			if(TotalLineMenu() == 1)
			{
				
				ClickOnLink();
				m_bReciveAdward == false;
				m_iCurEscort=3;
			}
			else if(ChooseMenu(1))
			{
				Sleep(1000);
				m_bReciveAdward == false;
				m_iCurEscort=3;
				return true;
			}
			//else 
			//{
			//	//m_iCurQuest = 1;
			//	return true;
			//}
		}

	}
	return false;
}





bool CCharacter::ProcessEscort(){
	if(m_iCurEscort>3)
		return true;
	CString szQuestText = GetQuestText(L"<pic=58><color=yellow>Vận Tiêu");
	CString szNameMap = GetNameMap(szQuestText);
	switch(m_iCurEscort){
		//case 1:
		//	//lấy text mô tả
		//	Sleep(500);
		//	if(szQuestText!=L"")
		//	{
		//		if(GetLinkPos(szQuestText)){
		//				if(ChooseMenu(1)==false){
		//					m_bReciveAdward == false;
		//					m_iCurEscort = 2;
		//					return false;
		//				}
		//				else 
		//				{
		//					m_bReciveAdward == false;
		//					m_iCurEscort = 3;
		//					return true;
		//				}
		//			}
		//		}
		//	return false;
		//case 2://di chuyen
		//	if(GetLinkPos(szQuestText))
		//	{
		//		if(FindingAWay(cTarget._iMapId, cTarget._iX ,cTarget._iY))
		//			return false;
		//		m_iCurEscort = 3;
		//		m_bReciveAdward = false;
		//	}
		//	break;
		case 3://xử lý
			//GetLocation();
			GetLinkPos(szQuestText);  //lay trang thai m_iProsessType
			switch(m_iProsessType){
				case 1:
					//if(AskNPC(L"Ủy thác " + m_szLocation)==0){
					/*if(AskNPC(FindNPC(L"ủy thác"),true)==1 || AskNPC(FindNPC(L"Ủy thác"),true)==1
						|| AskNPC(FindNPC(L"Người ủy thác"),true)==1)*/
					if(m_iBackHorse_MapID == m_iCurMapId)   //Neu dung cung map
					{
						if(AskNPC(FindNPC(L"ủy thác"),true)==1)
						{
							Sleep(200);
						//	ChooseMenu(0);
							if(ChooseMenu(0))
							{	//chưa đóng được menu
								GetLinkPos(L"<pic=58><color=yellow>Vận Tiêu");//m_iCurQuest = 4; //khong chon duoc chay ve thành
								//return true;
							}
							else
							{
								//m_iCurQuest = 3;
								return false;
							}
						}
					}
					break;
				case 2:
					GotoHorse();
					break;
			}
			szQuestText = GetQuestText(L"<pic=58><color=yellow>Vận Tiêu");
			if(szQuestText!=L""){
				if(szQuestText.Find(L"Tổng Tiêu Đầu")!=-1){
					
					m_iCurEscort = 5;
					return true;
				}
				/*else{
					GetLinkPos(szQuestText);
					if(GetDistance(cTarget)>1000){
						if (FindingAWay(cTarget._iMapId, cTarget._iX ,cTarget._iY))
							m_iCurQuest = 2;
					}*/
				//}
			}
			break;
	}
	return false;
}
bool CCharacter::ReceiveAward()
{
	//AutoChat(L"Xong nhiệm vụ rồi! đi nhận thưởng thôi!");
	CString szQuestText = GetQuestText(L"<pic=58><color=yellow>Vận Tiêu");
	int menu =-1;
	if(szQuestText!=L""){
		GetLinkPos(szQuestText);
		if(m_iCurEscort == 5 && m_bReciveAdward == false){
			int menu = AskNPC(L"Thẩm Chấn Uy", true);
			if(menu==1)
			{
				Sleep(1000);
				/*if(TotalLineMenu() > 3)
				{
					m_bReciveAdward = true;
					m_iCurEscort = 1;
				}*/
				if(!ChooseMenu(1))
					return false;
			}
			Sleep(500); 
			if(!ClickRecived())
				return false;
			else{	
				m_bReciveAdward = true;
				m_iCurEscort = 1;
				m_iBackHorse_MapID = 0;
			}
			return true;	
				////	ChooseMenu(1);
				//ChooseMenu(1);
				//	m_iCurQuest == 0;
				//	}
		
		}
	}
	return false;
}

/////////////////Van Tieu- 20140423==============
CString	CCharacter::FindNPC(CString szName)
{
	
	CNPC npc(m_hHandle,dwGameBaseAddress);
	for(int i=1;i<NPC_MAX_SIZE;i++){
		if(i==m_iIndex)//bỏ qua nhân vật
			continue;
		npc.m_iIndex = i;
		npc.GetInfo();

		//CString szStr1 = ConvertLower(L"HỦ VỒ  Ắ Ậ Ẩ");

		CString szStr = ConvertLower(npc.m_szName);
		if( npc.m_szName==L"")//Không tìm thấy target bỏ qua
			continue;
		if( npc.m_iType!=3)//không phải NPC bỏ qua
			continue;
		if(szStr.Find(szName)!=-1){//lấy NPC
			cTarget._iMapId = m_iCurMapId;
			cTarget._iX = npc.m_iTdX;
			cTarget._iY = npc.m_iTdY;
			return npc.m_szName;
		}
	}
	return L"";
}
CString CCharacter::GetNameMap(CString szname)
{
	CMarkup xmlMap;
	xmlMap = XMLSerialized::OpenFile(L"Map.cfg");
	if(xmlMap.GetDoc().GetLength()==0)
		return false;
    xmlMap.ResetPos();
	if (!xmlMap.FindElem(_T("Maps")))
        return false;
	
	if(xmlMap.IntoElem()){
		while(xmlMap.FindElem()){
			if(szname.Find(xmlMap.GetAttrib(_T("name")))!=-1){
				m_iBackHorse_MapID = _tstoi(xmlMap.GetAttrib(_T("id")));
				return xmlMap.GetAttrib(_T("name"));
			}
		}
	}
	return L"";
}
//=============Kết Thúc===================================

CString CCharacter::ConvertLower(CString szName)
{
	wstring str = (wstring)szName;
	string sResult =XMLSerialized::UnicodeToUTF8(str);
	//string sResult = (string)szName;
	char sValue[SIZE_TEXT];
	for(int i = 0; i < sResult.length() ; i++)
	{
		sValue[i] = (char)(LPCTSTR)sResult[i];
	}
	sValue[sResult.length()] ='\0';
	for(int i = 0; i <= sResult.length(); )
	{
		if((byte)sValue[i] >=  65 && (byte)sValue[i] <=  90)
		{
			sValue[i] = sValue[i] + 32;
			i++;
		}
		else if((byte)sValue[i] ==  0xC3)
		{
			i++;
			if((byte)sValue[i] >= 0x80 && (byte)sValue[i] <= 0x9F)
			{
				sValue[i] = sValue[i] + 32;
				i++;
			}
		}
		else if((byte)sValue[i] ==  0xE1)
		{
			i = i+2;
			if((byte)sValue[i] %2 ==0)
			{
				sValue[i] = sValue[i] + 1;
				i++;
			}
		}
		else if((byte)sValue[i] ==  0xC4)
		{
			i++;
			if((byte)sValue[i] ==  0x82 || (byte)sValue[i] ==  0xA8)
			{
				sValue[i] = sValue[i] + 1;
				i++;
			}

		}
		else if((byte)sValue[i] ==  0xC5)
		{
			i++;
			if((byte)sValue[i] ==  0xA8)
			{
				sValue[i] = sValue[i] + 1;
				i++;
			}

		}
		else if((byte)sValue[i] ==  0xC6)
		{
			i++;
			if((byte)sValue[i] ==  0xA0 || (byte)sValue[i] ==  0xAF)
			{
				sValue[i] = sValue[i] + 1;
				i++;
			}

		}
		else
			i++;


	}

	CString szStr  = (CString)sValue;
	
	//string converStr( );
	wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( szStr.GetString() ));
	szStr = wsResult.c_str();
	return szStr;
}


////////////SangDH--20140602--Fix lỗi dùng hồi thành thạch khi làm nhiệm vụ đánh quái
//							--Fix lỗi tìm đánh đúng quái khi làm nhiệm vụ
//Get Beaten target
int	 CCharacter::GetHitMeTarget(){
	//Get player Info address
	DWORD dwAddress = dwGameBaseAddress + GAME_OFFSET_INFO;
	ReadProcessMemory(m_hHandle, (void*)dwAddress, &dwAddress, (DWORD)sizeof(dwAddress), 0);
	
	CNPC npc(m_hHandle,dwGameBaseAddress);
	int iTarget =0; 
	
	for(int i=1;i<NPC_MAX_SIZE;i++){
		if(i==m_iIndex)
			continue;

		DWORD dwTempAddress = dwAddress + GAME_OFFSET_DATASIZE * i;
		ReadProcessMemory(m_hHandle, (void*)dwTempAddress, &dwTempAddress, (DWORD)sizeof(dwTempAddress), 0);
		
		 //	 Get target PK Index
		dwTempAddress = dwTempAddress + NPC_PK_TARGET;
		ReadProcessMemory(m_hHandle, (void*)dwTempAddress, &iTarget, (DWORD)sizeof(iTarget), 0);
		
		if(iTarget==m_iIndex)
			return iTarget;
	}
	
	return 0;
}


int CCharacter::SearchNPCByName(CString sName){
	CNPC npc(m_hHandle,dwGameBaseAddress);
	COOR coor;
	int idZ,iIndex=0,iMin=0;

	for(int i=1;i<NPC_MAX_SIZE;i++){
		if(i==m_iIndex)//bỏ qua nhân vật
			continue;
		npc.m_iIndex = i;
		npc.GetInfo();
		if( npc.m_szName==L"")//Không tìm thấy target bỏ qua
			continue;
		if( npc.m_iType ==1)//bỏ qua người chơi
			continue;
		if(sName.Find(L"Thủ Lĩnh")!=-1 && npc.m_szName.Find(L"Thủ Lĩnh")==-1){
			continue;
		}
		if(npc.m_szName.Find(L"Thủ Lĩnh")!=-1 && sName.Find(L"Thủ Lĩnh")==-1){
			continue;
		}
		if(CompareString(sName,npc.m_szName) >=50){//lấy NPC
			//Tìm theo khoảng cách	
			
			coor._iX = npc.m_iTdX;
			coor._iY = npc.m_iTdY;
			idZ = GetDistance(coor);
			if(idZ<= iMin || iMin==0){
				iMin = idZ;
				iIndex = i;
			}
		}
	}
	return iIndex;
}

int CCharacter::CompareString(CString sStr1,CString sStr2, bool exact/*=false*/){
	int iCount = 0;
	int iTotal = 0;
	CString curWord=L"";
	CString curLetter=L"";
	CString strCompare = ConvertLower(sStr1);
	CString strCompare1 = ConvertLower(sStr2);
	/*if(strCompare.GetLength() > strCompare1.GetLength()){
		strCompare = sStr2;
		strCompare1 = sStr1;
	}
	strCompare = ConvertLower(strCompare);
	strCompare1 = ConvertLower(strCompare1);*/
	int length = strCompare.GetLength();
	for(int i=0;i<length;i++){
		curLetter = strCompare.GetAt(i);
		if(curWord!=L"" && (curLetter==L"·" ||curLetter==L" " || i == length-1)){
			if(strCompare1.Find(curWord)!=-1)
				iCount++;
			iTotal++;
			curWord = L"";
		}else{
			curWord = curWord + curLetter;
		}
	}
	return iCount*100/iTotal;
}

//////////////////NghiaLP-20140604---Dieu Doi///////////////////////
bool CCharacter::OpenDialogQuest()
{
	bool LResuft = ::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_OPEN_QUEST,0);
	return LResuft;
}
bool CCharacter::ChooseQuest(int indx)
{	
	Sleep(200);
	bool LResuft = ::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_CHOOSE_QUEST,indx);
	return LResuft;

}

bool CCharacter::SendTeamDoQuest()
{
	if(IsInterval(6,4))
		return false;
	if(m_iCurMapId == 77 && GetBaseText(-1) != m_szName)  //Neu dang o gia vien nguoi khac
	{
		MoveTo(51555,103036);
	}
	if(m_iCurMapId == 77 && GetBaseText(-1) == m_szName)
	{
		
		if(GetBaseText(24).Find(L"Đang điều động") == -1 || GetNameParty() == L"Ngọc Nhi")
		{
			if(OpenDialogQuest())
			{
				if(GetBaseText(24).Find(L"Đang rảnh") != -1)
				{
					SendTeamGoTo();
				}
				else if(GetBaseText(24).Find(L"Đã điều động") != -1 || GetNameParty() == L"Ngọc Nhi")
				{
					::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_AGREE_PARTY,1);
					return ChooseQuest(-1);
				}
			}
		}
		if(GetBaseText(24).Find(L"Đang điều động") != -1)
		{
			ChooseQuest(-3);   //Tat cua so dieu doi
			m_bWaiting = true;		
			return false;
		}
	}
	else
	{
		if(GetBaseText(24).Find(L"Đang điều động") == -1 || GetNameParty() == L"Ngọc Nhi")
		{
			int menu = AskNPC(L"Nghiêm Nhiếp Ảnh");
			if(menu!=1)
			{
				return false;
			}
			if(!ChooseMenu(3))
			{//vào Gia Viên
				return false;
			}
		}
	}
	return false;
}

CString CCharacter::GetBaseText(int indx)
{
	DWORD dwbase = ::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_TEXT_QUEST,indx);

	char sValue[SIZE_TEXT];
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(m_hHandle, (void*)dwbase, &sValue, (DWORD)sizeof(sValue), 0);
	
	
	CString sName = (CString)sValue;
	/*int iStartIndx = m_sName.Find(L">")+1;
	int iEndIndx =  m_sName.Find(L"<",iStartIndx);
	m_sName=m_sName.Mid(iStartIndx,iEndIndx-iStartIndx);*/
	
	wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( sName.GetString() ));
	sName = wsResult.c_str();
	return sName;
}


int CCharacter::ConvertToInt(int indx)
{
	if(indx == 0)
	{
		CString szHealt = GetBaseText(indx);
		//int iStartIndx = m_sName.Find(L">")+1;
		int iEndIndx =  szHealt.Find(L"/",0);
		szHealt=szHealt.Mid(0,iEndIndx);
		int iHealt = _ttoi(szHealt);
		return iHealt;
	}
	else
	{
		CString szCount = GetBaseText(indx);
		int iCount = _ttoi(szCount);
		return iCount;
	}

}
////////================20140609=====================
bool CCharacter::SendTeamGoTo()
{
		if(m_bBC && ConvertToInt(13)!=0 && ConvertToInt(0) > 60) //Neu check bi canh
		{
			int LResult= (int)::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_OPEN_MENU,0);
			if(LResult == 1)
			{			
				if(ChooseMenu(3-m_iKindBC))
				{
					Sleep(2000);
					return true;
				}
			}
			else if(ChooseQuest(0)){  //Chon nv bi canh
				if(ChooseQuest(-1))  //nhan Nut Phai
				{
					if(LResult == 1)
					if(ChooseMenu(3-m_iKindBC))
					{
						Sleep(2000);
						return true;
					}
					return false;
				}
			}
		}
		else if(m_bTVP && ConvertToInt(10)!=0 && ConvertToInt(0) > 60)
		{
			if(ChooseQuest(1))  //Chon nv bi canh
				if(ChooseQuest(-1))  //nhan Nut Phai
					if(ChooseQuest(-2))
					{
							Sleep(2000);
							return true;
					}
					return false;
		}
		else if(m_bVT && ConvertToInt(5)!=0 && ConvertToInt(0) > 30)
		{
			if(ChooseQuest(2))  //Chon nv bi canh
				if(ChooseQuest(-1))  //nhan Nut Phai
					if(ChooseQuest(-2))
					{
							Sleep(2000);
							return true;
					}
					return false;
		}
		else if(m_bTN && ConvertToInt(4)!=0 && ConvertToInt(0) > 20)
		{
			if(ChooseQuest(3))  //Chon nv bi canh
				if(ChooseQuest(-1))  //nhan Nut Phai
					if(ChooseQuest(-2))
					{
							Sleep(2000);
							return true;
					}
					return false;
		}
		else if(m_bTVPhai && ConvertToInt(1)!=0 && ConvertToInt(0) > 60)
		{
				if(m_iLevel < 100)
				{
					if(ChooseQuest(4))  //Chon nv bi canh
						if(ChooseQuest(-1))  //nhan Nut Phai
							if(ChooseQuest(-2))
							{
								Sleep(2000);
								return true;
							}
						return false;
				}
			//	else
					////Cap ban khong phu hop
		}
		return false;
}


//==========NghiaLP======UU tieu Nhiem Vu===



void CCharacter::PriorityQuest()
{
	
	GetDoQuest();
	switch(m_iQuesting){
		case 1:
			SendTeamDoQuest();
			break;
		case 2:
			if(ReceiveEscort()){ //===========Van Tieu==============
				if(ProcessEscort())
					ReceiveAward();
			}
			break;
		case 3:
			if(m_iCurMapId == 77)
				MoveTo(51555,103036);
			else if(ReceiveQuest())
				ProcessQuest();
			break;
	}
	//if(m_iQuesting == 1)
	//	
	//else if(m_iQuesting == 2)
	//{
	//	if(ReceiveEscort()){ //===========Van Tieu==============
	//		if(ProcessEscort())
	//			ReceiveAward();
	//	}
	//}
	//else if(m_iQuesting==3){
	//	if(m_iCurMapId == 77)
	//		MoveTo(51555,103036);
	//	if(ReceiveQuest())
	//		ProcessQuest();
	//}
}

void CCharacter::GetDoQuest()
{
	//SendTeamDoQuest();
	//bool bTrue = CheckFinishQuest(L"cầu phúc(0)");

	
	if(m_bReciveAdward || m_iCurQuest ==0 || m_iCurQuest==6)	{//Da Nhan thuong NV van tieu
		if(GetNameParty() == L"Ngọc Nhi"){  //Da dieu doi xong
			m_bWaiting = false;   
		}
	}
	if(m_bSendTeamDoQuest && !m_bWaiting){			//check NV dieu doi va KHong phai trang thai dang cho hoac da nhan thuong Van tieu
		m_iQuesting = 1;
		return;
	}
		//SendTeamDoQuest();
	if(m_bWaiting || !m_bSendTeamDoQuest)			//Dang cho dieu doi hoac ko check dieu doi
	{
		if(m_iCurMapId == 77)  //Neu dang o gia vien thi di ra ngoai lam nv khac
		{
			MoveTo(51555,103036);
		}
		if(m_bEscort)
		{
			//if(!CheckFinishQuest(L"vận tiêu(0)"))
			//{
				m_iQuesting = 2;
			//}
			/*else if(m_bAutoQuest)
			{
				m_iQuesting = 3;
			}*/
		}
		else if(m_bAutoQuest)
		{
			/*if(!CheckFinishQuest(L"lịch luyện(0)"))
			{*/
				m_iQuesting = 3;
			//}
		}
		else
			m_iQuesting = 0;
	}
}

bool CCharacter::CheckFinishQuest(CString szText)
{

	for(int indx = 1; indx < 30; indx++)
	{
		DWORD dwbase = ::SendMessage(m_hwnd , WM_HOOK_WRITE,MSG_CHECK_FINISH,indx);
		char sValue[SIZE_TEXT];
		memset(sValue, 0x0, sizeof(sValue));
		ReadProcessMemory(m_hHandle, (void*)dwbase, &sValue, (DWORD)sizeof(sValue), 0);
		
		
		CString sName = (CString)sValue;

		wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( sName.GetString() ));
		sName = ConvertLower(wsResult.c_str());
		if(sName == szText)
		{
			//ShowMessage(L"Nhiem vu hoan thanh");
			return true;
		}
	
	}
	return false;
}


//---20140612--SangDH--
CString CCharacter::ConvertString(CString str){
	wstring wsResult = XMLSerialized::UTF8ToUnicode( (string)CW2A( str.GetString()));
	return (CString) wsResult.c_str();
}

bool CCharacter::IsInterval(int ID,int iDelay){
	time_t timer;
	time(&timer);
	TIMER::iterator result = m_mDelay.find(ID);
	if(result==m_mDelay.end()){
		m_mDelay.insert(make_pair(ID,timer));
		return false;
	}
	double seconds;
	seconds = difftime(timer,m_mDelay[ID]);
	if(seconds<iDelay)
		return true;

	m_mDelay[ID]=timer;
	return false;
}
void  CCharacter::RemoveInterval(int ID){
	m_mDelay.erase(ID);
}