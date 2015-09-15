// TabPK.cpp : implementation file
//

#include "stdafx.h"
#include "AutoPlay.h"
#include "TabPK.h"


// TabPK dialog

IMPLEMENT_DYNAMIC(CTabPK, CDialog)

CTabPK::CTabPK(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPK::IDD, pParent)
	, m_iXPos(0)
	, m_iYPos(0)
	, m_iRadius(0)
	, m_sCurrentChracterName(_T(""))
{

}

CTabPK::~CTabPK()
{
}

void CTabPK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_CMB_SKILL, m_cmbSkill); - ThongPH - 20130527 - Change to ComboBoxEx with ImageList
	DDX_Control(pDX, IDC_BTN_ADDSKILL, m_btnAddSkill);
	DDX_Control(pDX, IDC_BTN_DELSKILL, m_btnDelSkill);
	DDX_Control(pDX, IDC_BTN_MOVEUP, m_btnMoveUp);
	DDX_Control(pDX, IDC_BTN_MOVEDOWN, m_btnMoveDown);
	DDX_Control(pDX, IDC_LST_SELSKILL, m_lstSelSkill);
	DDX_Control(pDX, IDC_TXT_DELAY, m_txtDelay);
	DDX_Text(pDX, IDC_EDIT_X, m_iXPos);
	DDX_Text(pDX, IDC_EDIT_Y, m_iYPos);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_iRadius);
	DDX_Text(pDX, IDC_STATIC_CURRENTCHARACTER, m_sCurrentChracterName);
	DDX_Control(pDX, IDC_CHECK_PK, m_cbPK);
	DDX_Control(pDX, IDC_CHECK_MONSTER, m_cbMonster);
	DDX_Control(pDX, IDC_EDIT_RADIUS, m_txtRadius);
	DDX_Control(pDX, IDC_COMBOBOXEX_SKILL, m_cmbeSkill);
}


BEGIN_MESSAGE_MAP(CTabPK, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDSKILL, &CTabPK::OnBnClickedBtnAddskill)
	ON_BN_CLICKED(IDC_BTN_DELSKILL, &CTabPK::OnBnClickedBtnDelskill)
	ON_BN_CLICKED(IDC_BTN_MOVEUP, &CTabPK::OnBnClickedBtnMoveup)
	ON_BN_CLICKED(IDC_BTN_MOVEDOWN, &CTabPK::OnBnClickedBtnMovedown)
	//ON_CBN_SELCHANGE(IDC_CMB_SKILL, &CTabPK::OnCbnSelchangeCmbSkill) - ThongPH - 20130527 - Change to ComboBoxEx with ImageList
	ON_BN_CLICKED(IDC_BUTTON_GETPOS, &CTabPK::OnBnClickedButtonGetpos)
	ON_CBN_SELCHANGE(IDC_COMBOBOXEX_SKILL, &CTabPK::OnCbnSelchangeComboboxexSkill)
END_MESSAGE_MAP()

BOOL CTabPK::OnInitDialog(){
	CDialog::OnInitDialog();
	m_txtDelay.SetWindowTextW(L"0");
	setImage(&m_btnAddSkill,L"Images/Add.png");
	setImage(&m_btnDelSkill,L"Images/Delete.png");
	setImage(&m_btnMoveUp,L"Images/Up.png");
	setImage(&m_btnMoveDown,L"Images/Down.png");

	//Tạo cột cho danh sách skill
	m_lstSelSkill.SetExtendedStyle(LVS_EX_AUTOSIZECOLUMNS | LVS_EX_FULLROWSELECT);
	m_lstSelSkill.InsertColumn(0,L"Skill",LVCFMT_LEFT,140);
	m_lstSelSkill.InsertColumn(1,L"Delay",LVCFMT_LEFT,50);

	UpdateData(TRUE);
	m_sCurrentChracterName = L"Tọa độ nhân vật:";
	m_iRadius = 2000;
	UpdateData(FALSE);

	return true;
}

void CTabPK::setImage(CButton* btn, CString img){
	m_pngImage.Load(img);
	if(!m_pngImage.IsNull()){
		btn->SetButtonStyle(BS_BITMAP,1);
		CBitmap m_bmp;
		m_bmp.Attach(m_pngImage.Detach());
		HBITMAP hBitmap = (HBITMAP)m_bmp.GetSafeHandle();
		btn->SetBitmap(hBitmap);
	}
}

void CTabPK::addData(int row, int col,CString str)
{
    LVITEM lv;
    lv.iItem = row;
    lv.iSubItem = col;
    lv.pszText =  (LPTSTR) (LPCTSTR)str;
    lv.mask = LVIF_TEXT;
    if(col == 0)
        m_lstSelSkill.InsertItem(&lv);
    else
        m_lstSelSkill.SetItem(&lv); 
}

BOOL CTabPK::LoadSkillFile(CString strFilename){
	TCHAR m_curDir[256];
	GetCurrentDirectory(sizeof(m_curDir),m_curDir);
	CString strIniFileName;
	strIniFileName.Format(L"%s", m_curDir);
	strIniFileName += L"\\" + strFilename;
	CFileStatus m_status;
	if( !CFile::GetStatus(strIniFileName, m_status))
	{
		// not existed
		return FALSE;
	}
	m_iniReader.setINIFileName(strIniFileName);
	return TRUE;
}

void CTabPK::LoadSkillData(CString strType)
{
	//ThongPH - 20130522 - Load Skill to ComboBoxEX with image
	CStringList* section = m_iniReader.getSectionData(strType);
	CString str;
	POSITION Position = section->GetHeadPosition();
	int i = 0;

	//Reset Content
	m_lstSelSkill.DeleteAllItems();
	m_SkillImage.DeleteImageList();
	m_lstSkillImage.DeleteImageList();
	m_cmbeSkill.ResetContent();

	//Type of Role
	int nSkillCount = 0;
	int nType = 0;
	if (strType == "GiapSi")
	{
		nType = IDB_BITMAP_GIAPSI;
		nSkillCount = 12;
	}else if (strType == "DaoSi")
	{
		nType = IDB_BITMAP_DAOSI;
		nSkillCount = 17;
	}else if(strType == "DiNhan")
	{
		nType = IDB_BITMAP_DINHAN;
		nSkillCount = 4;
	}

	//Attach Skill ImageList to CombobBoxEx
	CBitmap bm;
	bm.LoadBitmap(nType);
	m_SkillImage.Create(24, 24, ILC_COLOR24, nSkillCount, 0);
	m_SkillImage.Add(&bm, RGB(255,255,255));
	m_cmbeSkill.SetImageList(&m_SkillImage);

	//ThongPH - 20130522 - edit Skill with ComboBoxEX
	//Attach Skill ImageList to ListCtrl
	CBitmap bm2;
	bm2.LoadBitmap(nType);
	m_lstSkillImage.Create(24, 24, ILC_COLOR24, nSkillCount, 0);
	m_lstSkillImage.Add(&bm2, RGB(255,255,255));
	m_lstSelSkill.SetImageList(&m_lstSkillImage, LVSIL_NORMAL);
	m_lstSelSkill.SetImageList(&m_lstSkillImage, LVSIL_SMALL);
	//ThongPH - 20130522 - edit Skill with ComboBoxEX

	while (Position){
		str = section->GetNext(Position);
		int n = str.Find('=');
		str = str.Left(n).Trim();

		COMBOBOXEXITEM     cbi;		
		int                nItem;

		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY |
			CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
		cbi.iItem = i;
		cbi.pszText = (LPTSTR)(LPCTSTR)str;
		cbi.cchTextMax = str.GetLength();
		cbi.iImage = i;
		cbi.iSelectedImage = i;
		cbi.iOverlay = i;
		cbi.iIndent = 0; //Set indentation according to item position

		nItem = m_cmbeSkill.InsertItem(&cbi);
		ASSERT(nItem == i);
		i++;
	}
	m_cmbeSkill.SetCurSel(0);
	//ThongPH - 20130522 - Load Skill to ComboBoxEX with image
}

void CTabPK::OnBnClickedBtnAddskill()
{	
	//ThongPH - 20130522 - edit Skill with ComboBoxEX
	int totalItem = m_lstSelSkill.GetItemCount();
	int index = m_cmbeSkill.GetCurSel();
	if(index != CB_ERR)
	{
		CString text;
		m_cmbeSkill.GetLBText(index,text);
		for (int i = 0; i<m_lstSelSkill.GetItemCount(); i++)
		{
			CString str = m_lstSelSkill.GetItemText(i,0);
			if (str == text)
				return;
		}
		//ThongPH - 20130522 - edit Skill with ComboBoxEX
		LVITEM lvi;
		lvi.mask =  LVIF_IMAGE | LVIF_TEXT;		
		lvi.iItem = totalItem;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(text);
		lvi.iImage = index;		
		m_lstSelSkill.InsertItem(&lvi);
		m_txtDelay.GetWindowText(text);		
		onChange();
		//ThongPH - 20130522 - edit Skill with ComboBoxEX
	}
	//ThongPH - 20130522 - edit Skill with ComboBoxEX
}

void CTabPK::OnBnClickedBtnDelskill()
{
	//ThongPH - 20130522 - edit Skill with ComboBoxEX
	int index = m_lstSelSkill.GetNextItem(-1, LVNI_SELECTED);
	if(index == CB_ERR) 
		return;	
	m_lstSelSkill.DeleteItem(index);	
	//ThongPH - 20130522 - edit Skill with ComboBoxEX
}

void CTabPK::OnOK(){
}

void CTabPK::OnCancel(){
}


void CTabPK::OnBnClickedBtnMoveup()
{
	int item = m_lstSelSkill.GetNextItem(-1,LVNI_SELECTED);
    if(item == CB_ERR) 
        return;

    if(item > 0)
    {
		//ThongPH - 20130531 - Edit ListCtrl with Image
		CString strCurItem, strBeforeItem;
		int idxImgTemp;
		strCurItem = m_lstSelSkill.GetItemText(item,0);
		strBeforeItem = m_lstSelSkill.GetItemText(item - 1, 0);
		LVITEM lvi;
		lvi.mask =  LVIF_IMAGE | LVIF_TEXT;		
		lvi.iItem = item;
		m_lstSelSkill.GetItem(&lvi);

		LVITEM lviBefore;
		lviBefore.mask =  LVIF_IMAGE | LVIF_TEXT;		
		lviBefore.iItem = item - 1;
		m_lstSelSkill.GetItem(&lviBefore);

		idxImgTemp = lvi.iImage;

		lvi.pszText = (LPTSTR)(LPCTSTR)strBeforeItem;
		lvi.iImage = lviBefore.iImage;
		m_lstSelSkill.SetItem(&lvi);

		lviBefore.pszText = (LPTSTR)(LPCTSTR)strCurItem;
		lviBefore.iImage = idxImgTemp;
		m_lstSelSkill.SetItem(&lviBefore);

		m_lstSelSkill.SetItemState(item - 1,LVNI_SELECTED,LVIS_SELECTED);
		//ThongPH - 20130531 - Edit ListCtrl with Image
    }
}

void CTabPK::OnBnClickedBtnMovedown()
{
	int item = m_lstSelSkill.GetNextItem(-1,LVNI_SELECTED);
    if(item == CB_ERR) 
        return;

    if(item < m_lstSelSkill.GetItemCount() - 1)
    {
		//ThongPH - 20130531 - Edit ListCtrl with Image
		CString strCurItem, strAfterItem;
		int idxImgTemp;
		strCurItem = m_lstSelSkill.GetItemText(item,0);
		strAfterItem = m_lstSelSkill.GetItemText(item + 1, 0);
		LVITEM lvi;
		lvi.mask =  LVIF_IMAGE | LVIF_TEXT;		
		lvi.iItem = item;
		m_lstSelSkill.GetItem(&lvi);

		LVITEM lviAfter;
		lviAfter.mask =  LVIF_IMAGE | LVIF_TEXT;		
		lviAfter.iItem = item + 1;
		m_lstSelSkill.GetItem(&lviAfter);

		idxImgTemp = lvi.iImage;

		lvi.pszText = (LPTSTR)(LPCTSTR)strAfterItem;
		lvi.iImage = lviAfter.iImage;
		m_lstSelSkill.SetItem(&lvi);

		lviAfter.pszText = (LPTSTR)(LPCTSTR)strCurItem;
		lviAfter.iImage = idxImgTemp;
		m_lstSelSkill.SetItem(&lviAfter);

		m_lstSelSkill.SetItemState(item + 1,LVNI_SELECTED,LVIS_SELECTED);
		//ThongPH - 20130531 - Edit ListCtrl with Image
    }
}
void CTabPK::SaveCharacterSkill(CString strName)
{
	TCHAR m_curDir[256];
	GetCurrentDirectory(sizeof(m_curDir),m_curDir);
	CString strIniFileName;
	strIniFileName.Format(L"%s", m_curDir);
	strIniFileName += L"\\Character";
	CreateDirectory(strIniFileName, NULL);
	strIniFileName = strIniFileName + L"\\" + strName + L".ini";

	CIniReader m_iniCharReader;
	m_iniCharReader.setINIFileName(strIniFileName);

	m_iniCharReader.setKey(L"Skill", L"Sect", strName);

	int totalItem = m_lstSelSkill.GetItemCount();	// lấy toàn bộ danh sách trong listView
	if (totalItem == 0)
		return;
	
	for (int i=0; i< totalItem; i++)
	{
		CString str0 = m_lstSelSkill.GetItemText(i,0);
		CString str1 = m_lstSelSkill.GetItemText(i,1);
		m_iniCharReader.setKey(L"Skill", str0, str1);
	}
	
}
//void CTabPK::OnCbnSelchangeCmbSkill()
//{
//	onChange();
//}
void CTabPK::onChange()
{	
	//ThongPH - 20130522 - edit Skill with ComboBoxEX
	int index = m_cmbeSkill.GetCurSel();
	CString text;
	m_cmbeSkill.GetLBText(index,text);
	m_Delay = m_iniReader.getItegerValue(m_Type,text);
	text.Format(L"%d",m_Delay);
	m_txtDelay.SetWindowText(text);
	//ThongPH - 20130522 - edit Skill with ComboBoxEX
}

void CTabPK::LoadCurrentPos(CCharacter character)
{
	UpdateData(TRUE);	
	m_cCurrentCharacter = character;
	m_iXPos = character.m_iCurXPosMap;
	m_iYPos = character.m_iCurYPosMap;
	m_sCurrentChracterName = L"Tọa độ nhân vật: " + character.m_sName;
	UpdateData(FALSE);
}

void CTabPK::OnBnClickedButtonGetpos()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_cCurrentCharacter.GetCurrentPlayerInfo();
	m_iXPos = m_cCurrentCharacter.m_iCurXPosMap;
	m_iYPos = m_cCurrentCharacter.m_iCurYPosMap;
	UpdateData(FALSE);
}

void CTabPK::OnCbnSelchangeComboboxexSkill()
{
	// TODO: Add your control notification handler code here
	onChange();
}
