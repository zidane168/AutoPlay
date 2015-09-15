#pragma once


// CCharacter

class   __declspec(dllexport)  CItem 
{
	
public:
	CItem(HANDLE ,int );
	virtual ~CItem();
	void GetInfo();

	CString m_szName;
	int m_iIndex;
	int m_iTdX;
	int m_iTdY;
	int m_iType;
	
private:
	DWORD m_dwItemBaseAddress;
	HANDLE m_hWnd;
};