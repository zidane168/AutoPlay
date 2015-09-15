#pragma once


// CCharacter

class   __declspec(dllexport)  CNPC 
{
	
public:
	CNPC(HANDLE,DWORD );
	virtual ~CNPC();
	void GetInfo();

	CString m_szName;
	CString m_szFamily; //gia tộc
	int m_iIndex;
	int m_iCurHP;
	int m_iTdX;
	int m_iTdY;
	int m_iType; //0: Monster | 1:player | 3:NPC |5: vòng xoay
	int m_iState; //1: Stand, 2: Run, 3: Sit, 7: Attack, 15:Die
	int m_iElement;
	int m_iFaction;
	int m_iParty;
	int m_iColor;
	byte m_siPkState;
	
private:
	DWORD dwGameBaseAddress;
	HANDLE hWnd;
};