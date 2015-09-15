#pragma once


enum ControlProp { Text,Check,Enable,Selected,Value };
enum DataType { Int, String, Float, Bool, ByteArray, IntArray,None };
enum TagType { Root, Element, Attribute };
typedef struct SKILL_DATA
{
	int		m_iId;
	CString	m_sName;
	int		m_iType; //1:Active (left) | 2:Deactive (right) | 3:Buff
	int		m_iLevel;
	int		m_iDelay;
	int		m_iRange;
} SKILL_DATA;
typedef struct SKILL_CFG
{
	int		m_iId;
	int		m_iRange;
	int		m_iDelay;
} SKILL_CFG;
typedef struct COOR
{
	int		_iX;
	int		_iY;
	COOR(){
		_iX = 0;
		_iY=0;
	}
	
} COOR;
typedef struct DATA
	{
		CString		_parent;
		CString		_tagName;
		DataType	_dataType;
		DWORD		_value;
		TagType		_tagType;
	} DATA;

typedef struct GUI_MAPPING
{
	ControlProp	_ctrlProp;
	DataType	_dataType;
	DWORD		_prop;
} GUI_MAPPING;

typedef struct CAPTAIN
{
	int		m_iId;
	CString	m_sName;
	short	m_iCurMap;
	int		m_iCurX;
	int		m_iCurY;
} CAPTAIN;

typedef struct POS
{
	int		_iX;
	int		_iY;
	int		_iMapId;	//map thuc hien nhiem vu
} POS;

typedef struct PARTY_INFO
{
	CString  _NameMember;
	bool	_IsOnline;
	bool	_IsMember;

} PARTY_INFO;