/*
-20140613 - SangDH- Update version game 2.36.1
	+ Nâng cấp lên version 1.8.0 thêm tính năng điều đội
- 20140611- SangDH-Change function GetRoleList()
	+ Search Base address


*/


// GameEngine.cpp : implementation file
//

#include "stdafx.h"
#include "GameEngine.h"
#include "GameInfo.h"
#include "Character.h"
#include "Serialize.h"
#include "../HookInj/HookInj.h"
#include <windows.h>
#include <WinBase.h>
#include <tlhelp32.h>
#include "WinHttpClient.h"
#include <string>



const UINT WM_HOOK_WRITE = RegisterWindowMessage(L"WM_HOOK_WRITE");
#define DELAY_VALID_TIME 1200
#define TITLE L"Auto 9K (v1.0.0.0)"


// CGameEngine


using namespace std;
IMPLEMENT_DYNAMIC(CGameEngine, CWnd)

BEGIN_MESSAGE_MAP(CGameEngine, CWnd)
END_MESSAGE_MAP()



CGameEngine::CGameEngine()
{
	bIsExit = !sys::SeDebugPrivileges();
	iOS= sys::DetectOS();
	m_notifyWnd = NULL;
	m_lstRole.clear();
	m_lstCharID.clear();
	bIsValid = true;
	//CheckValid();
	
}

CGameEngine::~CGameEngine()
{
	iOS=0;
	m_lstRole.clear();
	m_lstCharID.clear();
	m_notifyWnd = NULL;
	bIsValid = false;
}
void CGameEngine::RemoveRoleList(){
	bIsExit = true;
	std::map<CString,CCharacter*>::iterator item;
	for(item=  m_lstRole.begin(); item != m_lstRole.end(); item ++){
		m_char = item->second;
		if(m_char->m_bIsHooking){
			m_char->m_bIsHooking = false;
			UnmapDll( m_char-> m_hwnd);
		}
	}
	m_lstRole.clear();
	m_lstCharID.clear();
}




bool CGameEngine::InjectRole(CString sName){
	
	CCharacter* m_char = GetCharacter(sName);
	if(m_char==NULL)
		return false;
	//Lấy HWND của của sổ game hiện tại
	//EnumWindows(EnumWindowsProc, m_char->m_dwProId);
	//m_char->m_hwnd = m_Hwnd;	
	EnumWindows(EnumWindowsProc, (LPARAM)m_char);
	m_lstCharID[(int)m_char->m_hwnd] = sName;
	m_char->Reset();
	m_char->SaveInfo();

	if(sRole==L"")
		sRole =  sName;
	if(!CheckValid())
		return false;
	if(InjectDll( m_char-> m_hwnd)){
		m_char->m_bIsHooking = true;
		
		::SendMessage(m_char->m_hwnd , WM_HOOK_WRITE,MSG_INJECT, (LPARAM)m_notifyWnd->m_hWnd);
		m_char->SetAutoFight();
		return true;
	}
	return false;
}
bool CGameEngine::UnhookRole(CString sName){
	CCharacter* m_char = GetCharacter(sName);
	if(m_char==NULL)
		return false;
	if(m_char->m_bIsHooking){
		m_char->m_bIsHooking = false;
		UnmapDll( m_char-> m_hwnd);
		return true;
	}
	
}
bool CGameEngine::CheckValid(){
	//bIsValid = true;
	//return true;
	if(sRole==L""){
		bIsValid = true;
		return true;
	}
	//wstring url = L"http://124.158.10.127/mapi/login?name=admin&email=admin%40seasoft.vn&password=123456&birthday=02-11-2013";//L"http://localhost/Website/Report.php";
	
	

	wstring url=L"http://autongaokiem.com/process_role_login.php?role_name=" + sRole;
	//wstring url=L"http://autongaokiem.com/process_role_login_test.php?role_name=" + sRole;
    WinHttpClient client(url);
	
    // This method is synchronous and will take a long time.
   // if (client.SendHttpRequest(L"POST"))
	 if (client.SendHttpRequest())
    {
        wstring httpResponseHeader = client.GetHttpResponseHeader();
        wstring httpResponse = client.GetHttpResponse();
		wstring header = httpResponseHeader.c_str();
		wstring result = httpResponse.c_str();
		
		//string sResult = XMLSerialized::UnicodeToUTF8(result);
		
		CString szResult(result.c_str());

		/*string s3(CW2A( sResult.GetString() ) );
		wstring b =UTF8ToUnicode(s3);
		CString str1(CA2W(b));*/
		

	if(szResult==sRole){
			bIsValid = true;
			return bIsValid;
		}
    }
	bIsValid = false;
	RemoveRoleList();
	return bIsValid;
}



void CGameEngine::GetRoleList(){
	PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
	
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	//CCharacter cRole;
		if (Process32First(snapshot, &entry) == TRUE )
		{
		   do
			{
				if (wcscmp (entry.szExeFile, _T(EN_PROC_NAME))==0)
				{  
					HANDLE hnd= OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
					
					CString szName;
					DWORD dwBase = sys::GetBaseAddress(hnd,szName);
					if(dwBase!=NULL && szName.GetLength()>0){

						////Tim trong danh sach, neu ton tai thi lay ra tu danh sach, nguoc lai tao moi
						map<CString,CCharacter*>::const_iterator result = m_lstRole.find(CCharacter::ConvertString( szName));
						if ( result == m_lstRole.end() ){
							m_char = new CCharacter();
							m_char->m_hHandle = hnd;
							m_char->m_iOS = iOS;
							m_char->dwGameBaseAddress = dwBase;
							m_char->m_dwProId = entry.th32ProcessID;
							m_char->Init();


							EnumWindows(EnumWindowsProc,(LPARAM)m_char);
							//m_char->m_hwnd = m_Hwnd;
							if ( m_notifyWnd != NULL && ::IsWindow(m_notifyWnd->m_hWnd) ){		
								//Khởi tạo thông tin cho đối tượng
								m_char->InitInfo();
								m_lstRole.insert(make_pair(m_char->m_szName,m_char));
								//Hiển thị thông tin player lên listview
								m_notifyWnd->SendMessage(UM_ROLE_INFO_CHANGE,reinterpret_cast<WPARAM>(m_char), 1);								
							}
							//tạo thread mới
							THREADPARAM *_param = new THREADPARAM;
							_param->_this = this;
							_param->_char = m_char;
							AfxBeginThread(InfoChanged,_param);
							continue;
						}
					}
					//m_char = new CCharacter();
					//m_char->m_hHandle = hnd;
					//m_char->m_iOS = iOS;
					//m_char->Init();
					////EnumWindows(EnumWindowsProc, entry.th32ProcessID);
					//if(m_char->m_szTitle!= L""){
					//	////Tim trong danh sach, neu ton tai thi lay ra tu danh sach, nguoc lai tao moi
					//	map<CString,CCharacter*>::const_iterator result = m_lstRole.find(m_char->m_szName);
					//	if ( result == m_lstRole.end() ){
					//		m_char->m_dwProId = entry.th32ProcessID;
					//		EnumWindows(EnumWindowsProc,(LPARAM)m_char);
					//		//m_char->m_hwnd = m_Hwnd;
					//		if ( m_notifyWnd != NULL && ::IsWindow(m_notifyWnd->m_hWnd) ){		
					//			//Khởi tạo thông tin cho đối tượng
					//			m_char->InitInfo();
					//			m_lstRole.insert(make_pair(m_char->m_szName,m_char));
					//			//Hiển thị thông tin player lên listview
					//			m_notifyWnd->SendMessage(UM_ROLE_INFO_CHANGE,reinterpret_cast<WPARAM>(m_char), 1);								
					//		}
					//		//tạo thread mới
					//		THREADPARAM *_param = new THREADPARAM;
					//		_param->_this = this;
					//		_param->_char = m_char;
					//		AfxBeginThread(InfoChanged,_param);
					//		continue;
					//	}
					//}
					//delete m_char;
					//m_char=NULL;				
				}
			}while (Process32Next(snapshot, &entry) == TRUE);
		}
}


UINT CGameEngine::AddRoleList( LPVOID pParam ){

	THREADPARAM*    _param = (THREADPARAM*)pParam;
	if(_param->_this->bIsValid == false)
		return 0;
	int i=0;
	do{
		_param->_this->GetRoleList();
		if(i>=DELAY_VALID_TIME){
			_param->_this->CheckValid();
			i=0;
		}
		i++;
		Sleep(500);
		
	}while(!_param->_this->bIsExit && _param->_this->bIsValid );
	return 0;
}


UINT CGameEngine::InfoChanged( LPVOID pParam ){
	THREADPARAM*    _param = (THREADPARAM*)pParam;
	
	CCharacter* curChar = _param->_char;
	if(curChar==NULL)
		return 1;
	
	while (curChar->m_szTitle!= L"" && !_param->_this->bIsExit && _param->_this->bIsValid)
    {
		curChar->GetInfo();	
		_param->_this->m_notifyWnd->SendMessage (UM_ROLE_INFO_CHANGE,reinterpret_cast<WPARAM>(curChar), 0);		


		DWORD test=0;

		test = test | 1|2|4|16;
		if(test&8)
			int  a = 1;
		test = test^8;
		if(test&8)
			int b=1;

		//bool result = ::SendMessage(curChar->m_hwnd, WM_KEYDOWN , VK_TAB, 0xc00f0001);

		//curChar->AutoChat();
		//if(curChar->CheckMapChange())
			//_param->_this->m_notifyWnd->SendMessage (UM_MAP_CHANGED,0, 0);
		//curChar->SearchParty();
		//curChar->OpenGoBackRock();

		if(curChar->m_bIsHooking)
		{

			if(curChar->m_iState==15){//nhân vật chet tu quay ve thanh
				curChar->BackToCity();				
			}
			else{
				curChar->DoBuff();
				curChar->CheckSupport();
				//========NghiaLP-20141803=====Auto Party=============
				
				curChar->InterParty();//Đồng ý vào đội
				curChar->CreateParty();//tao party
			}



			////lam nhiem vu lich luyen
			//if(curChar->m_bAutoQuest && curChar->m_iCurQuest>=0){
			//	if(curChar->ReceiveQuest())
			//		curChar->ProcessQuest();

			//			//curChar->ReceiveAward();
			//}
			//else 
			if(curChar->m_bSendTeamDoQuest || curChar->m_bEscort || (curChar->m_bAutoQuest && curChar->m_iCurQuest>=0)){
				curChar->PriorityQuest();

			}

			//else if(curChar->m_bSendTeamDoQuest)
			//	curChar->SendTeamDoQuest();
			//else if(curChar->ReceiveEscort()){ //===========Van Tieu==============
			//	if(curChar->ProcessEscort())
			//			curChar->ReceiveAward();
			//}
			else if(curChar->m_bAutoFight){//bat tu dong danh
				
				if(curChar->m_bFollowLead){//Nếu có cho theo sau
					//curChar->m_cLead = _param->_this->GetCharacter(curChar->m_sLeadName);
					if(!curChar->GoingtoLead()){
						if(!curChar->PickBag())//kiem tra nhat do (NghiaLP)
							if(curChar->SearchNPC())
								curChar->DoFight();
					}
				}
				else{ //Không theo sau
					if(!curChar->PickBag())
						if(!curChar->MovingToTrainPoint()){
							if(!curChar->SearchNPC()){//tìm không thấy mục tiêu	
								curChar->MovingToTrainPoint(true);
							}
							//						
							else{
								//if(!curChar->GoingToPoint())
								curChar->DoFight();
							}
						}					
				}
			}
		}
	    Sleep(100);
    }
//Neu khong lay duoc thong tin nhan vat thì xóa khỏi danh sách
	
	if(curChar->m_hHandle!= NULL && ::IsWindow(curChar->m_hwnd)) {
		 if(curChar->m_bIsHooking){
			curChar->m_bIsHooking = false;
			UnmapDll( curChar-> m_hwnd);
		}
		 CloseHandle(curChar->m_hHandle);
	}
	if(_param->_this->m_lstRole.size()<=0 || _param->_this->bIsExit)
		return 0;
	if ( _param->_this!=NULL&&_param->_this->m_notifyWnd != NULL && ::IsWindow(_param->_this->m_notifyWnd->m_hWnd) )
		_param->_this->m_notifyWnd->SendMessage(UM_ROLE_REMOVE,reinterpret_cast<WPARAM>(curChar), 0);	
	_param->_this->m_lstRole.erase(curChar->t_szName);

	delete _param;
	_param = NULL;
	return 1;
}



void CGameEngine::SetNotifyWnd(CWnd* notifyWnd)
{
    m_notifyWnd = notifyWnd;
	//set tieu de
	m_notifyWnd->GetParent()->SetWindowTextW(TITLE);


	//Do tim danh sach character mới
	THREADPARAM *_param = new THREADPARAM;
	_param->_this = this;
	_param->_char = NULL;
	AfxBeginThread(AddRoleList,_param);	
}



CCharacter* CGameEngine::GetCharacter(CString sName){
	if(sName==L"")
		return NULL;

	map<CString,CCharacter*>::const_iterator result = m_lstRole.find(sName);
	if ( result != m_lstRole.end() ){
		//m_char = result->second;
		return result->second;
	}
	return NULL;
}




BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD processId;
	CCharacter* m_char = (CCharacter*)lParam;
	GetWindowThreadProcessId(hwnd, &processId);
	if( processId == (DWORD)m_char->m_dwProId && IsMainWindow(hwnd))   
	{      
//		m_Hwnd = hwnd; 
		m_char->m_hwnd = hwnd;
		LPWSTR txtWndTitle = new wchar_t[128];;
		GetWindowTextW(m_char->m_hwnd,txtWndTitle,128);
		CString txt= (CString) txtWndTitle;
		txt.Replace(L"Auto - ",L"");
		SetWindowTextW(m_char->m_hwnd,L"Auto - " +txt);
		return FALSE;;
	}
	return TRUE;
}

BOOL IsMainWindow(HWND handle)
{   
    return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}




