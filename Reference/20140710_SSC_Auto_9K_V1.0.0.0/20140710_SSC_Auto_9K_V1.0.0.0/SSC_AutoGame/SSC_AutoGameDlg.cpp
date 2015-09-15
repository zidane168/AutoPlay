// SSC_AutoGameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SSC_AutoGame.h"
#include "SSC_AutoGameDlg.h"
#include "../SSC_CONTROLLER/MainDlg.h"
#include "../SSC_GameEngine/GameInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CAboutDlg dialog used for App About

CSSC_AutoGameDlg::CSSC_AutoGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSSC_AutoGameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CSSC_AutoGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ROLE, m_listCwnd);
}

BEGIN_MESSAGE_MAP(CSSC_AutoGameDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ROLE, &CSSC_AutoGameDlg::OnHdnItemchangedListRole)
	ON_BN_CLICKED(ID_EXIT, OnBnClickedCancel)
	ON_MESSAGE(WM_TRAY_MESSAGE,OnTrayNotify)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_COMMAND(IDM_SHOW, OnShow)
	ON_NOTIFY(NM_CLICK, IDC_LNK_COPYRIGHT, OnLinkClick)
	//ON_MESSAGE(MSG_CALLBACK,OnCallBack)
END_MESSAGE_MAP()


// CSSC_AutoGameDlg message handlers

BOOL CSSC_AutoGameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	

	// Tạo màn hình chức năng điều khiển
	CRect clientRect;
    GetClientRect(clientRect);
	CRect rect(0,160,clientRect.Width(),clientRect.Height()-13);
	m_mainDlg = new CMainDlg;
	BOOL status = m_mainDlg ->Create(NULL, NULL, WS_VISIBLE | WS_CHILD , rect,
        this, 1002);
	if ( m_mainDlg != NULL && ::IsWindow(m_mainDlg->m_hWnd ))
    {
		char temp[MAX_PATH];
		CString m_strDir;
		//Tạo UI từ file XML
		if(GetModuleFileNameA( (HMODULE) AfxGetInstanceHandle(), temp, sizeof(temp)))
		{
			char* slash = strrchr(temp, '\\'); // remove the current EXE name from the path
			if(slash)
			{
				*(slash + 1) = '\0';
				m_strDir = temp; // we got the application path 
				m_strDir += "\\GUI.cfg"; // now add the other exe's path
				m_mainDlg->BuildFromXml( m_strDir);
				//Xây dựng giao diện list control
				m_mainDlg->BuildListControl(m_listCwnd);
				return TRUE;  // return TRUE  unless you set the focus to a control
			}
		}
    }
	return FALSE;
}



// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSSC_AutoGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSSC_AutoGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CSSC_AutoGameDlg::OnHdnItemchangedListRole(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->uChanged & LVIF_STATE) // item state has been changed
	{
		int row = pNMLV->iItem; 
		if(pNMLV->uNewState & LVNI_SELECTED){
			m_mainDlg->PostMessageW(WM_ITEM_SELECTED_CHANGE,0,row);
			*pResult = 1;
			return;
		}
		switch(pNMLV->uNewState & 0x3000 )
		{
			case 0x2000: // new state: checked
				m_mainDlg->PostMessageW(WM_ITEM_CHECKED_CHANGE,1,row);
				*pResult = 1;
				return;
			break;
			case 0x1000: // new state: unchecked
				m_mainDlg->PostMessageW(WM_ITEM_CHECKED_CHANGE,0,row);
				*pResult = 1;
				return;
			break;
		}
	}
	*pResult = 0;
}

void CSSC_AutoGameDlg::OnBnClickedCancel()
{
	if(m_mainDlg!= NULL && ::IsWindow(m_mainDlg->m_hWnd)){
	   m_mainDlg->PostMessageW(WM_BTN_CLICK,0,0);
	}
}

void CSSC_AutoGameDlg::OnCancel(){
	HideToTray();	
}

void CSSC_AutoGameDlg::HideToTray() 
{

	m_TrayData.cbSize = sizeof(NOTIFYICONDATA);
		
	m_TrayData.hWnd	= this->m_hWnd;
	
	m_TrayData.uID = 1;
	
	m_TrayData.uCallbackMessage	= WM_TRAY_MESSAGE;

	m_TrayData.hIcon = this->m_hIcon;
	m_TrayData.uTimeout = 1000;

	wcscpy(m_TrayData.szTip,L"SSC Auto game");
		
	m_TrayData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	
	BOOL bSuccess = FALSE;
	BOOL BSus = FALSE;
	if(m_menu == NULL){
		BSus = m_menu.LoadMenu(IDR_MENU1);
		if(!(BSus))
			MessageBox(L"Unabled to Loa menu");
	}

	bSuccess = Shell_NotifyIcon(NIM_ADD,&m_TrayData);

	if(!(bSuccess))
		MessageBox(L"Unable to Set Tary Icon");
	else
	{
		this->ShowWindow(SW_MINIMIZE);
		this->ShowWindow(SW_HIDE);

	}

}

LRESULT CSSC_AutoGameDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uID; 
		UINT uMsg; 
 
		uID = (UINT) wParam;
		uMsg = (UINT) lParam; 
		if (uID != 1)
			return -1;
	
		CPoint pt;	
		switch (uMsg ) 
		{ 
		case WM_LBUTTONDBLCLK:
			OnShow();
			break;
	
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			GetCursorPos(&pt);
			m_menu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,pt.x,pt.y,this);
			break;
		} 
		return 0; 
}
void CSSC_AutoGameDlg::OnLinkClick(NMHDR *pNMHDR, LRESULT *pResult) 
{ 
   PNMLINK pNMLink = (PNMLINK) pNMHDR;   

   ShellExecuteW(NULL, L"open", pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL); 

   *pResult = 0; 
}

LRESULT CSSC_AutoGameDlg::OnCallBack(WPARAM wParam, LPARAM lParam){
	if(m_mainDlg!= NULL && ::IsWindow(m_mainDlg->m_hWnd)){
		//GetWindowTextW(m_sName);
		//SetWindowTextW(EN_TITLE);
		m_mainDlg->SendMessageW(MSG_CALLBACK,wParam,lParam);
	}
	return 1;
}
void CSSC_AutoGameDlg::OnExit(){
	//Shell_NotifyIcon(NIM_DELETE,&m_TrayData);
	if(m_mainDlg!= NULL && ::IsWindow(m_mainDlg->m_hWnd)){
	   m_mainDlg->PostMessageW(WM_BTN_CLICK,0,0);
	}
}

void CSSC_AutoGameDlg::OnShow(){
	Shell_NotifyIcon(NIM_DELETE,&m_TrayData);
	this->ShowWindow(SW_RESTORE);
	this->ShowWindow(SW_SHOW);
}