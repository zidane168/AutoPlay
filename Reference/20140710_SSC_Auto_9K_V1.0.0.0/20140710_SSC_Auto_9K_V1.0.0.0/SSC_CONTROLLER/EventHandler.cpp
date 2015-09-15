// Filename: EventHandler.cpp 
// 13-Dec-2013 SSC copy right.

#include "stdafx.h"
#include "WndControl.h"
#include "WndDlg.h"
#include "MainDlg.h"
#include "EventHandler.h"
#include "../SSC_GameEngine/GameEngine.h"
#include "../SSC_GameEngine/Character.h"
#include "../SSC_GameEngine/Serialize.h"
#include "WndSkillCfg.h"
#include "WndPriority.h"


using namespace std;


CEventHandler::CEventHandler()
{
	events.clear();
}

CEventHandler::~CEventHandler()
{
	events.clear();
}

void CEventHandler::AddEventHandler(CString eventName,CEventHandler::function func){
	events.insert(make_pair(eventName, func ));
}
void CEventHandler::RemoveEventHandler(CString eventName){
	map<CString,CEventHandler::function>::const_iterator result = events.find(eventName);
	if (result != events.end()){
		events.erase(eventName);
	}
}
void CEventHandler::ClearEventHandler(){
	events.clear();
}

CEventHandler::function CEventHandler::RaiseEvent(CString eventName){
	map<CString,CEventHandler::function>::const_iterator result = events.find(eventName);
	if (result != events.end())
		return (*events[eventName]);
	return NULL;
}

void CEventHandler::SetPriority(CWndControl* sender,CMainDlg* parent){
	CWndComboBox* cmb = static_cast<CWndComboBox*> (sender);
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char!=NULL){
	
		m_char->m_iPriority = cmb->GetSelectedIndex();
		
		CWndButton * btn = static_cast<CWndButton*> (dlgCtrlContainer->GetControl(L"btnPriority"));
		btn->SetEnabled(m_char->m_iPriority==1 || m_char->m_iPriority==2);
		m_char->serialize->SaveInfo(L"Priority",m_char->m_iPriority,TagType::Element,L"/Info");
	}
}

void CEventHandler::BtnConfigSkill(CWndControl* sender,CMainDlg* parent){
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	if(m_wndSkillCfgDlg==NULL || !IsWindow(m_wndSkillCfgDlg->m_hWnd)){
		m_wndSkillCfgDlg= new CWndSkillCfg(parent);
	}
	m_wndSkillCfgDlg->LoadSkillList(false);
	m_wndSkillCfgDlg->ShowWindow(SW_SHOW);
}
void CEventHandler::BtnConfigBuff(CWndControl* sender,CMainDlg* parent){
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	if(m_wndSkillCfgDlg==NULL || !IsWindow(m_wndSkillCfgDlg->m_hWnd)){
		m_wndSkillCfgDlg= new CWndSkillCfg(parent);
	}
	
	m_wndSkillCfgDlg->LoadSkillList(true);
	m_wndSkillCfgDlg->ShowWindow(SW_SHOW);
}

void CEventHandler::BtnConfigPriority(CWndControl* sender,CMainDlg* parent){
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL|| (m_char->m_iPriority!=1 && m_char->m_iPriority!=2)|| (m_char->m_bIsHooking && m_char->m_bAutoFight) )
		return;
	if(m_wndPriorityDlg==NULL || !IsWindow(m_wndPriorityDlg->m_hWnd)){
		m_wndPriorityDlg= new CWndPriority(parent);
	}
	m_wndPriorityDlg->ShowWindow(SW_SHOW);	

}
void CEventHandler::ChkFightToDie(CWndControl* sender,CMainDlg* parent){
	CWndCheckBox* ctrl = static_cast<CWndCheckBox*> (sender);
	CWndCheckBox* chk = static_cast<CWndCheckBox*> (parent->m_controlDlg->GetControl(L"chkNotFollow"));
	chk->SetEnabled(ctrl->IsChecked());

}

void CEventHandler::ChkAutoFight(CWndControl* sender,CMainDlg* parent){
	//CWndCheckBox* ctrl = static_cast<CWndCheckBox*> (sender);
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	m_char->SetAutoFight();
}

//////////////////////////////////////Tab bãi quái //////////////////////////////////////////////////////////
void CEventHandler::AddMonsterCoor(CWndControl* sender,CMainDlg* parent){
	//Thêm tọa độ vào danh sách
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	CWndListBox * lstMonster = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstMonsterCoor"));
	
	COOR cCoor;
	cCoor._iX = m_char->m_iTdX;
	cCoor._iY = m_char->m_iTdY;

	m_char->m_lMonsters.push_back( cCoor);
	
	CString sCoor;
	sCoor.Format(L"%d,%d",cCoor._iX ,cCoor._iY);
	lstMonster->AddItem(sCoor);
}

void CEventHandler::RemoveMonsterCoor(CWndControl* sender,CMainDlg* parent){
	//xóa tọa độ khỏi danh sách
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	CWndListBox * lstMonster = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstMonsterCoor"));
	if(lstMonster->GetSelectedIndex()==-1)
		return;
	m_char->m_lMonsters.erase(m_char->m_lMonsters.begin()+lstMonster->GetSelectedIndex());
	lstMonster->ClearItems();
	std::vector<COOR>::iterator item;
	for(item=  m_char->m_lMonsters.begin(); item != m_char->m_lMonsters.end(); item ++){
		CString sCoor;
		sCoor.Format(L"%d,%d",item->_iX ,item->_iY);
		lstMonster->AddItem(sCoor);
	}
}
void CEventHandler::RemoveAllMonsterCoor(CWndControl* sender,CMainDlg* parent){
	//Xóa tất cả tọa độ trong danh sách
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CCharacter* m_char = parent->m_clsCurChar;
	CWndListBox * lstMonster = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstMonsterCoor"));
	if(m_char==NULL)
		return;
	m_char->m_lMonsters.clear();
	lstMonster->ClearItems();
}
void CEventHandler::UpMonsterCoor(CWndControl* sender,CMainDlg* parent){
	//Thay đổi tọa độ trong danh sách
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CWndListBox * lstMonster = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstMonsterCoor"));
	int itemSel=lstMonster->GetSelectedIndex();
	if(itemSel<=0)
		return;
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;

	CString sTmp = lstMonster->GetItem(itemSel);
	lstMonster->SetItem(itemSel,lstMonster->GetItem(itemSel-1));
	lstMonster->SetItem(itemSel-1,sTmp);
	lstMonster->SetSelectedIndex(itemSel-1);

		
	COOR tmp = m_char->m_lMonsters[itemSel];
	m_char->m_lMonsters[itemSel]=m_char->m_lMonsters[itemSel-1];
	m_char->m_lMonsters[itemSel-1]=tmp;
	
}
void CEventHandler::DownMonsterCoor(CWndControl* sender,CMainDlg* parent){
	//Thay đổi tọa độ trong danh sách
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CWndListBox * lstMonster = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstMonsterCoor"));
	int itemSel=lstMonster->GetSelectedIndex();
	if(itemSel>= lstMonster->GetCount()-1 || itemSel<0)
		return;
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;

	CString sTmp = lstMonster->GetItem(itemSel);
	lstMonster->SetItem(itemSel,lstMonster->GetItem(itemSel+1));
	lstMonster->SetItem(itemSel+1,sTmp);
	lstMonster->SetSelectedIndex(itemSel+1);

	
	COOR tmp = m_char->m_lMonsters[itemSel];
	m_char->m_lMonsters[itemSel]=m_char->m_lMonsters[itemSel+1];
	m_char->m_lMonsters[itemSel+1]=tmp;
	
}

void CEventHandler::LoadTrain(CWndControl* sender,CMainDlg* parent){
	
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CWndComboBox * cmbTrain = static_cast<CWndComboBox*> (dlgCtrlContainer->GetControl(L"cmbGoBack"));
	//m_char->m_iTrainMap =_ttoi( cmbTrain->GetItem(cmbTrain->GetSelectedIndex()));
	m_char->m_iTrainMap =m_char->GetMapId( cmbTrain->GetItem(cmbTrain->GetSelectedIndex()));
	parent->MappingObj();
}
void CEventHandler::SavePoint(CWndControl* sender,CMainDlg* parent){
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	m_char->SavePoint();
	m_char->ShowMessage(L"Da luu thong tin bai quai");
}

///////////////////////////tab nhiệm vụ
void CEventHandler::UpAward(CWndControl* sender,CMainDlg* parent){
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CWndListBox * lst = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstAward"));
	int itemSel=lst->GetSelectedIndex();
	if(itemSel<=0)
		return;
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;


	CString sTmp = lst->GetItem(itemSel);
	lst->SetItem(itemSel,lst->GetItem(itemSel-1));
	lst->SetItem(itemSel-1,sTmp);
	lst->SetSelectedIndex(itemSel-1);

	itemSel =itemSel+1;
	byte tmp = m_char->m_arrAward[itemSel];
	m_char->m_arrAward[itemSel]=m_char->m_arrAward[itemSel-1];
	m_char->m_arrAward[itemSel-1] = tmp;
	
}
void CEventHandler::DownAward(CWndControl* sender,CMainDlg* parent){
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CWndListBox * lst = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstAward"));
	int itemSel=lst->GetSelectedIndex();
	if(itemSel>= lst->GetCount()-1 || itemSel<0)
		return;
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;

	CString sTmp = lst->GetItem(itemSel);
	lst->SetItem(itemSel,lst->GetItem(itemSel+1));
	lst->SetItem(itemSel+1,sTmp);
	lst->SetSelectedIndex(itemSel+1);

	itemSel =itemSel+1;
	byte tmp = m_char->m_arrAward[itemSel];
	m_char->m_arrAward[itemSel]=m_char->m_arrAward[itemSel+1];
	m_char->m_arrAward[itemSel+1] = tmp;
	
}
void CEventHandler::QuestInCaveCheck(CWndControl* sender,CMainDlg* parent){
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CWndCheckBox * chkQuestInCave = static_cast<CWndCheckBox*> (dlgCtrlContainer->GetControl(L"chkQuestInCave"));
	CWndCheckBox * chkAutoQuest = static_cast<CWndCheckBox*> (dlgCtrlContainer->GetControl(L"chkAutoQuest"));
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	if( m_char->m_iLevel <90){
		//MessageBoxW(parent->m_hWnd,L"Cấp 90 mói vào được Thiên Tâm Cốc",L"Thông báo",0);
		m_char->ShowMessage(L"Chua du cap vao Thien Tam Coc");
		chkQuestInCave->SetChecked(false);		
		m_char->m_bQuestInCave = false;
		return;
	}
	if(chkQuestInCave->IsChecked()){
		m_char->ShowMessage(L"Chon lich luyen trong Thien Tam Coc");
		chkAutoQuest->SetChecked(true);		
		m_char->m_bAutoQuest = true;
	}
}

///////////////////////////tab party/////////////////////////
void CEventHandler::AutoPartyChecked(CWndControl* sender,CMainDlg* parent){
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CWndCheckBox * chkAutoParty = static_cast<CWndCheckBox*> (dlgCtrlContainer->GetControl(L"chkInterParty"));
	CWndCheckBox * chkAutoQuest = static_cast<CWndCheckBox*> (dlgCtrlContainer->GetControl(L"chkCreateParty"));
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;

	m_char->AutoPartyCheck(0);
	//m_char->AutoPartyCheck(2);
	
}

//=========================NghiaLP--20140204--PARTY============================
//Nhan vao Button de thuc hien
void CEventHandler::AddMemberParty(CWndControl* sender, CMainDlg* parent)
{
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	CWndListBox * lstMember = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstMemberParty"));
	if(lstMember->GetCount()>=6){
		m_char->ShowMessage(L"Party da du thanh vien");
		return;
	}

	CWndTextBox * txtAddMember = static_cast<CWndTextBox*> (dlgCtrlContainer->GetControl(L"txtAddMember"));
	
	CString sMemName = txtAddMember->GetText();
	if(sMemName==L"")
		return;
	txtAddMember->SetText(L"");
	std::vector<CString>::iterator item;
	for(item=  m_char->m_vMemberParty.begin(); item != m_char->m_vMemberParty.end(); item ++){
		if(item->Trim() == sMemName){
			m_char->ShowMessage(L"Thanh vien nay da co trong danh sach");
			return;
			
		}
	}
	//INV_PARTY swMem;
	//swMem._nameMember = m_char->m_swNameMember;
	m_char->m_vMemberParty.push_back(sMemName);
	lstMember->AddItem(sMemName);
	
	//m_char->m_swNameMember = L"";
	

} 
void CEventHandler::RemoveMemberParty(CWndControl* sender, CMainDlg* parent)
{
	//xóa thành viên khỏi danh sách mời
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	CWndListBox * lstMmeber = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstMemberParty"));
	if(lstMmeber->GetSelectedIndex()==-1)
		return;
	m_char->m_vMemberParty.erase(m_char->m_vMemberParty.begin()+lstMmeber->GetSelectedIndex());
	lstMmeber->ClearItems();
	std::vector<CString>::iterator item;
	for(item=  m_char->m_vMemberParty.begin(); item != m_char->m_vMemberParty.end(); item ++){
		lstMmeber->AddItem(item->Trim());
	}
}
void CEventHandler::RemoveAllMember(CWndControl* sender, CMainDlg* parent)
{
	//Xóa tất cả Member trong danh sách
	CWndDlg* dlgCtrlContainer = parent->m_controlDlg;
	CCharacter* m_char = parent->m_clsCurChar;
	CWndListBox * lstMember = static_cast<CWndListBox*> (dlgCtrlContainer->GetControl(L"lstMemberParty"));
	if(m_char==NULL)
		return;
	m_char->m_vMemberParty.clear();
	lstMember->ClearItems();
}
///Nhan btn lưu để đưa du lieu len file xml
void CEventHandler::SaveMemberParty(CWndControl* sender, CMainDlg* parent)
{
	CCharacter* m_char = parent->m_clsCurChar;
	if(m_char==NULL)
		return;
	m_char->SaveMemberInvite();
	
}