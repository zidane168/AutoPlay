#pragma once

class CCharacter
{
public:	
	CCharacter();
	~CCharacter(void);

	int m_iHP;
	int m_iMP;
	int m_iMaxHP;
	int m_iMaxMP;	
	int m_iCurXPosPixel;
	int m_iCurYPosPixel;
	int m_isize;
	int m_iID;
	
	int m_iCurXPosMap;
	int m_iCurYPosMap;
	double Distance(const CCharacter &character);
	DWORD m_dwPhai;

	CString m_sName;	
	
	CCharacter GetIndexOfMinDistance(const CArray<CCharacter, CCharacter&> &Characters);
	CCharacter GetIndexOfSecMinDistance(CArray<CCharacter, CCharacter&>& Characters);
		
	void Swap(CCharacter&, CCharacter&);
	void DistanceAscending(CArray<CCharacter, CCharacter&>& Characters);
};
