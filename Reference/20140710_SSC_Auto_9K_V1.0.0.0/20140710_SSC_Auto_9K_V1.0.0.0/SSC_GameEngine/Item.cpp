#include "stdafx.h"
#include "Item.h"
#include "GameInfo.h"


CItem::CItem(HANDLE hWnd,int index)
{
	m_szName=L"";
	m_iIndex=index;
	m_iTdX=0;
	m_iTdY=0;
	m_iType=0;
	m_hWnd = hWnd;
	//Get base address
	ReadProcessMemory(hWnd, (void*)ITEM_BASE_ADDRESS, &m_dwItemBaseAddress, (DWORD)sizeof(m_dwItemBaseAddress), 0);
	//base address + offset
	m_dwItemBaseAddress = m_dwItemBaseAddress + ITEM_DATA_SIZE * (DWORD) index;
}

CItem::~CItem()
{
	m_szName=L"";
	m_iIndex=-1;
	m_iTdX=0;
	m_iTdY=0;
	m_iType=0;
}

void  CItem::GetInfo(){
	DWORD dwItem = m_dwItemBaseAddress + ITEM_NAME;		
	char sValue[ITEM_NAME_SIZE];
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(m_hWnd, (void*)dwItem, &sValue, (DWORD)sizeof(sValue), 0);
	m_szName = (CString)sValue;
	dwItem = m_dwItemBaseAddress + ITEM_COOR_X;
	ReadProcessMemory(m_hWnd, (void*)dwItem, &m_iTdX, (DWORD)sizeof(m_iTdX), 0);
	dwItem = m_dwItemBaseAddress + ITEM_COOR_Y;
	ReadProcessMemory(m_hWnd, (void*)dwItem, &m_iTdY, (DWORD)sizeof(m_iTdY), 0);
}