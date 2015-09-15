// WndBuff.cpp : implementation file
//

#include "stdafx.h"
#include "WndSkillCfg.h"
#include "MainDlg.h"
#include "../SSC_GameEngine/GameEngine.h"
#include "../SSC_GameEngine/Character.h"


using namespace std;
// CWndSkillCfg dialog
IMPLEMENT_DYNAMIC(CWndSkillCfg, CDialog)

CWndSkillCfg::CWndSkillCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CWndSkillCfg::IDD, pParent)
{
	 Create(CWndSkillCfg::IDD, pParent); 
	 if(pParent != NULL){
		 m_wndParent = (CMainDlg*) pParent;
		 m_wndParent->GetParent()->EnableWindow(false);
	 }
}


CWndSkillCfg::~CWndSkillCfg()
{
	m_wndParent=NULL;
	m_mSkill.clear();
	
}

void CWndSkillCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CWndSkillCfg::PostNcDestroy() 
{	
    CDialog::PostNcDestroy();
    delete this;
}
BEGIN_MESSAGE_MAP(CWndSkillCfg, CDialog)
	ON_BN_CLICKED(ID_BTN_ADD_CFG, &CWndSkillCfg::OnBtnAddClick)
	ON_BN_CLICKED(ID_BTN_REMOVE_CFG, &CWndSkillCfg::OnBtnRemoveClick)
	ON_BN_CLICKED(ID_BTN_UP_CFG, &CWndSkillCfg::OnBtnUpClick)
	ON_BN_CLICKED(ID_BTN_DOWN_CFG, &CWndSkillCfg::OnBtnDownClick)
	ON_BN_CLICKED(ID_BTN_SAVE_CFG, &CWndSkillCfg::OnBtnSaveClick)
	ON_CBN_SELCHANGE(IDC_CMB_SKILL_CFG, &CWndSkillCfg::OnCbnSelchange)
END_MESSAGE_MAP()


// CWndSkillCfg message handlers
void CWndSkillCfg::OnCancel(){
	
}

void CWndSkillCfg::LoadSkillList(bool isBuff){
 	 //load danh sach skill
	bIsBuff = isBuff;
	CCharacter* m_char = m_wndParent->m_clsCurChar;	
	if(m_char==NULL)
		return;
	bIsFighting = m_char->m_bAutoFight;
	bIsAutoQuest = m_char->m_bAutoQuest;
		
	m_char->m_bAutoFight = false;
	m_char->m_bAutoQuest = false;

	SKILL_DATA* objSkill;
	SKILL_CFG* objSkillCfg;
	std::map<CString,SKILL_DATA*>::iterator item;
	std::map<int,SKILL_CFG*>::iterator skill_cfg;
	mSkill.clear();
	CComboBox* cmbLstSkill =  (CComboBox*)GetDlgItem(IDC_CMB_SKILL_CFG);
	CEdit * txtRange =  (CEdit*)GetDlgItem(IDC_TXT_RANGE_CFG);
	CEdit * txtDelay =  (CEdit*)GetDlgItem(IDC_TXT_DELAY_CFG);
	CListCtrl* lstLstSel =  (CListCtrl*)GetDlgItem(IDC_LST_SKILL_CFG);

	lstLstSel->SetExtendedStyle(lstLstSel->GetStyle()|LVS_EX_FULLROWSELECT);
	lstLstSel->InsertColumn(0,L"Skill",LVCFMT_LEFT,100);
	lstLstSel->InsertColumn(1,L"Range",LVCFMT_CENTER,70);
	lstLstSel->InsertColumn(2,L"Delay",LVCFMT_CENTER,70);
	
	for(item=  m_char->m_mSkills.begin(); item != m_char->m_mSkills.end(); item ++){
		objSkill = item->second;
		if(objSkill->m_iLevel > m_char->m_iLevel)
			continue;
		if(objSkill->m_iType>3)
			continue;
		if(isBuff && objSkill->m_iType==2)
			cmbLstSkill->AddString(item->first);
		if(!isBuff && objSkill->m_iType==1)
			cmbLstSkill->AddString(item->first);
		mSkill.insert(make_pair( objSkill->m_iId,objSkill->m_sName));
	}
	if(cmbLstSkill->GetCount()>0)
		cmbLstSkill->SetCurSel(0);	
	txtRange->SetWindowTextW(L"50");
	txtDelay->SetWindowTextW(L"0");
	if(isBuff){
		SetWindowTextW(L"Thiết lập Buff");	
		m_mSkill = m_char->m_mBuff;
		txtRange->SetWindowTextW(L"0");
		txtRange->EnableWindow(0);
	}
	else{
		SetWindowTextW(L"Thiết lập Skill");

		m_mSkill = m_char->m_mSkill;
	}

	if(m_mSkill.size()<=0)
	{
		m_char= NULL;
		return;
	}

	for(skill_cfg=  m_mSkill.begin(); skill_cfg != m_mSkill.end(); skill_cfg ++){
		
		objSkillCfg = skill_cfg->second;

		LVITEM lv;
		lv.iItem = skill_cfg->first;
		lv.iSubItem = 0;		
		lv.pszText = (LPTSTR)(LPCTSTR) mSkill[objSkillCfg->m_iId];
		lv.mask = LVIF_TEXT;
		int iCurIdx = lstLstSel->InsertItem(&lv);

		//insert sub item
		CString sVal;
		lv.iSubItem = 1;
		sVal.Format(L"%d",objSkillCfg->m_iRange);
		lv.pszText = (LPTSTR)(LPCTSTR) sVal;
		lstLstSel->SetItem(&lv); 
		
		sVal.Format(L"%d",objSkillCfg->m_iDelay);
		lv.iSubItem = 2;
		lv.pszText = (LPTSTR)(LPCTSTR) sVal;
		lstLstSel->SetItem(&lv); 
	}
	

	m_char= NULL;
}

void CWndSkillCfg::OnBtnAddClick(){
	CCharacter* m_char = m_wndParent->m_clsCurChar;;
	if(m_char==NULL)
		return;
	CListCtrl* lstLstSel =  (CListCtrl*)GetDlgItem(IDC_LST_SKILL_CFG);
	CComboBox* cmbLstSkill =  (CComboBox*)GetDlgItem(IDC_CMB_SKILL_CFG);
	CEdit * txtRangle =  (CEdit*)GetDlgItem(IDC_TXT_RANGE_CFG);
	CEdit * txtDelay =  (CEdit*)GetDlgItem(IDC_TXT_DELAY_CFG);
	
	CString sName;
	CString sRange;
	CString sDelay;
	
	int	iIndex;
	int iCurSel;

	txtRangle->GetWindowTextW(sRange);
	if(sRange.GetLength()==0){
		m_char= NULL;
		return;
	}
	txtDelay->GetWindowTextW(sDelay);
	if(sDelay.GetLength()==0){
		m_char= NULL;
		return;
	}
	if(cmbLstSkill->GetCurSel()==-1){
		m_char= NULL;
		return;
	}


	cmbLstSkill->GetLBText(cmbLstSkill->GetCurSel(),sName);
	iIndex = lstLstSel->GetItemCount();
	//add to list

	LVITEM lv;
	lv.iItem = iIndex;
    lv.iSubItem = 0;		
	lv.pszText = (LPTSTR)(LPCTSTR) sName;
    lv.mask = LVIF_TEXT;
	int iCurIdx = lstLstSel->InsertItem(&lv);

	//insert sub item
    lv.iSubItem = 1;
	lv.pszText = (LPTSTR)(LPCTSTR) sRange;
	lstLstSel->SetItem(&lv); 

	lv.iSubItem = 2;
	lv.pszText = (LPTSTR)(LPCTSTR) sDelay;
	lstLstSel->SetItem(&lv); 

	//Set to list buff
	SKILL_CFG* skillCfg = new SKILL_CFG();
	skillCfg->m_iId = m_char->m_mSkills[sName]->m_iId;
	skillCfg->m_iDelay = _ttoi(sDelay);
	skillCfg->m_iRange= _ttoi(sRange);
	m_mSkill.insert(make_pair( iIndex,skillCfg));
	m_char= NULL;
}


void CWndSkillCfg::OnBtnRemoveClick(){
	CCharacter* m_char = m_wndParent->m_clsCurChar;;
	if(m_char==NULL){
		m_char= NULL;
		return;
	}
	if(m_char->m_bIsHooking && m_char->m_bAutoFight){
		MessageBoxW(L"Hãy dừng đánh trước khi xóa skill");
		m_char= NULL;
		return;
	}
	CListCtrl* lstLstSel =  (CListCtrl*)GetDlgItem(IDC_LST_SKILL_CFG);
	int iIndex = lstLstSel->GetNextItem(-1, LVNI_SELECTED);
	if(iIndex==-1){
		m_char= NULL;
		return;
	}
	for(int i=iIndex;i<m_mSkill.size()-1;i++){
		m_mSkill[i] = m_mSkill[i+1];
	}
	int iDelIdx = m_mSkill.size()-1;
	m_mSkill.erase(iDelIdx);
	
	//del item in list buff
	lstLstSel->DeleteItem(iIndex);
	int cnt = lstLstSel->GetItemCount();
	if(cnt>0){
		if(iIndex >= cnt) 
			iIndex=cnt -1;
		lstLstSel->SetItemState(iIndex, LVIS_SELECTED, LVIS_SELECTED);
		lstLstSel->SetSelectionMark(iIndex+1);
	}

	m_char= NULL;	
}

void CWndSkillCfg::OnBtnUpClick(){
	CCharacter* m_char = m_wndParent->m_clsCurChar;;
	if(m_char==NULL)
		return;
	CListCtrl* lstLstSel =  (CListCtrl*)GetDlgItem(IDC_LST_SKILL_CFG);

	int iIndex = lstLstSel->GetNextItem(-1, LVNI_SELECTED);
	if(iIndex<1){
		m_char= NULL;
		return;
	}
	SKILL_CFG* item = m_mSkill[iIndex];
	m_mSkill[iIndex] = m_mSkill[iIndex-1];
	m_mSkill[iIndex-1]=item;
	
	item = m_mSkill[iIndex];
	LVITEM lv;
	lv.iItem = iIndex;
    lv.iSubItem = 0;		
	lv.pszText =(LPTSTR)(LPCTSTR) mSkill[item->m_iId];
    lv.mask = LVIF_TEXT;
	lstLstSel->SetItem(&lv);

	//insert sub item
	CString val;
	val.Format(L"%d",item->m_iRange);
    lv.iSubItem = 1;
	lv.pszText = (LPTSTR)(LPCTSTR) val;
	lstLstSel->SetItem(&lv); 

	val.Format(L"%d",item->m_iDelay);
	lv.iSubItem = 2;
	lv.pszText = (LPTSTR)(LPCTSTR) val;
	lstLstSel->SetItem(&lv); 


	item = m_mSkill[iIndex-1];
	lv.iItem = iIndex-1;
    lv.iSubItem = 0;		
	lv.pszText =(LPTSTR)(LPCTSTR) mSkill[item->m_iId];
    lv.mask = LVIF_TEXT;
	lstLstSel->SetItem(&lv);

	//insert sub item
	val.Format(L"%d",item->m_iRange);
    lv.iSubItem = 1;
	lv.pszText = (LPTSTR)(LPCTSTR) val;
	lstLstSel->SetItem(&lv); 

	val.Format(L"%d",item->m_iDelay);
	lv.iSubItem = 2;
	lv.pszText = (LPTSTR)(LPCTSTR) val;
	lstLstSel->SetItem(&lv);

	lstLstSel->SetItemState(iIndex, ~LVIS_SELECTED, LVIS_SELECTED);
	lstLstSel->SetItemState(iIndex-1, LVIS_SELECTED, LVIS_SELECTED);
	lstLstSel->SetSelectionMark(iIndex-1);
	m_char= NULL;	


}
void CWndSkillCfg::OnBtnDownClick(){
	CCharacter* m_char = m_wndParent->m_clsCurChar;;
	if(m_char==NULL)
		return;
	CListCtrl* lstLstSel =  (CListCtrl*)GetDlgItem(IDC_LST_SKILL_CFG);

	int iIndex = lstLstSel->GetNextItem(-1, LVNI_SELECTED);
	if(iIndex>= lstLstSel->GetItemCount()-1 ){
		m_char= NULL;
		return;
	}
	SKILL_CFG* item = m_mSkill[iIndex];
	m_mSkill[iIndex] = m_mSkill[iIndex+1];
	m_mSkill[iIndex+1]=item;
	
	item = m_mSkill[iIndex];
	LVITEM lv;
	lv.iItem = iIndex;
    lv.iSubItem = 0;		
	lv.pszText =(LPTSTR)(LPCTSTR) mSkill[item->m_iId];
    lv.mask = LVIF_TEXT;
	lstLstSel->SetItem(&lv);

	//insert sub item
	CString val;
	val.Format(L"%d",item->m_iRange);
    lv.iSubItem = 1;
	lv.pszText = (LPTSTR)(LPCTSTR) val;
	lstLstSel->SetItem(&lv); 

	val.Format(L"%d",item->m_iDelay);
	lv.iSubItem = 2;
	lv.pszText = (LPTSTR)(LPCTSTR) val;
	lstLstSel->SetItem(&lv); 


	item = m_mSkill[iIndex+1];
	lv.iItem = iIndex+1;
    lv.iSubItem = 0;		
	lv.pszText =(LPTSTR)(LPCTSTR) mSkill[item->m_iId];
    lv.mask = LVIF_TEXT;
	lstLstSel->SetItem(&lv);

	//insert sub item
	val.Format(L"%d",item->m_iRange);
    lv.iSubItem = 1;
	lv.pszText = (LPTSTR)(LPCTSTR) val;
	lstLstSel->SetItem(&lv); 

	val.Format(L"%d",item->m_iDelay);
	lv.iSubItem = 2;
	lv.pszText = (LPTSTR)(LPCTSTR) val;
	lstLstSel->SetItem(&lv);

	lstLstSel->SetItemState(iIndex, ~LVIS_SELECTED, LVIS_SELECTED);
	lstLstSel->SetItemState(iIndex+1, LVIS_SELECTED, LVIS_SELECTED);
	lstLstSel->SetSelectionMark(iIndex+1);
	m_char= NULL;	
}

void CWndSkillCfg::OnCbnSelchange(){
	CCharacter* m_char = m_wndParent->m_clsCurChar;	
	if(m_char==NULL)
		return;
	CString sName;
	CString sDelay;
	CString sRange;
	CComboBox* cmbLstSkill =  (CComboBox*)GetDlgItem(IDC_CMB_SKILL_CFG);
	CEdit * txtDelay =  (CEdit*)GetDlgItem(IDC_TXT_DELAY_CFG);
	CEdit * txtRange =  (CEdit*)GetDlgItem(IDC_TXT_RANGE_CFG);
	cmbLstSkill->GetLBText(cmbLstSkill->GetCurSel(),sName);
	SKILL_DATA* skill= m_char->m_mSkills[sName];
	if(skill!=NULL){
		sDelay.Format(L"%d",skill->m_iDelay);
		txtDelay->SetWindowTextW(sDelay);
		sRange.Format(L"%d",skill->m_iRange);
		txtRange->SetWindowTextW(sRange);
	}

}
void CWndSkillCfg::OnBtnSaveClick(){
	CCharacter* m_char = m_wndParent->m_clsCurChar;
	if(m_char!=NULL ){
		if(bIsBuff){
			m_char->m_mBuff = m_mSkill;
			m_char->SaveSkillCfg(L"Buffs");		
		}
		else{
			m_char->m_mSkill = m_mSkill;
			m_char->SaveSkillCfg(L"Skills");		
		}
		m_char->m_bAutoFight = bIsFighting;
		m_char->m_bAutoQuest = bIsAutoQuest;
	}	

	m_char= NULL;
	
	if(m_wndParent!=NULL){
		m_wndParent->GetParent()->EnableWindow(true);
		m_wndParent->GetParent()->ShowWindow(SW_SHOW);
	}
	PostNcDestroy();
}


