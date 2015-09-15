#include "stdafx.h"
#include "Character.h"
#include "GameInfo.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/************************************************************************/
CCharacter::CCharacter()
		: m_iHP(0)
		, m_iMP(0)
		, m_iMaxHP(0)
		, m_iMaxMP(0)
		, m_iCurXPosPixel(0)
		, m_iCurYPosPixel(0)
		, m_iID(0)
		, m_dwPhai(0)
		, m_iCurXPosMap(0)
		, m_iCurYPosMap(0)
{
	m_sName = "";
}

/************************************************************************/
CCharacter::~CCharacter(void)
{

}


/************************************************************************
VịLH - ReadCharacterName - 10/02/2013
	Trả về giá trị tương ứng => DWORD PTR SS:[dwAddress]
return:
	- CString: tên nhân vật tương ứng với handle
*************************************************************************/
CString CCharacter::ReadCharacterName()
{
	DWORD dwNpcBaseAddress;
	ReadProcessMemory(m_hHandle, (void*)NPC_INFO_BASEADDRESS, &dwNpcBaseAddress, (DWORD)sizeof(dwNpcBaseAddress), 0);	

	// Current Player
	DWORD dwBaseAddressName = dwNpcBaseAddress + NPC_OFFSET_B5C0 * 1 + NPC_OFFSET_15D0;		

	char sValue[NPC_NAME_SIZE];
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(m_hHandle, (void*)dwBaseAddressName, &sValue, (DWORD)sizeof(sValue), 0);
	m_sName =  (CString)sValue;
	return m_sName;
}

/************************************************************************
VịLH - GetPlayerType - 07/05/2013
	Trả về Loại nhân vật (Giáp Sĩ, Đạo Sĩ, Dị Nhân, quái,...)
return:
	DWORD: loại nhân vật (môn phái)
*************************************************************************/
DWORD CCharacter::GetPlayerType(void)
{	
	DWORD dwNpcBaseAddress;
	ReadProcessMemory(m_hHandle, (void*)NPC_INFO_BASEADDRESS, &dwNpcBaseAddress, (DWORD)sizeof(dwNpcBaseAddress), 0);	

	// Current Player
	DWORD dwBaseAddressName = dwNpcBaseAddress + NPC_OFFSET_B5C0 * 1 + NPC_OFFSET_15D0;		

	// Môn phái
	DWORD dwPlayerType = dwBaseAddressName + NPC_TYPE;
	DWORD dwValue;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerType, &dwValue, (DWORD)sizeof(dwValue), 0);

	return dwValue;
}


/************************************************************************
VịLH - GetCurrentPlayerInfo - 07/05/2013
	Lấy thông tin nhân vật (Tên, HP, MP, MaxHP, MaxMP, Phái, x, y,...)
*************************************************************************/
void CCharacter::GetCurrentPlayerInfo()
{
	DWORD dwNpcBaseAddress;
	ReadProcessMemory(m_hHandle, (void*)NPC_INFO_BASEADDRESS, &dwNpcBaseAddress, (DWORD)sizeof(dwNpcBaseAddress), 0);	

	// Current Player
	DWORD dwBaseAddressName = dwNpcBaseAddress + NPC_OFFSET_B5C0 * 1 + NPC_OFFSET_15D0;		
	m_iID = 1;

	char sValue[NPC_NAME_SIZE];
	memset(sValue, 0x0, sizeof(sValue));
	ReadProcessMemory(m_hHandle, (void*)dwBaseAddressName, &sValue, (DWORD)sizeof(sValue), 0);
	m_sName =  (CString)sValue;

	DWORD dwPlayerInfo;

	// CurrentHP
	dwPlayerInfo = dwBaseAddressName - NPC_CURRENT_HP;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iHP, (DWORD)sizeof(m_iHP), 0);

	// CurrentMP
	dwPlayerInfo = dwBaseAddressName - NPC_CURRENT_MP;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iMP, (DWORD)sizeof(m_iMP), 0);

	// MaxHP
	dwPlayerInfo = dwBaseAddressName - NPC_MAX_HP;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iMaxHP, (DWORD)sizeof(m_iMaxHP), 0);

	// MaxMP
	dwPlayerInfo = dwBaseAddressName - NPC_MAX_MP;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iMaxMP, (DWORD)sizeof(m_iMaxMP), 0);

	// Current XPos;
	dwPlayerInfo = dwBaseAddressName - NPC_XPOS; 
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iCurXPosPixel, (DWORD)sizeof(m_iCurXPosPixel), 0);
	m_iCurXPosMap = m_iCurXPosPixel / 256;

	// Current YPos;
	dwPlayerInfo = dwBaseAddressName - NPC_YPOS; 
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iCurYPosPixel, (DWORD)sizeof(m_iCurYPosPixel), 0);
	m_iCurYPosMap = m_iCurYPosPixel / 512;

	// Phái;
	dwPlayerInfo = dwBaseAddressName + NPC_TYPE;
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_dwPhai, (DWORD)sizeof(m_dwPhai), 0);
}

/************************************************************************
VịLH - Distance - 21/05/2013
	Trả về khoảng cách hai điểm
params:
	- const CCharacter&: NPC cần so sánh
return:
	double: khoảng cách giữa hai điểm
*************************************************************************/
double CCharacter::Distance(const CCharacter& character)
{
	double rel = sqrtf (  (m_iCurXPosPixel - character.m_iCurXPosPixel) * (m_iCurXPosPixel- character.m_iCurXPosPixel) 
						+ (m_iCurYPosPixel - character.m_iCurYPosPixel) * (m_iCurYPosPixel - character.m_iCurYPosPixel));
	return rel;
}

/************************************************************************
VịLH - GetIndexOfMinDistance - 21/05/2013
	Trả về NPC có khoảng cách nhỏ nhất giữa nhân vật đang xét và mảng nhân vật
params:
	- const CArray<CCharacter, CCharacter&> &: Mảng NPC cần xét
return:
	CChracter: NPC có khoảng cách nhỏ nhất
*************************************************************************/

CCharacter CCharacter::GetIndexOfMinDistance(const CArray<CCharacter, CCharacter&> &Characters)
{
	CCharacter character;

	if (Characters.GetCount() == 0)
		return character;		// character.m_iID = 0;

	double min = Distance(Characters[0]);
	double distance;
	character = Characters[0];	

	for (int i=1; i < Characters.GetCount(); i++)
	{		
		distance = Distance(Characters[i]); 
		if (min > distance)
		{
			min = distance;
			character = Characters[i];
		}
	}

	return character;
}

/************************************************************************
VịLH - CheckPointBelongCircle - 22/05/2013
	Kiểm tra một điểm nằm trong hình tròn
params:
	- POINT: điểm thứ 1 cần xét 
	- int: bán kính
	- enemy: chứa điểm thứ 2 cần xét
return:
CChracter: NPC có khoảng cách nhỏ nhất
*************************************************************************/
int CCharacter::CheckPointBelongCircle(POINT point, int radius, const CCharacter& enemy)
{
	// tâm I(x,y),ban kình r, diem A(x1,y1)
	// (x1-x)^2+(y1-y)^2 - r^2
	// >0 nam ngoai duong tron
	// <=0 nam trong duong tron

	int xA = enemy.m_iCurXPosPixel - point.x;
	int yA = enemy.m_iCurYPosPixel - point.y;

	int IA =  ((xA * xA) + (yA * yA)) - (radius * radius);
	return (IA > 0 ? -1 : 1);
}

/************************************************************************
VịLH - IsPlayer - 22/05/2013
	Kiểm tra NPC có phải là người chơi không
params:
	const CCharacter&: character cần kiểm tra
return:
	int: 1: người chơi / 0: monster
*************************************************************************/
int CCharacter::IsPlayer(const CCharacter& character)
{
	if (character.m_dwPhai == DAOSI || character.m_dwPhai == DAOSI2)
		return IM_PLAYER;
	if (character.m_dwPhai == GIAPSI || character.m_dwPhai == GIAPSI2)
		return IM_PLAYER;
	if (character.m_dwPhai == DINHAN || character.m_dwPhai == DINHAN2)
		return IM_PLAYER;

	return IM_MONSTER;
}

/************************************************************************
VịLH - GetMonsterIndexInRadius - 22/05/2013
	tìm các monster nằm trong phạm vi của người chơi
params:
	int: phạm vi dò tìm
	CArray<CCharacter, CCharacter&>&: quái nằm trong phạm vi này
*************************************************************************/
void CCharacter::GetMonsterIndexInRadius(int radius, CArray<CCharacter, CCharacter&>& arrEnemies, int type)
{
	if (!m_hHandle)	
		return;

	DWORD dwNpcBaseAddress;
	ReadProcessMemory(m_hHandle, (void*)NPC_INFO_BASEADDRESS, &dwNpcBaseAddress, (DWORD)sizeof(dwNpcBaseAddress), 0);

	int k = 0;
	for (int i=2; i<NPC_MAX_SIZE; i++)
	{
		CCharacter enemy;	
		enemy.m_iID = i;

		// Character Name;		
		DWORD dwBaseAddressName = dwNpcBaseAddress + NPC_OFFSET_B5C0 * i + NPC_OFFSET_15D0;		

		char sName[NPC_NAME_SIZE];
		memset(sName, 0x0, sizeof(sName));
		ReadProcessMemory(m_hHandle, (void*)dwBaseAddressName, &sName, (DWORD)sizeof(sName), 0);
		enemy.m_sName = (CString)sName;

		if (m_sName.Compare(enemy.m_sName) == 0 || enemy.m_sName == L"")	// trường hợp trùng tên và tên rỗng
			continue;

		// Current XPos;
		DWORD dwCurXPos = dwBaseAddressName - NPC_XPOS;
		ReadProcessMemory(m_hHandle, (void*)dwCurXPos, &enemy.m_iCurXPosPixel, (DWORD)sizeof(enemy.m_iCurXPosPixel), 0);

		// Current YPos;
		DWORD dwCurYPos = dwBaseAddressName - NPC_YPOS;
		ReadProcessMemory(m_hHandle, (void*)dwCurYPos, &enemy.m_iCurYPosPixel, (DWORD)sizeof(enemy.m_iCurYPosPixel), 0);

		// Phái;
		DWORD dwPhai = dwBaseAddressName + NPC_TYPE;
		ReadProcessMemory(m_hHandle, (void*)dwPhai, &enemy.m_dwPhai, (DWORD)sizeof(enemy.m_dwPhai), 0);

		// Current HP;
		DWORD dwCurrentHP = dwBaseAddressName - NPC_CURRENT_HP;
		ReadProcessMemory(m_hHandle, (void*)dwCurrentHP, &enemy.m_iHP, (DWORD)sizeof(enemy.m_iHP), 0);

		POINT point;
		point.x = m_iCurXPosPixel;
		point.y = m_iCurYPosPixel;		

		
		if ((CheckPointBelongCircle(point, radius, enemy) == 1))
		{
			if (type == IsPlayer(enemy))
				arrEnemies.Add(enemy);	

			else if (type == IM_PLAYER_MONSTER)
				arrEnemies.Add(enemy);	
		}
	}
}


/************************************************************************
VịLH - GetCurrentPosMap - 22/05/2013
	Lấy tọa độ map của người chơi
*************************************************************************/
void CCharacter::GetCurrentPosMap()
{
	DWORD dwNpcBaseAddress;
	ReadProcessMemory(m_hHandle, (void*)NPC_INFO_BASEADDRESS, &dwNpcBaseAddress, (DWORD)sizeof(dwNpcBaseAddress), 0);	

	// Current Player
	DWORD dwBaseAddressName = dwNpcBaseAddress + NPC_OFFSET_B5C0 * 1 + NPC_OFFSET_15D0;		

	DWORD dwPlayerInfo;

	// Current XPos;
	dwPlayerInfo = dwBaseAddressName - NPC_XPOS; 
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iCurXPosPixel, (DWORD)sizeof(m_iCurXPosPixel), 0);
	m_iCurXPosMap = m_iCurXPosPixel / 256;

	// Current YPos;
	dwPlayerInfo = dwBaseAddressName - NPC_YPOS; 
	ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &m_iCurYPosPixel, (DWORD)sizeof(m_iCurYPosPixel), 0);
	m_iCurYPosMap = m_iCurYPosPixel / 512;
}

/************************************************************************
VịLH - GetHP - 22/05/2013
	Lấy thông tin HP của người chơi
*************************************************************************/
void CCharacter::GetHP(void)
{
	DWORD dwNpcBaseAddress;
	ReadProcessMemory(m_hHandle, (void*)NPC_INFO_BASEADDRESS, &dwNpcBaseAddress, (DWORD)sizeof(dwNpcBaseAddress), 0);

	// Character Name;		
	DWORD dwBaseAddressName = dwNpcBaseAddress + NPC_OFFSET_B5C0 * m_iID + NPC_OFFSET_15D0;		

	// HP
	DWORD dwHP = dwBaseAddressName - NPC_CURRENT_HP;
	ReadProcessMemory(m_hHandle, (void*)dwHP, &m_iHP, (DWORD)sizeof(m_iHP), 0);
}

/************************************************************************
VịLH - GetSkillIDFromName - 22/05/2013
	Trả vể ID skill ứng với tên skill
params:
	CString: tên skill
return:
	int: id skill
*************************************************************************/
int CCharacter::GetSkillIDFromName(const CString& name)
{	
	int iSkillID = 0;
	DWORD dwType = GetPlayerType();
	switch (dwType)
	{
		case GIAPSI: case GIAPSI2:
			if (name == L"Tế Huyết Trảm")
				iSkillID = S_GS_TE_HUYET_TRAM;
			else if (name == L"Hồi Phong Trảm")
				iSkillID = S_GS_HOI_PHONG_TRAM;
			else if (name == L"Hoành Không Trảm")
				iSkillID = S_GS_HOANH_KHONG_TRAM;
			else if (name == L"Huyền Băng Trảm")
				iSkillID = S_GS_HUYEN_BANG_TRAM;
			else if (name == L"Liên Hoàn Trảm")
				iSkillID = S_GS_LIEN_HOAN_TRAM;
			else if (name == L"Thiên Quân Trảm")
				iSkillID = S_GS_THIEN_QUAN_TRAM;
			else if (name == L"Khai Sơn Trảm")
				iSkillID = S_GS_KHAI_SON_TRAM;
			else if (name == L"Điện Quang Trảm")
				iSkillID = S_GS_DIEN_QUANG_TRAM;
			else if (name == L"Tam Đầu Lục Thủ")
				iSkillID = S_GS_TAM_DAU_LUC_THU;
			else if (name == L"Hỏa Quang Trảm")
				iSkillID = S_GS_HOA_QUANG_TRAM;
			else if (name == L"Lạc Địa Trảm")
				iSkillID = S_GS_LAC_DIA_TRAM;
			else if (name == L"Khuynh Thành Nhất Kích")
				iSkillID = S_GS_KHUYNH_THANH_NHAT_KICH;
			break;

		case DAOSI: case DAOSI2:
			if (name == L"Chưởng Tâm Lôi")
				iSkillID = S_DS_CHUONG_TAM_LOI;
			else if (name == L"Hạn Địa Lôi")
				iSkillID = S_DS_HAN_DIA_LOI;
			else if (name == L"Phong Vân Lôi Động")
				iSkillID = S_DS_PHONG_VAN_LOI_DONG;
			else if (name == L"Lôi Động Cửu Thiên")
				iSkillID = S_DS_LOI_DONG_CUU_THIEN;
			else if (name == L"Lưu Tinh Thạch")
				iSkillID = S_DS_LUU_TINH_THACH;
			else if (name == L"Thiên Phong Địa Nhận")
				iSkillID = S_DS_THIEN_PHONG_DIA_NHAN;
			else if (name == L"Ngũ Nhạc Triều Tông")
				iSkillID = S_DS_NGU_NHAC_TRIEU_TONG;
			else if (name == L"Thiên Băng Địa Liệt")
				iSkillID = S_DS_THIEN_BANG_DIA_LIET;
			else if (name == L"Huyền Nữ Bổ Thiên")
				iSkillID = S_DS_HUYEN_NU_BO_THIEN;
			else if (name == L"Băng Tuyết Đạn")
				iSkillID = S_DS_BANG_TUYET_DAN;
			else if (name == L"Thiết Mã Băng Qua")
				iSkillID = S_DS_THIET_MA_BANG_QUA;
			else if (name == L"Băng Phong Bạo")
				iSkillID = S_DS_BANG_PHONG_DAO;
			else if (name == L"Băng Phong Vạn Lý")
				iSkillID = S_DS_BANG_PHONG_VAN_LY;
			else if (name == L"Tích Lịch Hỏa")
				iSkillID = S_DS_TICH_LICH_HOA;
			else if (name == L"Phong Lâm Hỏa Sơn")
				iSkillID = S_DS_PHONG_LAM_HOA_SON;
			else if (name == L"Thập Phương Liệt Hỏa")
				iSkillID = S_DS_THAP_PHUONG_LIET_HOA;
			else if (name == L"Tam Muội Chân Hỏa")
				iSkillID = S_DS_TAM_MUOI_CHAN_HOA;

			break;

		case DINHAN: case DINHAN2:
			if (name == L"Thôi Thần Chú")
				iSkillID = S_DN_THOI_THAN_CHU;
			else if (name == L"Phá Giáp Chú")
				iSkillID = S_DN_PHA_GIAP_CHU;
			else if (name == L"Trảm Tâm Chú")
				iSkillID = S_DN_TRAM_TAM_CHU;
			else if (name == L"Vạn Cốt Toàn Khô")
				iSkillID = S_DN_VAN_COT_TOAN_KHO;
			
			break;

	}	
	
	return iSkillID;
}
