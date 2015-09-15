// Filename: MainDlg.cpp
// 18-Nov-2013 SSC copy right.

#include "stdafx.h"
#include "MainDlg.h"
#include "WndDlg.h"
#include "GroupControl.h"
#include "TabControl.h"
#include "XmlToControl.h"
#include "WndControl.h"
#include "../SSC_AutoGame/Resource.h"
#include "EventHandler.h"
#include "../SSC_GameEngine/GameEngine.h"
#include "../SSC_GameEngine/Character.h"
#include "../SSC_GameEngine/Serialize.h"
#include "../SSC_GameEngine/GameInfo.h"
#include "GUI_Mapping.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;


// MainDlg

IMPLEMENT_DYNAMIC(CMainDlg, CWnd)
BEGIN_MESSAGE_MAP(CMainDlg, CWnd)
    ON_WM_CREATE()
    ON_WM_MOUSEACTIVATE()
    ON_WM_SETFOCUS()
    ON_WM_SIZE()
	ON_MESSAGE(WM_TAB_SELECTION_CHANGE, OnTabSelectionChange)
	ON_MESSAGE(WM_ITEM_SELECTED_CHANGE, OnItemStateChanged)
	ON_MESSAGE(WM_ITEM_CHECKED_CHANGE, OnItemCheckedChanged)
	ON_MESSAGE(WM_BTN_CLICK, OnBtnExitClick)
	ON_MESSAGE(UM_ROLE_INFO_CHANGE, OnRoleChanged)
	ON_MESSAGE(UM_ROLE_REMOVE, OnRoleOut)
	//ON_MESSAGE(UM_MAP_CHANGED, OnMapChanged)
	ON_MESSAGE(MSG_CALLBACK,OnCallBack)
	
END_MESSAGE_MAP()


CMainDlg::CMainDlg()
{
	m_hWndFocus = 0;
	m_tabWnd = NULL;
    m_controlDlg = NULL;
	m_listCtrl = NULL;
	m_clsCurChar = NULL;
	m_mapping = NULL;
//	m_wndSkill= NULL;
}

CMainDlg::~CMainDlg()
{
	
	m_clsCurChar = NULL;
	if(m_engine!=NULL)
		delete m_engine;
	m_engine = NULL;
	delete m_tabWnd;
    m_tabWnd = NULL;
		
	events->ClearEventHandler();
	delete  events;
	if(m_mapping!=NULL){
		m_mapping->ClearMapping();
		delete m_mapping;
	}
	m_mLstMap.clear();
//	delete m_wndSkill;
//	m_wndSkill= NULL;
    DeleteGroups();
    DeleteControls();
}

// MainDlg message handlers

void CMainDlg::BuildFromXml(const CString& xmlFile)
{
    CXmlToControl controlXml;
	if(! controlXml.ParseXml(xmlFile, m_topLevelGroupsList, m_allControlsList)){
		MessageBox(L"Err1001: Lỗi tạo giao diện ứng dụng.",L"Lỗi 1001", MB_OK);
		PostQuitMessage(0);
		return;
	}

	list<CGroupControl*>::iterator i = m_topLevelGroupsList.begin();
    for( ; i != m_topLevelGroupsList.end(); i++)
    {
		m_tabWnd->AddTabs(*i);
    }
   list<CWndControl*>::iterator j = m_allControlsList.begin();
    for( ; j != m_allControlsList.end(); j++)
    {
        CWndControl* pControl = *j;
		
        pControl->AddEventHandler(this);
    }
	m_tabWnd->SetNotifyWnd(this);

	CRect clientRect;
    GetClientRect(clientRect);
	int cnt =  m_tabWnd->GetRowCount();

	// Resize and reposition the control dialog.
    if ( m_controlDlg != NULL && ::IsWindow(m_controlDlg->m_hWnd) )
    {
		m_controlDlg->SetWindowPos(NULL, 14,33 + (cnt-1)*19,
            clientRect.Width()-30, clientRect.Height()-(48+(cnt-1)*19),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

	UpdateControlDlg();
	m_controlDlg->EnableWindow(0);
	//Đăng ký events
	events = new CEventHandler();

////tab Chiến đấu
	events->AddEventHandler(L"cmbPriority",&CEventHandler::SetPriority);
	events->AddEventHandler(L"btnConfigPK",&CEventHandler::BtnConfigSkill);
	events->AddEventHandler(L"btnConfigBuff",&CEventHandler::BtnConfigBuff);
	events->AddEventHandler(L"btnPriority",&CEventHandler::BtnConfigPriority);
	events->AddEventHandler(L"chkFightToDie",&CEventHandler::ChkFightToDie);
	events->AddEventHandler(L"chkAutoFight",&CEventHandler::ChkAutoFight);
//
////Tab bãi quái
	events->AddEventHandler(L"btnAddCoor",&CEventHandler::AddMonsterCoor);
	events->AddEventHandler(L"btnRemoveCoor",&CEventHandler::RemoveMonsterCoor);
	events->AddEventHandler(L"btnUpCoor",&CEventHandler::UpMonsterCoor);
	events->AddEventHandler(L"btnDownCoor",&CEventHandler::DownMonsterCoor);
	events->AddEventHandler(L"btnClearAll",&CEventHandler::RemoveAllMonsterCoor);
	events->AddEventHandler(L"cmbGoBack",&CEventHandler::LoadTrain);
	events->AddEventHandler(L"btnSaveCoor",&CEventHandler::SavePoint);

	//tab nhiệm vụ
	events->AddEventHandler(L"btnUpAward",&CEventHandler::UpAward);
	events->AddEventHandler(L"btnDownAward",&CEventHandler::DownAward);
	events->AddEventHandler(L"chkQuestInCave",&CEventHandler::QuestInCaveCheck);

	///Tab party
	events->AddEventHandler(L"chkInterParty",&CEventHandler::AutoPartyChecked);

	events->AddEventHandler(L"btnAddMember",&CEventHandler::AddMemberParty);
	events->AddEventHandler(L"btnRemoveMember",&CEventHandler::RemoveMemberParty);
	events->AddEventHandler(L"btnClearAllMember",&CEventHandler::RemoveAllMember);
	events->AddEventHandler(L"btnSaveParty",&CEventHandler::SaveMemberParty);
}



void CMainDlg::PostNcDestroy()
{
    // Delete the C++ instance.
    delete this;
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( !CreateTabWnd() )
    {
        return -1;
    }
    if ( !CreateControlDlg() )
    {
        return -1;
    }

    return 0;
}

int CMainDlg::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    int status = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);

    SetFocus();

    return status;
}

void CMainDlg::OnSetFocus(CWnd* pOldWnd)
{
    ASSERT( m_controlDlg != NULL );

    CWnd::OnSetFocus(pOldWnd);

    if ( ::IsWindow(m_controlDlg->m_hWnd) )
    {
        m_controlDlg->SetFocus();
    }
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    CRect clientRect;
    GetClientRect(clientRect);
	
    // Resize and reposition the tab wnd.
   
    if ( m_tabWnd != NULL && ::IsWindow(m_tabWnd->m_hWnd) )
    {
        m_tabWnd->SetWindowPos(NULL, 0, 0,
            clientRect.Width(), clientRect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }
}



LRESULT CMainDlg::OnTabSelectionChange(WPARAM wParam, LPARAM lParam)
{
	
    UpdateControlDlg();
	MappingObj();
    return 0;
}

bool CMainDlg::CreateTabWnd()
{
    // Create tab wnd instance.
	m_tabWnd = new CTabControl();

    // Create the tab control GUI.
    CRect rect(0,0,0,0);
	DWORD dwStyle = TCS_TABS | WS_CHILD | WS_VISIBLE | TCS_MULTILINE | TCS_FIXEDWIDTH;
    if ( !m_tabWnd->Create( dwStyle, rect, this, IDD_TABCONTRL) )
    {        
        return false;
    }
    return true;
}

bool CMainDlg::CreateControlDlg()
{
    // Create the control dialog instance. This will create the GUI instance as well.
	m_controlDlg = new CWndDlg(m_tabWnd);

    return true;
}


void CMainDlg::UpdateControlDlg()
{
    // Record current focus wnd so that we can restore it later.    
    SaveFocus();

    // Remove the controls from control dlg.
    m_controlDlg->RemoveAllControls();

    // Get the currently selected group from tab wnd.
    CGroupControl* group = m_tabWnd->GetSelectedGroup();
    if ( group != NULL )
    {
        // Get the controls belonging to this control group.
        list<CWndControl*> controlList;
        group->GetControls(controlList);

        // Add these controls to the control dlg.
        if ( controlList.size() > 0 )
        {
            m_controlDlg->AddControls(controlList);            
        }
    }
    // Restore focus.
    RestoreFocus();
}

void CMainDlg::SaveFocus()
{
    // Record current focus wnd so that we can restore it later.    
    m_hWndFocus = ::GetFocus();
}

void CMainDlg::RestoreFocus()
{
    if ( m_hWndFocus != ::GetFocus() && ::IsWindow(m_hWndFocus) )
        ::SetFocus(m_hWndFocus);
}

void CMainDlg::DeleteGroups()
{
    list<CGroupControl*>::iterator iter = m_topLevelGroupsList.begin();
    for( ; iter != m_topLevelGroupsList.end(); ++iter)
    {
        delete *iter;
    }
    m_topLevelGroupsList.clear();
}

void CMainDlg::DeleteControls()
{
    list<CWndControl*>::iterator iter = m_allControlsList.begin();
    for( ; iter != m_allControlsList.end(); ++iter)
    {
        delete *iter;
    }
    m_allControlsList.clear();
}


void CMainDlg::BuildListControl(CListCtrl& ctrl){
	m_listCtrl = &ctrl;
	m_listCtrl->SetExtendedStyle(m_listCtrl->GetStyle()|LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_listCtrl->InsertColumn(0,L"Tên nhân vật",LVCFMT_LEFT,100);
	m_listCtrl->InsertColumn(1,L"HP/MaxHP",LVCFMT_CENTER,70);
	m_listCtrl->InsertColumn(2,L"MP/MaxMP",LVCFMT_CENTER,70);	

	//Get Role Info
	m_engine = new CGameEngine();
	m_engine->SetNotifyWnd(this);
	//m_engine->GetRoleList();
}


//Add data to list control
int CMainDlg::AddData(int row,int col, const CString str)
{	
	LVFINDINFO info;
	LVITEM lv;
	int iExist;
	//Thêm mới Item
	if(col==0){
		if( str.GetLength() == 0 ){
			return -1;
		}
		info.flags = LVFI_STRING;
		info.psz = (LPTSTR)(LPCTSTR) str;
		iExist = m_listCtrl->FindItem(&info);
		//nếu đã có item thì trả về vị trí Index của item
		if(iExist !=-1){
			return iExist;
		}
	}
	//nếu chưa tồn tại hoặc là insert subItem
	//nếu không xác định row thì tạo row tiếp theo
	if(row ==-1){
		row = m_listCtrl->GetItemCount();
		col = 0;
	}
    lv.iItem = row;
    lv.iSubItem = col;
    lv.pszText = (LPTSTR)(LPCTSTR) str;
    lv.mask = LVIF_TEXT;
	if(col == 0){
		return m_listCtrl->InsertItem(&lv);
	}
    else
        m_listCtrl->SetItem(&lv); 
	
	return -1;
}


LRESULT CMainDlg::OnRoleChanged(WPARAM wParam, LPARAM lParam){
	
	CCharacter* m_char = reinterpret_cast<CCharacter*> (wParam);
	if(m_char==NULL || m_char->m_iCurHP<0){
		return 0;
	}
	//Nếu có char mới thì mapping với giao diện
	if((int)lParam==1){
		m_mapping = new CGUIMapping(this);
		m_mapping->AddMapping(L"txtRange",ControlProp::Value,(DWORD)&m_char->m_iRange);
		m_mapping->AddMapping(L"cmbPriority",ControlProp::Selected,(DWORD)&m_char->m_iPriority);
		m_mapping->AddMapping(L"chkFollowPK",ControlProp::Check,(DWORD)&m_char->m_bPkFollowLead);
		m_mapping->AddMapping(L"chkAutoRide",ControlProp::Check,(DWORD)&m_char->m_bAutoRide);
		m_mapping->AddMapping(L"chkPersonFirst",ControlProp::Check,(DWORD)&m_char->m_bPkFirst);
		m_mapping->AddMapping(L"chkBlackList",ControlProp::Check,(DWORD)&m_char->m_bBlackList);
		m_mapping->AddMapping(L"chkFightToDie",ControlProp::Check,(DWORD)&m_char->m_bFightToDie);
		m_mapping->AddMapping(L"chkNotFollow",ControlProp::Check,(DWORD)&m_char->m_bNotFollow);
		//m_mapping->AddMapping(L"chkFollowLead",ControlProp::Check,(DWORD)&m_char->m_bFollowLead);
		m_mapping->AddMapping(L"txtDistance",ControlProp::Value,(DWORD)&m_char->m_iDistance);
		m_mapping->AddMapping(L"chkGoto",ControlProp::Check,(DWORD)&m_char->m_bAutoGoto);
		m_mapping->AddMapping(L"chkRevival",ControlProp::Check,(DWORD)&m_char->m_bAutoRevival);
		m_mapping->AddMapping(L"chkAutoFight",ControlProp::Check,(DWORD)&m_char->m_bAutoFight);
		m_mapping->AddMapping(L"cmbCombo",ControlProp::Selected,(DWORD)&m_char->m_iKeyFight);
		m_mapping->AddMapping(L"txtRestorHP",ControlProp::Value,(DWORD)&m_char->m_iHpLimit);
		m_mapping->AddMapping(L"cmbRestorHP",ControlProp::Selected,(DWORD)&m_char->m_iKeyHP);
///////tab bãi quái
		m_mapping->AddMapping(L"txtRangePoint",ControlProp::Value,(DWORD)&m_char->m_iRangePoint);
		m_mapping->AddMapping(L"chkAutoEat",ControlProp::Check,(DWORD)&m_char->m_bAutoEat);
		m_mapping->AddMapping(L"cmbRestorMP",ControlProp::Selected,(DWORD)&m_char->m_iKeyMP);
		m_mapping->AddMapping(L"chkAutoGoBack",ControlProp::Check,(DWORD)&m_char->m_bAutoBackToTrain);
		//======NghiaLP=============
		m_mapping->AddMapping(L"chkAutoPickBag",ControlProp::Check,(DWORD)&m_char->m_bPickBag);

		///======ket thuc======
//=======Tab Nhiem vu
		m_mapping->AddMapping(L"chkAutoQuest",ControlProp::Check,(DWORD)&m_char->m_bAutoQuest);
		m_mapping->AddMapping(L"chkQuickAward",ControlProp::Check,(DWORD)&m_char->m_bQuickAward);
		m_mapping->AddMapping(L"chkQuestInCave",ControlProp::Check,(DWORD)&m_char->m_bQuestInCave);
		
		
//======NghiaLP_Party=============
		m_mapping->AddMapping(L"chkInterParty",ControlProp::Check,(DWORD)&m_char->m_bInterParty);
		m_mapping->AddMapping(L"txtNameRequest",ControlProp::Text,(DWORD)&m_char->m_szNameParty);

		m_mapping->AddMapping(L"chkCreateParty",ControlProp::Check,(DWORD)&m_char->m_bCreateParty);
		
		//=============Van Tieu==========
		m_mapping->AddMapping(L"chkEscort",ControlProp::Check,(DWORD)&m_char->m_bEscort);
		
		//=============Dieu Doi==================================
		m_mapping->AddMapping(L"chkSendTeam",ControlProp::Check,(DWORD)&m_char->m_bSendTeamDoQuest);
		m_mapping->AddMapping(L"chkPriority1",ControlProp::Check,(DWORD)&m_char->m_bBC);
		m_mapping->AddMapping(L"cmbKindBC",ControlProp::Selected,(DWORD)&m_char->m_iKindBC);
		m_mapping->AddMapping(L"chkPriority2",ControlProp::Check,(DWORD)&m_char->m_bTVP);
		m_mapping->AddMapping(L"chkPriority3",ControlProp::Check,(DWORD)&m_char->m_bVT);
		m_mapping->AddMapping(L"chkPriority4",ControlProp::Check,(DWORD)&m_char->m_bTN);
		m_mapping->AddMapping(L"chkPriority5",ControlProp::Check,(DWORD)&m_char->m_bTVPhai);
		

		///======ket thuc======





		m_mLstMap.insert(make_pair(m_char->m_szName,m_mapping));

		//Thêm vào danh sách theo sau
		//CWndComboBox* cmbFollow = static_cast<CWndComboBox*> (m_controlDlg->GetControl(L"cmbFollow"));
		//cmbFollow->AddItem(m_char->m_szName);
	}
	int iRow;
		//insert Item
		
	iRow = AddData(-1,0,m_char->m_szName);
		
	////Insert subitem
	if(iRow !=-1){
		CString val;
		val.Format(L"%d/%d", m_char->m_iCurHP,m_char->m_iMaxHP);
		AddData(iRow,1,val);
		val.Format( L"%d/%d",m_char->m_iCurMP,m_char->m_iMaxMP);
		AddData(iRow,2,val);
	}
	return 1;
}

LRESULT CMainDlg::OnRoleOut(WPARAM wParam, LPARAM lParam){
	CCharacter* m_char = reinterpret_cast<CCharacter*> (wParam);
	if(m_char==NULL){
		return 0;
	}
	CString sName = m_char->t_szName;
	//xóa danh sách mapping của nhân vật
	if(m_mLstMap.size()>0)
		m_mLstMap.erase(sName);
	
	//Xóa nhân vật khỏi danh sách
	LVFINDINFO info;

	int nItem;
	
	info.flags = LVFI_STRING;
	info.psz = (LPTSTR)(LPCTSTR) sName;
	nItem = m_listCtrl->FindItem(&info);
	//nếu không có item cần xóa
	if(nItem ==-1){
		return 0;
	}
	m_listCtrl->DeleteItem(nItem);

	return 1;
}

LRESULT CMainDlg::OnItemStateChanged(WPARAM wParam, LPARAM lParam)
{
	//select	
	int row = lParam;
    if(row < 0)
        return 0;
	CString m_szSelCharName = m_listCtrl->GetItemText(row, 0);
	if(m_szSelCharName==L"")
		return 0;
	//Load thông tin đối tượng lên giao diện
	m_clsCurChar = m_engine->GetCharacter(m_szSelCharName);
	if(m_clsCurChar==NULL){
		return 0;
	}
	if(m_mLstMap.size()==0)
		return 0;
	m_mapping = m_mLstMap[m_szSelCharName];
	MappingObj();
	return 1;
}
LRESULT CMainDlg::OnItemCheckedChanged(WPARAM wParam, LPARAM lParam)
{
	//select	
	int row = lParam;
	bool checked = (int)wParam==1;
    if(row < 0)
        return 0;
	CString m_szSelCharName = m_listCtrl->GetItemText(row, 0);
	if(m_szSelCharName==L"")
		return 0;
	if(checked)
		m_engine->InjectRole(m_szSelCharName);	
	else
		m_engine->UnhookRole(m_szSelCharName);
	return 1;
}
LRESULT CMainDlg::OnCallBack(WPARAM wParam, LPARAM lParam){
	
	CString strName = m_engine->m_lstCharID[(int)wParam];
	CCharacter* cChar = m_engine->GetCharacter(strName);
	if(cChar==NULL)
		return 0;
	if((int)lParam==1)
		cChar ->m_bAutoFight = true;
	else
		cChar ->m_bAutoFight = false;
	
	//MessageBoxW(*str,L"a");
	return 1;
}

LRESULT CMainDlg::OnBtnExitClick(WPARAM wParam, LPARAM lParam){
	
	if ( MessageBox(L"Bạn muốn thoát ứng dụng Auto?",L"Thông Báo", MB_YESNO) == IDYES ){
		events->ClearEventHandler();
		if(m_engine!=NULL){
			m_engine->RemoveRoleList();
			Sleep(50);
			delete m_engine;
			m_engine = NULL;
		}
		this->GetParent()->DestroyWindow();
//		::PostQuitMessage(0);
	}
	return 1;
}

void CMainDlg::MappingObj(){
	
	if(m_clsCurChar==NULL){
		m_controlDlg->EnableWindow(0);
		return;
	}
	m_controlDlg->EnableWindow(1);
	////tab bãi quái
	m_clsCurChar->LoadPoint();
	if(m_mapping!=NULL)
		m_mapping->ObjToGUI();
	
	

	if(m_tabWnd->GetCurSel()==1){
		
		CWndListBox * lstMonster = static_cast<CWndListBox*> (m_controlDlg->GetControl(L"lstMonsterCoor"));
		lstMonster->ClearItems();
		std::vector<COOR>::iterator item;
		for(item=  m_clsCurChar->m_lMonsters.begin(); item != m_clsCurChar->m_lMonsters.end(); item ++){
			CString sCoor;
			sCoor.Format(L"%d,%d",item->_iX ,item->_iY);
			lstMonster->AddItem(sCoor);
		}
		
		//hiển thị danh sách map đã lưu
		CWndComboBox * cmbTrain = static_cast<CWndComboBox*> (m_controlDlg->GetControl(L"cmbGoBack"));
		if(cmbTrain==NULL)
			return;
		cmbTrain->ClearItems();
		std::map<int,CString> lst;
		m_clsCurChar->LoadTrain(lst);
		std::map<int,CString>::iterator iter;
		int i=0;
		int index=0;
		cmbTrain->AddItem(L"0");
		for(iter=lst.begin();iter!=lst.end();iter++){
			cmbTrain->AddItem(m_clsCurChar->GetMapName(_ttoi(iter->second)));
			if(_ttoi(iter->second) == m_clsCurChar->m_iTrainMap)
				index=i+1;
			i++;
		}
		cmbTrain->SetSelectedIndex(index);
		return;
	}
	if(m_tabWnd->GetCurSel()==3){//tab nhiệm vụ lịch luyện
		CWndListBox * lstAward = static_cast<CWndListBox*> (m_controlDlg->GetControl(L"lstAward"));
		if(lstAward==NULL)
			return;
		lstAward->ClearItems();
		for(int i=1;i<=m_clsCurChar->m_arrAward[0];i++){
			int index = m_clsCurChar->m_arrAward[i];
			
			lstAward->AddItem(m_clsCurChar->m_mAward[index]);
		}
	}
	if(m_tabWnd->GetCurSel()==2){//tab tổ đội
		m_clsCurChar->LoadMemberParty();
		std::vector<CString>::iterator name_member;
		//INV_PARTY* objmem;
		CWndListBox * lstMember = static_cast<CWndListBox*> (m_controlDlg->GetControl(L"lstMemberParty"));
		lstMember->ClearItems();
		for(name_member = m_clsCurChar->m_vMemberParty.begin(); name_member != m_clsCurChar->m_vMemberParty.end(); name_member++)
		{
			lstMember->AddItem(name_member->Trim());
		}
	}
}

//Control Event Handle
void CMainDlg::HandleWndEvent(const CWndEvent& ev)
{
//	CCharacter* m_char = m_engine->GetCharacter(m_szSelCharName);
	//::SendMessage(m_char-> m_hwnd , WM_HOOK_WRITE, 0,  0);
	if(m_clsCurChar==NULL)
		return;
	
	if(m_mapping!=NULL){
		m_mapping->GUIToObj(ev.GetSender()->GetName());
	}
	CEventHandler::function func = events->RaiseEvent(ev.GetSender()->GetName());
	if(func != NULL)
		func(ev.GetSender(),this);

	//Lưu thông tin nhân vật vào file
	m_clsCurChar->serialize->SaveInfo();
}
