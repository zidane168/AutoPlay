// WndPriority.cpp : implementation file
//

#include "stdafx.h"
#include "WndPriority.h"
#include "MainDlg.h"
#include "../SSC_GameEngine/GameEngine.h"
#include "../SSC_GameEngine/Character.h"
#include "../SSC_GameEngine/Serialize.h"


// CWndPriority dialog
using namespace std;
IMPLEMENT_DYNAMIC(CWndPriority, CDialog)

CWndPriority::CWndPriority(CWnd* pParent /*=NULL*/)
	: CDialog(CWndPriority::IDD, pParent)
{
	Create(CWndPriority::IDD, pParent); 	
	m_arrByte=NULL;
	 if(pParent != NULL){
		 m_wndParent = (CMainDlg*) pParent;
		 m_wndParent->GetParent()->EnableWindow(false);
	 }
	 CCharacter* m_char = m_wndParent->m_clsCurChar;
	 m_mElement.insert(make_pair(1,L"Kim"));
	 m_mElement.insert(make_pair(2,L"Mộc"));
	 m_mElement.insert(make_pair(3,L"Thủy"));
	 m_mElement.insert(make_pair(4,L"Hỏa"));
	 m_mElement.insert(make_pair(5,L"Thổ"));


 //1:Thien Vuong | 2:Thieu Lam | 3:Tieu Giao | 4:Hoa Son | 5:Con Luan | 6:Vo Dang | 7:Dao Hoa Dao | 8:Nga My | 9: Cai Bang | 10:Duong Mon
	 m_mFaction.insert(make_pair(1,L"Thiên Vương"));
	 m_mFaction.insert(make_pair(2,L"Thiếu Lâm"));
	 m_mFaction.insert(make_pair(3,L"Tiêu Dao"));
	 m_mFaction.insert(make_pair(4,L"Hoa Sơn"));
	 m_mFaction.insert(make_pair(5,L"Côn Luân"));
	 m_mFaction.insert(make_pair(6,L"Võ Đang"));
	 m_mFaction.insert(make_pair(7,L"Đào Hoa Đảo"));
	 m_mFaction.insert(make_pair(8,L"Nga My"));
	 m_mFaction.insert(make_pair(9,L"Cái Bang"));
	 m_mFaction.insert(make_pair(10,L"Đường Môn"));	

	
	if(m_char!=NULL){
		
	  //load danh sach ngũ hành hệ phái
		CListBox* lst=  (CListBox*)GetDlgItem(IDC_LSTELEMENT);
		CStatic* lbl=  (CStatic*)GetDlgItem(IDC_TITLE);
		//std::map<int,CString>::const_iterator item;
		int i=0;

		if(m_char->m_iPriority==1){
			for(i=1;i<=ELEMENT_SIZE;i++){
				/*item = m_mElement.find(m_char->m_arrElement[i]);
				if(item!= m_mElement.end())
					lst->AddString(item->second);*/
				lst->AddString(m_mElement[m_char->m_arrElement[i]]);
			}
			lbl->SetWindowTextW(L"Ngũ hành");
		}
		else{
			for(i=1;i<=FACTION_SIZE;i++){
				/*item = m_mFaction.find(m_char->m_arrFaction[i]);
				if(item!= m_mFaction.end())
					lst->AddString(item->second);*/
				lst->AddString(m_mFaction[m_char->m_arrFaction[i]]);
			}
			lbl->SetWindowTextW(L"Môn phái");
		}
	}
}

CWndPriority::~CWndPriority()
{
	m_mElement.clear();
	m_wndParent=NULL;
	m_arrByte = NULL;
}

void CWndPriority::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndPriority, CDialog)
	ON_BN_CLICKED(IDC_BTN_UP, &CWndPriority::OnBtnUpClick)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CWndPriority::OnBtnDownClick)
END_MESSAGE_MAP()

void CWndPriority::OnBtnUpClick(){
	//ngũ hành
	CListBox* lstElement =  (CListBox*)GetDlgItem(IDC_LSTELEMENT);
	int row = lstElement->GetCurSel();
	if(row > 0){
		CCharacter* m_char = m_wndParent->m_clsCurChar;;
		if(m_char!=NULL){
			CString sSelText;
			lstElement->GetText(row, sSelText);
			lstElement->DeleteString(row);
			lstElement->InsertString(row-1,sSelText);
			lstElement->SetCurSel(row-1);

			if(m_char->m_iPriority==1)
				m_arrByte = m_char->m_arrElement;
			else
				m_arrByte = m_char->m_arrFaction;

			byte tmp = m_arrByte[row];
			m_arrByte[row]=m_arrByte[row+1];
			m_arrByte[row+1] = tmp;
		}
	}
}
void CWndPriority::OnBtnDownClick(){
	//ngũ hành
	CListBox* lstElement =  (CListBox*)GetDlgItem(IDC_LSTELEMENT);
	int row = lstElement->GetCurSel();
	if(row >= 0 && row < lstElement->GetCount()-1 ){
		CCharacter* m_char = m_wndParent->m_clsCurChar;;
		if(m_char!=NULL){
			CString sSelText;
			lstElement->GetText(row, sSelText);
			lstElement->DeleteString(row);
			lstElement->InsertString(row+1,sSelText);
			lstElement->SetCurSel(row+1);

			if(m_char->m_iPriority==1)
				m_arrByte = m_char->m_arrElement;
			else
				m_arrByte = m_char->m_arrFaction;

			byte tmp = m_arrByte[row+1];
			m_arrByte[row+1]=m_arrByte[row+2];
			m_arrByte[row+2] = tmp;
			
		}
	}
}

// CWndPriority message handlers
void CWndPriority::OnCancel(){
	if(m_wndParent!=NULL){
		m_wndParent->GetParent()->EnableWindow(true);
		m_wndParent->GetParent()->ShowWindow(SW_SHOW);
		//save info
		if(m_arrByte!=NULL){
			CCharacter* m_char = m_wndParent->m_clsCurChar;;
			if(m_char!=NULL){
				CString result;
				m_char->serialize->ByteArrayToString(result,L"|",m_arrByte);
				if(m_char->m_iPriority==1)
					m_char->serialize->SaveInfo(L"Element",result,TagType::Element,L"/Info");
				else
					m_char->serialize->SaveInfo(L"Faction",result,TagType::Element,L"/Info");
			}
		}
	}
	DestroyWindow();
	delete this;
	
}