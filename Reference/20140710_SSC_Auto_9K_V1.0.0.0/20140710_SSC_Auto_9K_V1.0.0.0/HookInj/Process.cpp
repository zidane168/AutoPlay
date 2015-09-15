/*
20141106 - SangDH - Update version game 2.36.1
	+ Nâng cấp lên version 1.8.0 thêm tính năng điều đội
20140611--SangDH - Update hàm Init(): thay đổi cách lấy base address
//20140606 == update so luong base tren Item ham :ClickOnItem(HWND hwnd,int Indx): 157offset ==> 158ofset
//20140606 == update so luong base tren city: GetWorldMapAddress(): 159ofset ==> 160ofset
*/

#include "StdAfx.h"
#include "Process.h"

#include "../SSC_GameEngine/GameInfo.h"


CCharacter::CCharacter(void)
{
	m_dwBase = NULL;
	m_dwBackToCityBase = NULL;
	m_dwWorldMap = NULL;
	m_dwWorldMapButton = NULL;
	m_dwWorldMapCity = NULL;
	m_dwSceneMap = NULL;
	m_dwPackage = NULL;
	bLinkSel = false;
	bMenuSel = false;
	bMapSel = false;
	dwBaseSelectMenu = NULL;
	dwCheckInviteParty = NULL;
	dw_GetBase = NULL;
	bChooseQuest = false;
	bSendTeam = false;
	
	m_dwMapAddress = NULL;

	dwCheckFinish = NULL;
}

CCharacter::~CCharacter(void)
{
	m_dwBase = NULL;
	m_dwBackToCityBase = NULL;
	m_dwWorldMap = NULL;
	m_dwWorldMapButton = NULL;
	m_dwWorldMapCity = NULL;
	m_dwSceneMap = NULL;
	m_dwPackage = NULL;
	dwBaseSelectMenu = NULL;
	dwCheckInviteParty=NULL;
	dw_GetBase = NULL;
	bChooseQuest = false;
	bSendTeam = false;

	m_dwMapAddress = NULL;
	dwCheckFinish = NULL;

}
void CCharacter::Init(void){
	//iOS = sys::DetectOS();
	////Get Base Address win64
	//if(iOS==6164||iOS==5164||iOS==5264)
	//	m_dwBase = (DWORD)((*(LPDWORD)GAME_BASE_ADDRESS) + (GAME_OFFSET_1));
	////Get Base Address win32
	//else{
	//	if(iOS==5132){
	//		m_dwBase = (DWORD)((*(LPDWORD)((*(LPDWORD)GAME_BASE_ADDRESS_XP_32) + GAME_OFFSET_32))+GAME_OFFSET_1 );
	//	}
	//	else if(iOS==6132){
	//		m_dwBase = (DWORD)((*(LPDWORD)((*(LPDWORD)GAME_BASE_ADDRESS_32) + GAME_OFFSET_32))+GAME_OFFSET_1 );
	//	}
	//}

	//m_dwNpcBase = (DWORD)((*(LPDWORD)m_dwBase) + GAME_OFFSET_INDEX) ;
	//DWORD dwIndex = (DWORD)((*(LPDWORD)m_dwNpcBase) + NPC_INDEX) ;
	//int* iIdx = (int*)dwIndex;
	//m_dwNpcBase = (DWORD)((*(LPDWORD)m_dwBase) + GAME_OFFSET_INFO) ;
	//m_dwNpcBase = (DWORD)((*(LPDWORD)m_dwNpcBase) + (GAME_OFFSET_DATASIZE *(*iIdx))) ;

	CString szName;
	m_dwBase = sys::GetBaseAddress(GetCurrentProcess(),szName);
	if(m_dwBase==NULL)
		return;
	m_dwNpcBase = (DWORD)(m_dwBase + GAME_OFFSET_INDEX) ;
	DWORD dwIndex = (DWORD)((*(LPDWORD)m_dwNpcBase) + NPC_INDEX) ;
	int* iIdx = (int*)dwIndex;
	m_dwNpcBase = (DWORD)(m_dwBase + GAME_OFFSET_INFO) ;
	m_dwNpcBase = (DWORD)((*(LPDWORD)m_dwNpcBase) + (GAME_OFFSET_DATASIZE *(*iIdx))) ;

	m_dwMapAddress = (DWORD)(m_dwBase + GAME_OFFSET_MAPID);
	m_dwMapAddress = (DWORD)((*(LPDWORD)m_dwMapAddress) + NPC_MAP) ;

}

int CCharacter::SetTarget(int tarIndex)
{
	DWORD dwTarget = (DWORD)((*(LPDWORD)m_dwNpcBase) + NPC_TARGET) ;
	int* tar = (int*)dwTarget;
	*tar = tarIndex;
	return *tar;
}
bool CCharacter::SetSkill(int iSkillId)
{
	DWORD dwSkillId = (DWORD)((*(LPDWORD)m_dwNpcBase) + NPC_ACTIVE_SKILL) ;
	int* id = (int*)dwSkillId;
	*id = iSkillId;
	return true;
}
bool CCharacter::DoRightSkill(HWND hwnd,int lParam)
{
	
	//DWORD func = (DWORD)((*(LPDWORD)m_dwBase) + FUNC_RIGHT_CLICK) ;
	//DWORD func2 = (DWORD)((*(LPDWORD)m_dwBase) + FUNC_RIGHT_CLICK_1) ;
	DWORD func = (DWORD)(m_dwBase + FUNC_RIGHT_CLICK) ;
	DWORD func2 = (DWORD)(m_dwBase + FUNC_RIGHT_CLICK_1) ;

	int iCenX = ((int)(*(LPDWORD)(m_dwBase + CENTER_X_OFFSET)))/2 ;
	int iCenY = ((int)(*(LPDWORD)(m_dwBase + CENTER_Y_OFFSET)))/2+62 ;
	
	DWORD dwAddress = m_dwNpcBase;
	DWORD base = (DWORD)(*(LPDWORD)m_dwNpcBase);

	int iTdx = (int)(*(LPDWORD)((*(LPDWORD)dwAddress) + NPC_COOR_X));
	int iTdy = (int)(*(LPDWORD)((*(LPDWORD)dwAddress) + NPC_COOR_Y));
	int iTarX = iTdx;
	int iTarY = iTdy;
	int iTarget =  lParam/10000;
	int iSkillId = lParam % 10000;
	if(iTarget != 0){
		dwAddress = (DWORD)(m_dwBase + GAME_OFFSET_INFO) ;
		dwAddress = (DWORD)((*(LPDWORD)dwAddress) + (GAME_OFFSET_DATASIZE *(iTarget))) ;
	
		iTarX = (int)(*(LPDWORD)((*(LPDWORD)dwAddress) + NPC_COOR_X));
		iTarY = (int)(*(LPDWORD)((*(LPDWORD)dwAddress) + NPC_COOR_Y));
	}
	iTdx = iCenX + (iTarX - iTdx);
	iTdy = iCenY + (iTarY - iTdy);
	//xac dinh toa chuot
	//lay toa do character - toa do npc
	/*CString str;
	str.Format(L"%x - %x - %x",base,func,func2);
	SetWindowTextW(hwnd,str);*/
	__asm{
		PUSHAD
		PUSHFD
		mov edi, iSkillId //skill ID
		mov esi,base
		push edi
		call func
		mov eax,iTdy //mouse y
		mov ecx,iTdx //mouse x
		mov edx, [esi +0xCB0]
		push eax
		push ecx
		call func2
		POPFD
		POPAD
	}
	
	return true;
}

bool CCharacter::BackToCity(HWND hWnd){
	if(m_dwBackToCityBase==NULL){
		DWORD base = (DWORD)(m_dwBase + GAME_BUTTON_BASE_ADDRESS) ;
		if(base == NULL)
			return false;
		for(int i=0;i<21;i++){
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		}
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		m_dwBackToCityBase = (DWORD)((*(LPDWORD)base));
	}
	return Click(m_dwBackToCityBase);
}
bool CCharacter::MoveAway(HWND hWnd,int iMapIndx){
	int menu = OpenMenu(hWnd);
	if(menu!=0){
		if(!SelectedMenu(hWnd,-1))//click cáo từ
			return false;
	}
	
	return ChooseMap(hWnd,iMapIndx);
}
bool CCharacter::Move(HWND hwnd,LPARAM lParam)
{
	//	DWORD func = (DWORD)((*(LPDWORD)m_dwBase) + FUNC_MOVE_OFFSET) ;
	//	int iCenX = ((int)(*(LPDWORD)((*(LPDWORD)m_dwBase) + CENTER_X_OFFSET)))/2 ;
	//	int iCenY = ((int)(*(LPDWORD)((*(LPDWORD)m_dwBase) + CENTER_Y_OFFSET)))/2+62 ;
	////Tinh toa do x, y
	//	byte sign = lParam / 100000000;
	//	lParam = lParam % 100000000;
	//	int tdX = (int)(lParam/10000);
	//	int tdY = (int)(lParam%10000);
	//	if(sign==1)
	//		tdX = -tdX;
	//	else if(sign==2)
	//		tdY = -tdY;
	//	else if(sign==3){
	//		tdX = -tdX;
	//		tdY = -tdY;
	//	}

	//	int iDesX = iCenX+ tdX;
	//	int iDesY = iCenY + tdY;
	//	/*CString str;
	//	str.Format(L"%d - %d",iDesX,iDesY);
	//	SetWindowTextW(hwnd,str);*/
	//	__asm{
	//		PUSHAD
	//		PUSHFD
	//		mov eax,iDesY
	//		mov ecx,iDesX
	//		push eax
	//		push ecx
	//		call func
	//		POPFD
	//		POPAD
	//	}

	//////////////////////////////////////////////////////////////
	//20140418--SangDH--Cap nhat ham di chuyen moi
	int menu = OpenMenu(hwnd);
	if(menu!=0){
		if(!SelectedMenu(hwnd,-1))//click cáo từ
			return false;
	}
	DWORD ofset = (DWORD)(m_dwBase + FUNC_MOVE_OFFSET) ;
	DWORD func = (DWORD)(m_dwBase + FUNC_MOVE_FUNC) ;
	int tdX = (int)(lParam/10000);
	int tdY = (int)(lParam%10000);
	//DWORD s = func;
	ofset = (DWORD)(*(LPDWORD)ofset);
	__asm{
			PUSHAD
			PUSHFD			
			mov edi, tdY					//Y  
			mov eax, tdX					//X
			push 0x01
			push edi
			mov edi,ofset//0x03448068//[esi+0x04]
			push eax
			call func
			POPFD
			POPAD
		}
	return true;
}
bool CCharacter::RideHorse(HWND hWnd)
{

	//::PostMessage(hWnd, WM_KEYDOWN , 0x4D, 1);
	///20140510_thay đổi cách cưỡi ngựa bằng cách click vào button

		DWORD base = (DWORD)(m_dwBase + GAME_BUTTON_BASE_ADDRESS) ;
		if(base == NULL)
			return false;
		for(int i=0;i<31;i++){
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		}
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base));

	return Click(base);
}
bool CCharacter::OpenMap(HWND hWnd)
{
		DWORD base = (DWORD)(m_dwBase + GAME_BUTTON_BASE_ADDRESS) ;
		if(base == NULL)
			return false;
		for(int i=0;i<26;i++){
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		}
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base));

	return Click(base);
}
bool CCharacter::OpenFeatureDialog(HWND hWnd,int iIndex,bool open)
{
	//return OpenMap(hWnd);
	DWORD base = (DWORD)(m_dwBase + GAME_BUTTON_BASE_ADDRESS) ;
	if(base == NULL)
		return false;
	for(int i=0;i<15;i++){
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	}
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
	base = (DWORD)((*(LPDWORD)base) + (iIndex * 0x448));

	return OpenDialog(base,open);
}
bool CCharacter::OpenDialog( DWORD add,bool open){
	int opened = (int)open;
	DWORD func = (DWORD)(m_dwBase + GAME_CLICK_FUNC) ;
	__asm{
			PUSHAD
			PUSHFD
			mov ecx, add
			mov eax,opened
			mov [ecx+0x3A0],eax
			mov eax,[ecx+0x32C]
			mov ebx,[ecx+0x3A0]
			push ecx
			push 0x531
			call func
			POPFD
			POPAD		
		}
	return true;
}
bool CCharacter::Recover(HWND hWnd)
{
	DWORD dwNpcState = (DWORD)((*(LPDWORD)m_dwNpcBase) + NPC_STATE) ;
	int* state = (int*)dwNpcState;
	if(*state!=3){
		::PostMessage(hWnd, WM_KEYDOWN , 0x56, 1);
		return true;
	}
	return false;
}
bool CCharacter::KeyPress(HWND hWnd, int iKey)
{
	::PostMessage(hWnd, WM_KEYDOWN , iKey, 1);
	return true;
}

CString CCharacter::GetName(){
	char* sValue;
	DWORD dwAdd = (DWORD)((*(LPDWORD)(m_dwBase + GAME_OFFSET_NAME)) + NPC_NAME) ;
	sValue = (char*) dwAdd;
	return (CString)sValue;
}

bool CCharacter::Quit()
{
	::PostQuitMessage(0);
	//::PostMessage(hWnd, WM_QUIT , 0, 1);
	return true;
}

bool CCharacter::Click(DWORD dwMenuBase){
	DWORD func = (DWORD)(m_dwBase + GAME_CLICK_FUNC) ;
	__asm{
			PUSHAD
			PUSHFD
			mov ecx, dwMenuBase
			mov eax,[ecx+0x32C]
			mov ebx,[ecx+0x3A0]
			push ecx
			push 0x531
			call func
			POPFD
			POPAD
		}
	return true;
}
bool CCharacter::ChooseMap(HWND hwnd,int mapIndx)
{
	
	//::PostMessage(hwnd, WM_KEYDOWN , VK_TAB, 0xc00f0001);
	
	//GetWorldMapAddress();
	//::PostMessage(hwnd, WM_KEYDOWN , VK_TAB, 0xc00f0001);
	//OpenMap(hwnd);
	//::SendMessage(hwnd, WM_KEYDOWN , VK_TAB, 0xc00f0001);
	/*CString add;
	add.Format(L"%x",m_dwWorldMap);
	SetWindowTextW(hwnd,add);*/
	/*m_dwWorldMapCity = (DWORD)((*(LPDWORD)m_dwWorldMap) + GAME_MENU_OFFSET_2) ;
	m_dwWorldMapCity = (DWORD)((*(LPDWORD)m_dwWorldMapCity) + GAME_MENU_OFFSET_2) ;
	
	if(mapIndx<=15)
		m_dwWorldMapCity = (DWORD)((*(LPDWORD)m_dwWorldMapCity) + GAME_MAP_CITY_1*mapIndx) ;
	else{
		m_dwWorldMapCity = (DWORD)((*(LPDWORD)m_dwWorldMapCity) + GAME_MAP_CITY_1*15+GAME_MAP_CITY_2*(mapIndx-15)) ;
	}
	
	if(m_dwWorldMapCity!=NULL)
		Click(m_dwWorldMapCity);
	*/
	

//Click on scensce map
	DWORD addresss;

	m_dwSceneMap = (DWORD)((*(LPDWORD)m_dwWorldMap) + GAME_MENU_OFFSET_1) ;
	m_dwSceneMap = (DWORD)((*(LPDWORD)m_dwSceneMap) + GAME_MENU_OFFSET_1) ;
	m_dwSceneMap = (DWORD)((*(LPDWORD)m_dwSceneMap) + GAME_MENU_OFFSET_2) ;
	m_dwSceneMap = (DWORD)((*(LPDWORD)m_dwSceneMap) + GAME_MENU_OFFSET_1) ;

	m_dwSceneMap = (DWORD)((*(LPDWORD)m_dwSceneMap));
	//}
	DWORD func = (DWORD)(m_dwBase + GAME_CLICK_FUNC) ;
	addresss = m_dwSceneMap;
	__asm{
		PUSHAD
		PUSHFD
		mov esi, addresss//0x135B38A0 //scence map
		mov eax, [esi+0x32c]		
		mov edx, 0x1E0			// X target
		mov ecx, 0x12C			// Y target
		mov ebx,ecx
		shl ebx,0x10
		or ebx,edx
		push esi
		push 0x5f2
		call func
		POPFD
		POPAD
	}
	OpenMap(hwnd);
	return true;
}
bool CCharacter::GetWorldMapAddress(){
	DWORD base = (DWORD)(m_dwBase + GAME_MAP_BASE_ADDRESS) ;
	if(base == NULL)
		return false;
	for(int i=0;i<=164;i++){
		if(i==162||i==163)
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		/*else if(i==0){
			if(!bMapSel)
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			bMapSel = !bMapSel;
		}*/
		else
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	}
	m_dwWorldMap = base;
	return true;
}
int CCharacter::OpenMenu(HWND hwnd){
	DWORD dwOpenMenu  = (DWORD)(m_dwBase + QUEST_AWARD_OPENED) ;
	bool *opened = (bool*)dwOpenMenu;
	if(*opened){//nếu menu phan thuong
		return 2;
	}
	dwOpenMenu = (DWORD)(m_dwBase + MENU_OPENED) ;
	opened = (bool*)dwOpenMenu;
	if(*opened){//nếu mở menu chon
		return 1;
	}
	return 0;
}
bool CCharacter::SelectedMenu(HWND hwnd,int menuIndx)
{
	CString str;
	DWORD base;
	DWORD dwTemp;
	if(dwBaseSelectMenu == NULL){
		base = (DWORD)(m_dwBase + MENU_BASE_ADDRESS) ;
		if(base == NULL)
			return false;
		DWORD dwCheck = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		while(*(LPDWORD)dwCheck != NULL){		//Kiem tra offset tiep theo cua base co rong hay chua
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			dwCheck = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ; //lay offset tiep theo
		}
		//int total = 30;
		//for(int i=0;i<total;i++){
		//	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		//}
		//DWORD dwBase47 = (DWORD)(*(LPDWORD)base);
		//DWORD dwBase48 = (DWORD)(*(LPDWORD)(dwBase47 + GAME_MENU_OFFSET_1));
		//DWORD dwBase49 = NULL;
		//if(dwBase48 != NULL)
		//	dwBase49 = (DWORD)(*(LPDWORD)(dwBase48 + GAME_MENU_OFFSET_1));
		////thêm 1 offset 1
		////if(bMenuSel)//21040510
		//////if(iOS == 6164)
		////	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		////bMenuSel=!bMenuSel;
		//if(dwBase49 != NULL)
		//{
		//	base = dwBase49 + GAME_MENU_OFFSET_2 ;
		//	
		//}
		//else if(dwBase48 != NULL)
		//{
		//	base = dwBase48 + GAME_MENU_OFFSET_2 ;
		//	
		//}
		//else
		//{
		//	base = dwBase47 + GAME_MENU_OFFSET_2 ;
		//}
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		dwTemp = base;
	}
	else    //Nei luu duoc
		dwTemp = dwBaseSelectMenu;
	if(menuIndx==-1)//click nut cancel
	{
		//offset lấy button
		base = (DWORD)((*(LPDWORD)dwTemp) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base));//+MENU_DATA_SIZE;
		if(base != NULL)
			dwBaseSelectMenu = dwTemp;

		Click(base);
		if(OpenMenu(hwnd)!=0){//nếu vẫn còn mở
			return false;
		}
		return true;
	}
	if(menuIndx==-2)//click nut giao nhiem vu
	{
		base = (DWORD)((*(LPDWORD)dwTemp));
		if(base != NULL)
			dwBaseSelectMenu = dwTemp;
		Click(base);
		return true;
	}

		base = (DWORD)((*(LPDWORD)dwTemp) + GAME_MENU_OFFSET_2) ;
		
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;

		base = (DWORD)((*(LPDWORD)base));
		if(base != NULL)
			dwBaseSelectMenu = dwTemp;
	//get menu total line
	int *totalLine = (int*)(base + GAME_MENU_TOTAL_LINE);
	
	if(base==NULL)
		return false;
	int index=0;
	if(menuIndx<100){
		//
		if(*totalLine <= menuIndx ){
			index= (*totalLine- (menuIndx-1));
		}
		else
			index= *totalLine- menuIndx;
	}
	else{
		index = menuIndx - 99;
		/*if(*totalLine==1)
			index = 1;
		else
			index= (*totalLine- 1);*/
	}
	DWORD func = (DWORD)(m_dwBase + GAME_CLICK_FUNC) ;
	__asm{
			PUSHAD
			PUSHFD
			mov ebx,index
			mov ecx, base
			mov eax,[ecx+0x32C]			
			mov [ecx+0x408],ebx		//mov [ecx+0x404],ebx	
			
			push ecx
			push 0x561
			call func
			POPFD
			POPAD
		}
	m_dwMenu = base;
	
	if(OpenMenu(hwnd)!=0){//nếu vẫn còn mở
		return false;
	}
	return true;
}

bool CCharacter::ClickOnItemLink(HWND hwnd,int blockIndx)
{
	CString str;
	DWORD base;
	
	if(m_dwLink==NULL){		
		base = (DWORD)(m_dwBase + QUEST_BASE_ADDRESS) ;
		if(base == NULL)
			return false;
		
		for(int i=0;i<9;i++){
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		}
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		m_dwLink = base;
	}
	switch(blockIndx){
		case 0:
			base = (DWORD)((*(LPDWORD)m_dwLink) + GAME_MENU_OFFSET_2) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			break;
		case 1:
			base = (DWORD)((*(LPDWORD)m_dwLink) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			break;
		case 2:
			base = (DWORD)((*(LPDWORD)m_dwLink) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			break;
		case 3:
			
			base = (DWORD)((*(LPDWORD)m_dwLink) + GAME_MENU_OFFSET_1) ;
			
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			break;
		case 4:
			base = (DWORD)((*(LPDWORD)m_dwLink) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			break;
	}
	base = (DWORD)((*(LPDWORD)base));
	if(base==NULL)
		return false;
	//Click(base);
	//str.Format(L"%x",base);
	//SetWindowTextW(hwnd, str);
	DWORD func = (DWORD)(m_dwBase + GAME_CLICK_FUNC) ;
	__asm{
		PUSHAD
		PUSHFD
		mov esi, base		//Button Address
		mov eax, [esi+0x32c]		
		mov edx, 0x0			//cot
		mov edi, 0x0			//dong
		mov ebx,edi
		shl ebx,0x10
		or ebx,edx
		push esi
		push 0x521
		call func
		POPFD
		POPAD
		}
	//SetWindowTextW(hwnd, L"aa");
	return true;
}

bool CCharacter::ClickOnItem(HWND hwnd,int Indx){
	int iRow = (int)(Indx / 9);
	int iCol = Indx % 9;

	//get base address package
	DWORD base;
	//if(m_dwPackage==NULL){
		//KeyPress(hwnd,VK_F2);
		//::PostMessage(hwnd, WM_KEYDOWN , VK_F2, 1);
		OpenFeatureDialog(hwnd,1);
		base = (DWORD)(m_dwBase + PACKAGE_BASE_ADDRESS) ;
		
		for(int i=0;i<162/*158*/;i++){
			 base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		}
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		m_dwPackage=base;
		//KeyPress(hwnd,VK_F2);
		//::PostMessage(hwnd, WM_KEYDOWN , VK_F2, 1);
		OpenFeatureDialog(hwnd,1,false);
	//}

	base = (DWORD)(*(LPDWORD)m_dwPackage);
	//click item in package
	DWORD func = (DWORD)(m_dwBase + GAME_CLICK_FUNC) ;

	__asm{
		PUSHAD
		PUSHFD
		mov esi, base		//Button Address
		mov eax, [esi+0x32c]		
		mov edx, iCol			//cot
		mov edi, iRow			//dong
		mov ebx,edi
		shl ebx,0x10
		or ebx,edx
		push esi
		push 0x521
		call func
		POPFD
		POPAD
		}
	
	//SetWindowTextW(hwnd, L"aa");
	return true;
}
bool CCharacter::ClickOnLink(HWND hwnd,int blockIndx)
{
	int menu = OpenMenu(hwnd);
	if(menu==1){
		if(!SelectedMenu(hwnd,-1))//click cáo từ
			return false;
	}
	CString str;
	DWORD base;
	
	if(m_dwLink==NULL){		
		base = (DWORD)(m_dwBase + QUEST_BASE_ADDRESS) ;
		if(base == NULL)
			return false;
		
		for(int i=0;i<9;i++){
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		}
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		m_dwLink = base;
	}
	switch(blockIndx){
		case 0:
			base = m_dwLink;
			break;
		case 1:
			base = (DWORD)((*(LPDWORD)m_dwLink) + GAME_MENU_OFFSET_1) ;
			break;
		case 2:
			base = (DWORD)((*(LPDWORD)m_dwLink) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			break;
		case 3:
			base = (DWORD)((*(LPDWORD)m_dwLink) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			break;
		case 4:
			base = (DWORD)((*(LPDWORD)m_dwLink) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
			break;
	}
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	if(bLinkSel){
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	}
	bLinkSel=!bLinkSel;
	base = (DWORD)((*(LPDWORD)base));
	/*str.Format(L"%x",base);
	SetWindowTextW(hwnd, str);*/
	if(base==NULL)
		return false;
	//Click(base);
	
	DWORD address = (DWORD)(m_dwBase + QUEST_LINK_FUNCTION) ;
	DWORD func = (DWORD)(m_dwBase + QUEST_LINK_FUNCTION1) ;
	__asm{
		PUSHAD
		PUSHFD
		mov esi, base		//Button Address
		push edx
		mov	ecx,address
		mov edx,esi
		call func
		POPFD
		POPAD
		}
	//SetWindowTextW(hwnd, L"aa");
	return true;
}
bool CCharacter::ChooseAward(HWND hwnd,int Indx)
{
	DWORD base;
	DWORD button;
	base = (DWORD)(m_dwBase + QUEST_AWARD_BASE_ADDRESS) ;		
	if(base == NULL)
		return false;

	if(iOS==5164 || iOS==5264){//WinXP 64
		for(int i=0;i<14;i++){
			if(i==0||i==6||i==7||i==11)
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
			else
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		}
		
		//button = (DWORD)((*(LPDWORD)base) + 0xc34 ) ;
		if(Indx>=1)
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		if(Indx>=2)
			base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;

		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
		base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	}
	else{//các win khác
		for(int i=0;i<15;i++){
			if(i==0||i==6||i==7||i==11||i==14)
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
			else
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		}
		
		//button = (DWORD)((*(LPDWORD)base) + 0xc34 ) ;
		if(Indx>1)
			base = (DWORD)((*(LPDWORD)base) +GAME_MENU_OFFSET_1+ ((Indx-1)*QUEST_AWARD_SIZE) ) ;
	}
	
	base = (DWORD)((*(LPDWORD)base));	
	//button = (DWORD)((*(LPDWORD)button) - 0x1DF8)  ;

	if(base==NULL)
		return false;
	/*CString str;
	str.Format(L"Award: %x",button);
	SetWindowTextW(hwnd, str);*/
	Click(base);//chọn phần thưởng

	RecivedAward(hwnd);
	//SelectedMenu(hwnd,-2);//giao nhiem vu
	//Click(button);
	//DWORD dwOpenMenu = (DWORD)((*(LPDWORD)m_dwBase) + QUEST_AWARD_OPENED) ;
	//bool *opened = (bool*)dwOpenMenu;
	if(OpenMenu(hwnd)!=0){//nếu vẫn còn mở
		return false;
	}
	return true;
}


//=================NghiaLP=====================
///////////////tab tổ đội/////////////////////

bool CCharacter::AgreeParty(HWND hwnd,bool agree)
{
	GetButtonPartyAddress();
	if(dwAgreeButtonParty==NULL)
		return false;
	if(agree)
		Click(dwAgreeButtonParty);
	else
		Click(dwDenyButtonParty);
	return true;
}


void CCharacter::GetButtonPartyAddress(){
	if(dwAgreeButtonParty!=NULL)
		return;

	DWORD base;
	base = (DWORD)(m_dwBase + OFSET_OPEN_DIALOG_PARTY) ;
	if(base == NULL)
		return;
	
	for(int i = 0; i<14; i++)
	{
			if(i != 12)
			{
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1);
			}
			else
			{
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2);
			}
			if(base == 0)
				return;
	
	}
	dwAgreeButtonParty = (DWORD)(*(LPDWORD)base);   //button đồng ý
	dwDenyButtonParty = dwAgreeButtonParty +MENU_DATA_SIZE;

}

void CCharacter::CreatePartyInList(HWND hWnd,DWORD dwMenuBase)
{/*
	CString str1;
	str1.Format(L"%x",dwMenuBase);
	SetWindowTextW(hWnd,str1);*/
	DWORD func =  (DWORD)(m_dwBase + OFFSET_FUNTION_INVITE) ;
	//DWORD f = 0x566DD9C8;
	__asm{
			PUSHAD
			PUSHFD
			mov esi, dwMenuBase
			mov edx,esi
			call func
			POPFD
			POPAD		
		}
	
}

void CCharacter::CheckAutoParty(HWND hWnd,byte check)
{
	//OpenFeatureDialog(hWnd,5,(bool)check);
	//lấy checkbox address
	GetCheckPartyAddress(hWnd);
	if(dwCheckInviteParty==NULL)
		return;

	DWORD dwCheckBase = dwCheckInviteParty;
	if(check>1)
		dwCheckBase =  dwCheckBase + MENU_DATA_SIZE;


	OpenDialog(dwCheckBase,check == 1 || check == 3);

	

}
void CCharacter::GetCheckPartyAddress(HWND hwnd){
	if(dwCheckInviteParty!=NULL)
		return;
	//KeyPress(hwnd,0x50);
	OpenFeatureDialog(hwnd,5);

	DWORD base;
	base = (DWORD)(m_dwBase + GAME_CHECK_PARTY) ;
	if(base == NULL)
		return;
	
	for(int i = 0; i<173; i++)
	{
			if(i != 163)
			{
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1);
			}
			else
			{
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2);
			}
			if(base == 0)
				return;
	
	}
	//base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1);
	//ReadProcessMemory(m_hHandle, (void*)base1, &base1, (DWORD)sizeof(base1), 0);
	dwCheckInviteParty = (DWORD)(*(LPDWORD)base);   //Check lời mời
	//dwCheckRequestParty = base + GAME_CHECK_BOOL_PARTY;   //Check Đơn xin
	//KeyPress(hwnd,0x50);
	OpenFeatureDialog(hwnd,5,false);
}

//============Van Tieu=============================

bool CCharacter::RecivedAward(HWND hWnd)
{
	DWORD base = (DWORD)(m_dwBase + QUEST_AWARD_BASE_ADDRESS);
	base = (DWORD)(*(LPDWORD)base);
	if(base == NULL)
		return false;
	base = base + GAME_MENU_OFFSET_2 ;
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
	base = (DWORD)(*(LPDWORD)base);
	
	Click(base);
	return true;
}
///==================================================



/////////////////////////////Dieu Doi///////////////
bool CCharacter::OpenDialogQuest()
{
	DWORD dwbase = (DWORD)(m_dwBase + QUEST_BASE_ADDRESS) ;
	DWORD getbase = dwbase;
	for(int i = 0; i < 13; i++)
	{
		if(i == 3 || i == 4)
			dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_2);
		else
			dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1);
	}
	dwbase = (DWORD)(*(LPDWORD)dwbase);
	if(dwbase == NULL)
		return false;
	return Click(dwbase);
}

bool CCharacter::ChooseQuest(int indx)
{
	DWORD dwbase = (DWORD)(m_dwBase + GAME_CHECK_PARTY) ;
	if(dwbase == NULL)
		return false;
	/*for(int i = 0; i < 163; i++)
	{
		if(i == 158)
			dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_2);
		else
			dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1);

	}*/

	DWORD dwTemp;
	if(dw_GetBase == NULL)
	{
		DWORD dwbase = (DWORD)(m_dwBase + GAME_CHECK_PARTY) ;
		for(int i = 0; i < 162; i++)
		{
			dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1);
		}
		if(bSendTeam && iOS == WIN_7_64)//21040510
			dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1) ;
		bSendTeam=!bSendTeam;
		/*DWORD dwbase6 = (DWORD)(*(LPDWORD)dwbase);
		DWORD dwbase7 = (DWORD)(*(LPDWORD)(dwbase6 + GAME_MENU_OFFSET_1));
		DWORD dwbase8 = NULL;
		if(dwbase8 != NULL)
			dwbase8 = (DWORD)(*(LPDWORD)(dwbase7 + GAME_MENU_OFFSET_1));
		DWORD dwbase9 = NULL;
		if(dwbase8 != NULL)
			dwbase9 = (DWORD)(*(LPDWORD)(dwbase8 + GAME_MENU_OFFSET_1));
		
		if(dwbase9 != NULL)
			dwbase = dwbase8 + GAME_MENU_OFFSET_2;
		else if(dwbase8 != NULL)
			dwbase = dwbase7 + GAME_MENU_OFFSET_2;
		else
			dwbase = dwbase6 + GAME_MENU_OFFSET_2;*/
		dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_2);
		dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1);
		dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1);
		dwTemp = dwbase;
	}
	else 
		dwTemp = dw_GetBase;
	if(indx == -3)   //Chon Button "x"(close)
	{
		DWORD bntclose = (DWORD)(*(LPDWORD)dwTemp);
		if(bntclose != NULL)
			dw_GetBase = dwTemp;
		return Click(bntclose);

	}
	if(indx == -1)  ///Chon Button Click "Phai"
	{
		DWORD bntbase = (DWORD)((*(LPDWORD)dwTemp) + GAME_MENU_OFFSET_1) ;
		bntbase = (DWORD)((*(LPDWORD)bntbase) + GAME_MENU_OFFSET_1);
		bntbase = (DWORD)((*(LPDWORD)bntbase) + GAME_MENU_OFFSET_1) ;
		bntbase = (DWORD)((*(LPDWORD)bntbase) + GAME_MENU_OFFSET_1) ;
		bntbase = (DWORD)((*(LPDWORD)bntbase) + GAME_MENU_OFFSET_1) ;
		bntbase = (DWORD)((*(LPDWORD)bntbase) + GAME_MENU_OFFSET_1) ;
		bntbase = (DWORD)((*(LPDWORD)bntbase) + GAME_MENU_OFFSET_1) ;
		bntbase = (DWORD)((*(LPDWORD)bntbase) + GAME_MENU_OFFSET_1);
		bntbase = (DWORD)(*(LPDWORD)bntbase);
		if(bntbase != NULL)
			dw_GetBase = dwTemp;
		return Click(bntbase);
	}
	
	if(indx == -2)    //Xac Nhan Dieu Doi
	{
		DWORD bntAccept = (DWORD)(m_dwBase + OFFSET_BUTTON_ACCEPT) ;
		if(bntAccept == NULL)
			return false;
		bntAccept = (DWORD)((*(LPDWORD)bntAccept) + GAME_MENU_OFFSET_2) ;
		bntAccept = (DWORD)((*(LPDWORD)bntAccept) + GAME_MENU_OFFSET_1) ;
		bntAccept = (DWORD)(*(LPDWORD)bntAccept);
		return Click(bntAccept);
	}

	DWORD choosebase = (DWORD)((*(LPDWORD)dwTemp) + GAME_MENU_OFFSET_1) ;
	choosebase = (DWORD)((*(LPDWORD)choosebase) + GAME_MENU_OFFSET_1);
	choosebase = (DWORD)((*(LPDWORD)choosebase) + GAME_MENU_OFFSET_2);
	choosebase = (DWORD)((*(LPDWORD)choosebase) + GAME_MENU_OFFSET_2) ;
	choosebase = (DWORD)((*(LPDWORD)choosebase) + GAME_MENU_OFFSET_1) ;
	choosebase = (DWORD)((*(LPDWORD)choosebase) + GAME_MENU_OFFSET_2) ;
	
	choosebase = (DWORD)((*(LPDWORD)choosebase) + (indx * MENU_DATA_SIZE));
	if(choosebase != NULL)
		dw_GetBase = dwTemp;
	return Click(choosebase);
	
}

DWORD CCharacter::GetBaseText(int indx)
{
	if(indx == -1)  //Dang o gia vien cua ai do
	{
		DWORD dwName = (DWORD)(m_dwBase + QUEST_BASE_ADDRESS) ;
		for(int i = 0; i < 7; i++)
		{
			if(i == 3 || i == 4 )
					dwName = (DWORD)((*(LPDWORD)dwName) + GAME_MENU_OFFSET_2);
			else
					dwName = (DWORD)((*(LPDWORD)dwName) + GAME_MENU_OFFSET_1);
		}
		dwName = (DWORD)((*(LPDWORD)dwName) +	OFFSET_NAME_PARTY);
		dwName =   (DWORD)(*(LPDWORD)dwName);
		return dwName;
		
	}
	//DWORD dwbase;
	
	DWORD dwTemp;
	if(dw_GetBase == NULL)
	{	
		DWORD dwbase = (DWORD)(m_dwBase + GAME_CHECK_PARTY) ;
		for(int i = 0; i < 162; i++)
		{
			dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1);
		}
		if(bSendTeam && iOS == WIN_7_64)//21040510
			dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1) ;
		bSendTeam=!bSendTeam;
		/*if((*(LPDWORD)dwbase) == NULL)
			return false;*/
		/*DWORD dwbase6 = (DWORD)(*(LPDWORD)dwbase);
		DWORD dwbase7 = (DWORD)(*(LPDWORD)(dwbase6 + GAME_MENU_OFFSET_1));
		DWORD dwbase8 = NULL;
		if(dwbase8 != NULL)
			dwbase8 = (DWORD)(*(LPDWORD)(dwbase7 + GAME_MENU_OFFSET_1));
		DWORD dwbase9 = NULL;
		if(dwbase8 != NULL)
			dwbase9 = (DWORD)(*(LPDWORD)(dwbase8 + GAME_MENU_OFFSET_1));
		
		if(dwbase9 != NULL)
			dwbase = dwbase8 + GAME_MENU_OFFSET_2;
		else if(dwbase8 != NULL)
			dwbase = dwbase7 + GAME_MENU_OFFSET_2;
		else
			dwbase = dwbase6 + GAME_MENU_OFFSET_2;*/
		dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_2);
		dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1);
		dwbase = (DWORD)((*(LPDWORD)dwbase) + GAME_MENU_OFFSET_1);
		dwTemp = dwbase;
	}
	else 
		dwTemp = dw_GetBase;
	DWORD dw_Base;
	//if(indx == 2)  //Thong bao trang thai dieu doi
	//{
	//	
	//	dw_Base = (DWORD)((*(LPDWORD)dwTemp) + GAME_MENU_OFFSET_1);
	//	dw_Base = (DWORD)((*(LPDWORD)dw_Base) + GAME_MENU_OFFSET_1);
	//	dw_Base = (DWORD)((*(LPDWORD)dw_Base) + GAME_MENU_OFFSET_2);
	//	dw_Base = (DWORD)((*(LPDWORD)dw_Base) + GAME_MENU_OFFSET_2);
	//	dw_Base = (DWORD)((*(LPDWORD)dw_Base) + OFFSET_GET_TEXT_2);
	//	dw_Base = (DWORD)(*(LPDWORD)dw_Base);
	//	DWORD dwCheckBase = (DWORD)(*(LPDWORD)dw_Base);
	//	if(dwCheckBase != NULL)
	//		dw_GetBase = dwTemp;
	//	//return dw_Base;
	//}
	//else //indx = 0:The Luc; 1 : Thu ve Phai; 4: Truy na; 5: van tieu; 10(A): Thanh van Phong;    D(13) : Bi Canh; 24(0x18): thong bao
	//{
		dw_Base = (DWORD)((*(LPDWORD)dwTemp) + GAME_MENU_OFFSET_1);
		dw_Base = (DWORD)((*(LPDWORD)dw_Base) + GAME_MENU_OFFSET_1);
		dw_Base = (DWORD)((*(LPDWORD)dw_Base) + GAME_MENU_OFFSET_1);
		dw_Base = (DWORD)((*(LPDWORD)dw_Base) + GAME_MENU_OFFSET_1);
		dw_Base = (DWORD)((*(LPDWORD)dw_Base) - (OFFSET_GET_TEXT + indx * MENU_DATA_SIZE));
		dw_Base = (DWORD)(*(LPDWORD)dw_Base);
		DWORD dwCheckBase = (DWORD)(*(LPDWORD)dw_Base);
		if(dwCheckBase != NULL)
			dw_GetBase = dwTemp;
		//return dw_Base;
	//}
	return dw_Base;
}


//////base Lich Su Kien==== Kiem tra do hoanh thanh cua Nhiem vu
DWORD CCharacter::CheckFinishQuest(int idx)
{
	DWORD dwBase = NULL;
	if(idx == 1)
	//if(dwCheckFinish == NULL)  //Neu da lay dia chi roi khong can phai lay lai
	{
		OpenListEvent();
		OpenListEvent();
		dwBase = (DWORD)(m_dwBase + GAME_MAP_BASE_ADDRESS);
		if(dwBase == NULL)
			return 0;
		for(int i = 0; i < 163; i++)
		{
			dwBase = (DWORD)((*(LPDWORD)dwBase) + GAME_MENU_OFFSET_1);
		}
		DWORD dwSaveBase = dwBase;
		int cnt=0;
		for(int i = 0; i < 11; i++){
			if(i == 0 || i == 6 || i == 8 || i == 10)
				dwBase = (DWORD)((*(LPDWORD)dwBase) + GAME_MENU_OFFSET_2);
			else
				dwBase = (DWORD)((*(LPDWORD)dwBase) + GAME_MENU_OFFSET_1);
			if((*(LPDWORD)dwBase) == NULL)
			{
				if(cnt>=3)
					return NULL;
				dwBase = (DWORD)((*(LPDWORD)dwSaveBase) + GAME_MENU_OFFSET_1);
				i = 0;
				cnt++;
			}
		}
		dwBase = (DWORD)((*(LPDWORD)dwBase) + GAME_MENU_OFFSET_2);
		dwBase = (DWORD)((*(LPDWORD)dwBase) + GAME_MENU_OFFSET_1);
		dwCheckFinish = dwBase;
	}
	else
		dwCheckFinish = (DWORD)((*(LPDWORD)dwCheckFinish) + GAME_MENU_OFFSET_1);	

	dwBase = (DWORD)((*(LPDWORD)dwCheckFinish) + GAME_MENU_OFFSET_2);
	dwBase = (DWORD)((*(LPDWORD)dwBase) + GAME_MENU_OFFSET_1);
	dwBase = (DWORD)((*(LPDWORD)dwBase) + GAME_MENU_OFFSET_2);
	dwBase = (DWORD)((*(LPDWORD)dwBase) + GAME_MENU_OFFSET_1);
	dwBase = (DWORD)((*(LPDWORD)dwBase) + GAME_MENU_OFFSET_1);
	dwBase = (DWORD)((*(LPDWORD)dwBase) + OFFSET_NAME_PARTY);

	dwBase = (DWORD)(*(LPDWORD)dwBase);
	
	return dwBase;
}


bool CCharacter::OpenListEvent()
{
		DWORD base = (DWORD)(m_dwBase + GAME_BUTTON_BASE_ADDRESS) ;
		if(base == NULL)
			return false;
		for(int i=0;i<37;i++){
			if(i == 26 || i == 27)
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_2) ;
			else
				base = (DWORD)((*(LPDWORD)base) + GAME_MENU_OFFSET_1) ;
		}
		base = (DWORD)((*(LPDWORD)base));
	return Click(base);
}