/*
20140611_SangDH - Thay đổi hàm constructor: cho truyền vào base address từ ngoài

*/

#include "stdafx.h"
#include "NPC.h"
#include "GameInfo.h"
#include "Serialize.h"
#include <string>



CNPC::CNPC(HANDLE hWnd,DWORD baseAdress)
{
	m_szName=_T("");
	m_iIndex=0;
	m_iCurHP=0;
	m_iTdX=0;
	m_iTdY=0;
	m_iType=0;
	m_iElement=0;
	m_iFaction=0;
	m_iParty=0;
	m_iColor=0;
	m_siPkState=0;
	m_iState=0;
	this->hWnd = hWnd;
	m_szFamily = L"";
	
	////Get Base Address win64
	//if(iOS==6164||iOS==5164||iOS==5264)
	//	ReadProcessMemory(hWnd, (void*)GAME_BASE_ADDRESS, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	////Get Base Address win32
	//else{
	//	if(iOS==5132){
	//		ReadProcessMemory(hWnd, (void*)GAME_BASE_ADDRESS_XP_32, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	//	}
	//	else if(iOS==6132){
	//		ReadProcessMemory(hWnd, (void*)GAME_BASE_ADDRESS_32, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	//	}
	//	dwGameBaseAddress = dwGameBaseAddress + GAME_OFFSET_32;
	//	ReadProcessMemory(hWnd, (void*)dwGameBaseAddress, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	//}
	////Get info
	//dwGameBaseAddress = dwGameBaseAddress + GAME_OFFSET_1;
	//ReadProcessMemory(hWnd, (void*)dwGameBaseAddress, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
		


	dwGameBaseAddress= baseAdress;
	//Get player Info address
	dwGameBaseAddress = dwGameBaseAddress + GAME_OFFSET_INFO;
	ReadProcessMemory(hWnd, (void*)dwGameBaseAddress, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
	
}

CNPC::~CNPC()
{
	m_szName=L"";
	m_iIndex=0;
	m_iCurHP=0;
	m_iTdX=0;
	m_iTdY=0;
	m_iType=0;
	m_iElement=0;
	m_iFaction=0;
	m_iParty=0;
	m_iColor=0;
	m_siPkState=0;
	m_iState=0;
	m_szFamily = L"";
}

void  CNPC::GetInfo(){
	DWORD dwStartNpc = dwGameBaseAddress + GAME_OFFSET_DATASIZE * m_iIndex;
	ReadProcessMemory(hWnd, (void*)dwStartNpc, &dwStartNpc, (DWORD)sizeof(dwStartNpc), 0);
	DWORD dwNpc = dwStartNpc + NPC_TITLE;		
	char sValue[NPC_NAME_SIZE];
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(hWnd, (void*)dwNpc, &sValue, (DWORD)sizeof(sValue), 0);
	m_szName = (CString)sValue;
	wstring wsResult =XMLSerialized::UTF8ToUnicode( (string)CW2A( m_szName.GetString() ));
	m_szName = wsResult.c_str();
	//CString a = m_szName;
	dwNpc = dwStartNpc + NPC_FAMILY;
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(hWnd, (void*)dwNpc, sValue, (DWORD)sizeof(sValue), 0);
	m_szFamily = (CString)sValue;
	dwNpc = dwStartNpc + NPC_CURRENT_HP;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_iCurHP, (DWORD)sizeof(m_iCurHP), 0);
	dwNpc = dwStartNpc + NPC_COOR_X;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_iTdX, (DWORD)sizeof(m_iTdX), 0);
	dwNpc = dwStartNpc + NPC_COOR_Y;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_iTdY, (DWORD)sizeof(m_iTdY), 0);
	dwNpc = dwStartNpc + NPC_TYPE;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_iType, (DWORD)sizeof(m_iType), 0);
	

	// Get npc faction
	dwNpc = dwStartNpc + NPC_FACTION;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_iFaction, (DWORD)sizeof(m_iFaction), 0);
	// Get npc element
	dwNpc = dwStartNpc + NPC_ELEMENT;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_iElement, (DWORD)sizeof(m_iElement), 0);
	if(m_iElement==0){
		dwNpc = dwStartNpc + NPC_ELEMENT_2;
		ReadProcessMemory(hWnd, (void*)dwNpc, &m_iElement, (DWORD)sizeof(m_iElement), 0);
	}
	// Get npc pk state
	dwNpc = dwStartNpc + NPC_PK_STATE;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_siPkState, (DWORD)sizeof(m_siPkState), 0);
	// Get npc state
	dwNpc = dwStartNpc + NPC_STATE;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_iState, (DWORD)sizeof(m_iState), 0);
	// Get npc party
	dwNpc = dwStartNpc + NPC_PARTY;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_iParty, (DWORD)sizeof(m_iParty), 0);
	// Get npc color
	dwNpc = dwStartNpc + NPC_NAME_COLOR;
	ReadProcessMemory(hWnd, (void*)dwNpc, &m_iColor, (DWORD)sizeof(m_iColor), 0);
}