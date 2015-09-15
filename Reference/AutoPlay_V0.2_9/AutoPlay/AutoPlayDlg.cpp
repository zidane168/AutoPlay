// AutoPlayDlg.cpp : implementation file
//
// HWND:	HWND cửa sổ window
// HANDLE:	handle Process (Open Process)


#include "stdafx.h"
#include "AutoPlay.h"
#include "AutoPlayDlg.h"
#include "GameInfo.h"
#include "TabHP.h"
#include "TabPK.h"
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include "..\\Controller\\Controller.h"


#define CheckDifferent(a, b)			(a != b)
#define CheckThreeDifferent(a, b, c)	((a != b) && (a != c) && (b != c))

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoPlayDlg dialog
CAutoPlayDlg::CAutoPlayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoPlayDlg::IDD, pParent)
	, m_iTimerDelay(0)
	, m_iRadius(0)
	, m_iSkillID(0)
	, m_bPK(false)
	, m_bMonster(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LstRole, m_lstRole);
	DDX_Control(pDX, IDC_TabConfig, m_tabConfig);
}

BEGIN_MESSAGE_MAP(CAutoPlayDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LstRole, &CAutoPlayDlg::OnLvnItemchangedLstrole)
	ON_WM_TIMER()
	
//	ON_NOTIFY(LVN_ODSTATECHANGED, IDC_LstRole, &CAutoPlayDlg::OnLvnOdstatechangedLstrole)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CAutoPlayDlg::OnBnClickedButtonExec)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAutoPlayDlg message handlers


BOOL CAutoPlayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//SangDH - 2/1/2012 - Add column to list control
	m_lstRole.SetExtendedStyle(m_lstRole.GetStyle()|LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_lstRole.InsertColumn(0,L"Tên nhân vật",LVCFMT_LEFT,160);
	m_lstRole.InsertColumn(1,L"HP/MaxHP",LVCFMT_LEFT,120);
	m_lstRole.InsertColumn(2,L"MP/MaxMP",LVCFMT_LEFT,120);	
	//end

	//SangDH - 2/1/2012 - Add Tab to TabControl
	m_tabConfig.InsertItem(0,L"Hỗ Trợ");
	m_tabConfig.InsertItem(1,L"Chiến Đấu");
	m_tabConfig.Init();
	
	//ThongPH - 20130401 - Load Potion
	if(!((CTabHP*) m_tabConfig.m_tabPages[0])->LoadPotionToList())
	{
		MessageBox(L"Không tìm thấy file cấu hình!", L"Lỗi", MB_OK);
		PostQuitMessage(TRUE);
	}

	m_hCurrentHandle = NULL;
	
	GetAllRole();
	m_bIsRunning = false;
	SetTimer(ID_CLOCK_TIMER, 500, NULL);		

	/*****************************************************************
	Load thong tin tab chien dau
	*********************************************************************/
	
	if(!((CTabPK*) m_tabConfig.m_tabPages[1])->LoadSkillFile(L"skill.ini")){
		MessageBox(L"Không tìm thấy file cấu hình!", L"Lỗi", MB_OK);
		PostQuitMessage(TRUE);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoPlayDlg::OnPaint()
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
HCURSOR CAutoPlayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//SangDH - 2/1/2012 - Xây dựng hàm addData, thêm item vào listview
void CAutoPlayDlg::AddData(CListCtrl &ctrl, int row, int col,CString str)
{
    LVITEM lv;
    lv.iItem = row;
    lv.iSubItem = col;
    lv.pszText =  (LPTSTR) (LPCTSTR)str;
    lv.mask = LVIF_TEXT;
    if(col == 0)
        ctrl.InsertItem(&lv);
    else
        ctrl.SetItem(&lv);  
}
//end

/************************************************************************
SangDH - GetAllRole - 08/04/2013
	lấy tất cả các nhân vật của cửa sổ đang mở
return:
	- !=NULL: có handle tương ứng (cụ thể là tìm được cửa sổ FS)
	- ==NULL: không tìm thấy handle tương ứng
*************************************************************************/
void CAutoPlayDlg::GetAllRole()
{
	PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	CCharacter role;

    if (Process32First(snapshot, &entry) == TRUE)
    {
		int i=0;
       do
        {
			if (wcscmp(entry.szExeFile, L"game.exe")==0)
            {  
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				//m_cRole.m_hHandle = hProcess;
				//m_cRole.GetCurrentPlayerInfo();
				
				role.m_hHandle = hProcess;
				role.GetCurrentPlayerInfo();

				i++;

				// kiểm tra nhân vật online, offline
				//if (IsOffline(m_cRole) == OFFLINE)

				if (IsOffline(role) == OFFLINE)
				{
						CloseHandle(hProcess);
						m_lstRole.DeleteItem(i-1);
						bool exist = false;
						for (int j=0; j<m_lstRole.GetItemCount(); j++){
							CString tmpName = m_lstRole.GetItemText(j, 0);
							if(strName == tmpName)
							{
								exist = true;
								break;
							}
						}
						if(!exist && m_bIsRunning){
							UnmapDll(m_hCurrentWnd);
							if (m_hCurrentHandle)
							{
								CloseHandle(m_hCurrentHandle);
								m_hCurrentHandle = NULL;
							}
							GetDlgItem(IDC_BUTTON_EXEC)->SetWindowTextW(L"Bắt Đầu");
							m_tabConfig.EnableWindow(1);
							m_lstRole.EnableWindow(1);
							SetWindowTextW(L"AutoPlay");
							m_bIsRunning = !m_bIsRunning;
							KillTimer(ID_HP_RESTORE_TIMER);
							KillTimer(ID_CLOCK_TIMER);
							KillTimer(ID_USESKILL_TIMER);
							KillTimer(ID_LAG_TIMER);
							m_iSkillID = 0;
							m_iTimerDelay = 0;
						}
						continue;
				}
				
				//AddUserToList(m_cRole);

				AddUserToList(role);
                CloseHandle(hProcess);
            }
			
        } while (Process32Next(snapshot, &entry) == TRUE);
    }

    CloseHandle(snapshot);
}



/************************************************************************
Sự kiện change của List control, khi clicked vào list control
************************************************************************/
void CAutoPlayDlg::OnLvnItemchangedLstrole(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//int row = m_lstRole.GetNextItem(-1, LVNI_SELECTED);
 	BOOL newSelectionState = (pNMLV->uNewState & LVIS_SELECTED);	// trạng thái chọn hiện tại
    BOOL oldSelectionState = (pNMLV->uOldState & LVIS_SELECTED);	// trạng thái chọn trước đó
	
	if (newSelectionState != LVIS_SELECTED || oldSelectionState != 0)
		return;

	// item đang chọn
	int row = pNMLV->iItem; //m_lstRole.GetNextItem(-1, LVNI_SELECTED);
	
	//ThongPH - 20130401 - Load Character Potion
	//MessageBox(L"Test", L"info", MB_OK);

	// active button 'bắt đầu'
	m_sCurName = m_lstRole.GetItemText(row, 0);
	if(m_sCurName == "" && m_bIsRunning == false)
	{
		GetDlgItem(IDC_BUTTON_EXEC)->EnableWindow(FALSE);
		return;
	}
	GetDlgItem(IDC_BUTTON_EXEC)->EnableWindow(1);

	// hiển thị thông tin máu (load từ file)
	if (!((CTabHP*) m_tabConfig.m_tabPages[0])->LoadCharacterPotion(m_sCurName)){
		((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbHP1.SetCurSel(0);
		((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbHP2.SetCurSel(0);
		((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbHP3.SetCurSel(0);
		((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbMP1.SetCurSel(0);
		((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbMP2.SetCurSel(0);
		((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbMP3.SetCurSel(0);
		((CTabHP*) m_tabConfig.m_tabPages[0])->m_txtPercentHP.SetWindowText(L"0");
		((CTabHP*) m_tabConfig.m_tabPages[0])->m_txtPercentMP.SetWindowText(L"0");
	}
	*pResult = 0;

	// hiển thị thông tin tab PK (load từ process để tìm phái)
	HANDLE hHandle = GetHandleByRoleName(m_sCurName);
	if (hHandle == NULL)
		return;

	m_cRole.m_hHandle = hHandle;
	DWORD dwType = m_cRole.GetPlayerType();	
	m_cRole.GetCurrentPlayerInfo();	
	((CTabPK*) m_tabConfig.m_tabPages[1])->LoadCurrentPos(m_cRole);	

	// không được CloseHandle(hHandle), do bên TabPK đang sửd dụng

	switch (dwType)
	{
		case GIAPSI: case GIAPSI2:
			((CTabPK*) m_tabConfig.m_tabPages[1])->LoadSkillData(L"GiapSi");
			break;
		case DAOSI: case DAOSI2:
			((CTabPK*) m_tabConfig.m_tabPages[1])->LoadSkillData(L"DaoSi");
			break;
		case DINHAN: case DINHAN2:
			((CTabPK*) m_tabConfig.m_tabPages[1])->LoadSkillData(L"DiNhan");
			break;
	}



	//int nItem = 0; //Represents the row number inside CListCtrl
	//for(nItem = 0 ; nItem <  m_lstRole.GetItemCount(); nItem++)
	//{
	//	BOOL bChecked = m_lstRole.GetCheck(nItem);
	//	if( bChecked == 1 )	// checked
	//	{
	//		CString strText = m_lstRole.GetItemText(nItem, 0);
	//		MessageBox(strText);
	//	}
	//}
}

/*************************************************************************
sangdh - 09/04/2013
	Hàm thêm user vào danh sách
param:
	+ Character: Class chứa user (class nằm trong file role.cpp)
*******************************************************************************/
void CAutoPlayDlg::AddUserToList(CCharacter role)
{
	CString strCharHP, strCharMP;
	strCharHP.Format(L"%d/%d", role.m_iHP, role.m_iMaxHP);
	strCharMP.Format(L"%d/%d", role.m_iMP, role.m_iMaxMP);

	int iCurRole = CheckAddUserToListCtrl(role.m_sName);
	if (iCurRole != -1)	// tồn tại gamer trùng trong list, thực hiện update tại vị trí iCurRole
	{	
		AddData(m_lstRole,iCurRole,1, strCharHP);
		AddData(m_lstRole,iCurRole,2, strCharMP);
	}	
	else				// chưa tồn tại gamer này, thực hiện thêm mới
	{
		AddData(m_lstRole,0,0, role.m_sName);
		AddData(m_lstRole,0,1, strCharHP);
		AddData(m_lstRole,0,2, strCharMP);
	}
}

/************************************************************************
VịLH - CheckToExec - 12/04/2013
	Kiểm tra hồi phục HP, MP
************************************************************************/
void CAutoPlayDlg::CheckToRestore(){

	if (!m_hCurrentWnd)
		return;

	m_cRole.m_hHandle = m_hCurrentHandle;
	m_cRole.GetCurrentPlayerInfo();
	if (GetCurrentPercent(m_cRole.m_iHP, m_cRole.m_iMaxHP) < m_iHPPercentConfig)
	{	
		RestoreHP(m_hCurrentWnd,m_iTypeHP1);		
		
		if (CheckDifferent(m_iTypeHP1, m_iTypeHP2))
			RestoreHP(m_hCurrentWnd,m_iTypeHP2);		
		
		if (CheckThreeDifferent(m_iTypeHP1,m_iTypeHP2,m_iTypeHP3))
			RestoreHP(m_hCurrentWnd,m_iTypeHP3);
	}

	if (GetCurrentPercent(m_cRole.m_iMP, m_cRole.m_iMaxMP) < m_iMPPercentConfig)
	{
		RestoreMP(m_hCurrentWnd,m_iTypeMP1);
		
		if (CheckDifferent(m_iTypeMP1, m_iTypeMP2))
			RestoreMP(m_hCurrentWnd,m_iTypeMP2);

		if (CheckThreeDifferent(m_iTypeMP1, m_iTypeMP2, m_iTypeMP3))
			RestoreMP(m_hCurrentWnd,m_iTypeMP3);
	}
}

/***********************************************************************
hàm dò kiểm tra cửa sổ game và thực thi hàm bom máu, sử dụng skill
************************************************************************/

void CAutoPlayDlg::OnTimer(UINT_PTR nIDEvent)
{
	// cập nhật lại listcontrol (remove, thêm role vào list control
	GetAllRole();

	if (!m_bIsRunning)
		return;
	
	//if(isRunning && nIDEvent == ID_HP_RESTORE_TIMER){
	//	CheckToExec();
	//}

	UpdateData(TRUE);
	
	switch (nIDEvent)
	{
		case ID_HP_RESTORE_TIMER:	// hồi phục HP
			CheckToRestore();
			
			break;

		case ID_LAG_TIMER:	// sau khoảng thời gian không đánh được target, ta tìm target khác		
				
				m_cRole.m_hHandle = m_hCurrentHandle;
				m_cRole.GetCurrentPlayerInfo();

				if (m_arrCEnemies.GetCount() > 0)
					m_arrCEnemies.RemoveAll();

				if (m_bPK == TRUE && m_bMonster == FALSE)
					m_cRole.GetMonsterIndexInRadius(m_iRadius, m_arrCEnemies, IM_PLAYER);
				else if (m_bMonster == TRUE && m_bPK == FALSE)
					m_cRole.GetMonsterIndexInRadius(m_iRadius, m_arrCEnemies, IM_MONSTER);
				else if ((m_bMonster == TRUE && m_bPK == TRUE) || (m_bMonster == FALSE && m_bPK == FALSE))
					m_cRole.GetMonsterIndexInRadius(m_iRadius, m_arrCEnemies, IM_PLAYER_MONSTER);

				m_cNearestEnemy = m_cRole.GetIndexOfMinDistance(m_arrCEnemies);
				m_cCurrentMonster = m_cNearestEnemy;				

				KillTimer(ID_LAG_TIMER);				
				m_iTimerDelay = 0;		

				//g_LogWriter.WriteLog(LOG_TYPE_ERROR, L"ID_LAG_TIMER ....");
			break;

		case ID_USESKILL_TIMER:
						
			if (m_cNearestEnemy.m_iHP == 0)	// đã kill thành công monster này, search monster khác
			{	
				m_cRole.m_hHandle = m_hCurrentHandle;
				m_cRole.GetCurrentPlayerInfo();
				
				if (m_arrCEnemies.GetCount() > 0)
					m_arrCEnemies.RemoveAll();
				
				if (m_bPK == TRUE && m_bMonster == FALSE)
					m_cRole.GetMonsterIndexInRadius(m_iRadius, m_arrCEnemies, IM_PLAYER);
				else if (m_bMonster == TRUE && m_bPK == FALSE)
					m_cRole.GetMonsterIndexInRadius(m_iRadius, m_arrCEnemies, IM_MONSTER);
				else if ((m_bMonster == TRUE && m_bPK == TRUE) || (m_bMonster == FALSE && m_bPK == FALSE))
					m_cRole.GetMonsterIndexInRadius(m_iRadius, m_arrCEnemies, IM_PLAYER_MONSTER);

				m_cNearestEnemy = m_cRole.GetIndexOfMinDistance(m_arrCEnemies);	
			}
			else
			{
				if (m_cNearestEnemy.m_iID < 2)
					return;

				COPYDATASTRUCT MyCDS;
				Locksomeoneuseskill locksomeoneStruct;
				locksomeoneStruct.iIndex = m_iSkillID;
				locksomeoneStruct.iPos = m_cNearestEnemy.m_iID;
				
				MyCDS.dwData = DATA_LOCKSOMEONEUSESKILL;
				MyCDS.cbData = sizeof(locksomeoneStruct);
				MyCDS.lpData = &locksomeoneStruct;

				::SendMessage(m_hCurrentWnd, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&MyCDS));		
				::SendMessage(m_hCurrentWnd, WM_HOOK_WRITE, WPARAM_LOCKSOMEONEUSESKILL, 1);			// use skill	

				// cập nhật lại thông tin, để biết enemy đã bị người chơi hạ gục hay chưa?
				m_cNearestEnemy.m_hHandle = m_hCurrentHandle;
				m_cNearestEnemy.GetHP();
	
				if (m_cNearestEnemy.m_iHP == m_cCurrentMonster.m_iHP) // đánh không trúng target hay bị chặn bởi chướng ngại vật (núi,sông..)
				{
					if (m_iTimerDelay == 0)	// chưa set timer delay
						m_iTimerDelay = SetTimer(ID_LAG_TIMER, INTERVAL_LAG, NULL);
				}
				else	// đang đánh quái này
				{
					if (m_iTimerDelay)
					{
						KillTimer(m_iTimerDelay);	// không set nữa
						m_iTimerDelay = 0;
					}
					m_cCurrentMonster = m_cNearestEnemy;
				}								
			}

			break;
	}

	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
/************************************************************************
VịLH - CheckAddUserToListCtrl - 03/04/2013
	kiểm tra có trùng role hay không?
param:	
	+ CString: Tên của người chơi	
return:
	+ int: vị trí 
*************************************************************************/
int CAutoPlayDlg::CheckAddUserToListCtrl(CString curName)
{
	int totalItem = m_lstRole.GetItemCount();	// lấy toàn bộ danh sách trong listView
	if (totalItem == 0)
		return -1;
	
	for (int i=0; i< totalItem; i++)
	{
		CString str0 = m_lstRole.GetItemText(i,0);

		if (str0 == curName )
			return i;	// trả về vị trí của control ListView bị trùng
	}

	return -1;
}

/*************************************************************************/
HWND theHwnd;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD processId;
	theHwnd = NULL;
	GetWindowThreadProcessId(hwnd, &processId);
	if( processId == (DWORD)lParam )   // Found the window - set the current hwnd to some global variable or something.
	{      
		theHwnd = hwnd;
		return FALSE;
	}
	return TRUE;
}

/**************************************************************************
SangDH, ViLH - GetHWNDByRoleName - 21/05/2013
	Lấy hwnd theo role được chọn trong danh sách
params:
	CString: Tên của role cần lấy handle (cửa số cần lấy handle)
return:
	HWND: hwnd của role được chọn
***************************************************************************/
HWND CAutoPlayDlg::GetHWNDByRoleName(CString roleName){
	PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	CCharacter cRole;

    if (Process32First(snapshot, &entry) == TRUE)
    {
       do
        {
			if (wcscmp(entry.szExeFile, L"game.exe")==0)
            {  
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

				// VịLH - gán lại handle mới 

				//m_cRole.m_hHandle = hProcess;
				//CString name = m_cRole.ReadCharacterName();	// tìm tên với handle tương ứng

				cRole.m_hHandle = hProcess;
				CString name = cRole.ReadCharacterName();	// tìm tên với handle tương ứng
				CloseHandle(hProcess);		// close hProcess cho nên ra khỏi hàm này không tìm thấy m_role

				if(name == roleName)	// nếu tên character trùng với tên đang chọn thì trả về handle đó
				{					
					if( entry.th32ProcessID > 0 )
						EnumWindows(EnumWindowsProc, entry.th32ProcessID);	// nếu tồn tại process có id = "game.exe" thì gán vào theHwnd
					
					if(theHwnd)
						return theHwnd;	// xem lại chỗ này, chưa giải phóng snapShot =======================================
					return NULL;
				}
            }
        } while (Process32Next(snapshot, &entry) == TRUE);	// duyệt hết tất cả process trong Windows
    }
    CloseHandle(snapshot);
	return NULL;
}

/************************************************************************
VịLH - OnBnClickedButtonExec - 02/04/2013 (Button: "Bắt đầu")
	Thực hiện Inject Vào ứng dụng
+ isRunning = true:  đang thực hiện Inject
+ isRunning = false: chưa thưc hiện Inject
*************************************************************************/
void CAutoPlayDlg::OnBnClickedButtonExec()
{
	if(m_bIsRunning)
	{
		UnmapDll(m_hCurrentWnd);
		if (m_hCurrentHandle)
		{
			CloseHandle(m_hCurrentHandle);
			m_hCurrentHandle = NULL;
		}

		GetDlgItem(IDC_BUTTON_EXEC)->SetWindowTextW(L"Bắt Đầu");
		m_tabConfig.EnableWindow(1);
		m_lstRole.EnableWindow(1);
		SetWindowTextW(L"AutoPlay");
		m_bIsRunning = !m_bIsRunning;
		KillTimer (ID_HP_RESTORE_TIMER);
		KillTimer (ID_USESKILL_TIMER);
		KillTimer (ID_LAG_TIMER);

		m_iSkillID = 0;
		m_iTimerDelay = 0;
		

		return;
	}

	//ThongPH - 20130401 - Save Character Potion -----------------
	SaveData();
	GetDlgItem(IDC_BUTTON_EXEC)->SetWindowTextW(L"Ngừng");
	m_tabConfig.EnableWindow(0);
	m_lstRole.EnableWindow(0);
	m_bIsRunning = !m_bIsRunning;
	
	// kiểm tra hồi phục HP, MP cho character	------------------	
	m_iHPPercentConfig = ((CTabHP*) m_tabConfig.m_tabPages[0])->m_sldLimitHP.GetPos();
	m_iMPPercentConfig = ((CTabHP*) m_tabConfig.m_tabPages[0])->m_sldLimitMP.GetPos();
	
	m_iTypeHP1 = ((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbHP1.GetCurSel();
	m_iTypeHP2 = ((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbHP2.GetCurSel();
	m_iTypeHP3 = ((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbHP3.GetCurSel();

	m_iTypeMP1 = ((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbMP1.GetCurSel();
	m_iTypeMP2 = ((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbMP2.GetCurSel();
	m_iTypeMP3 = ((CTabHP*) m_tabConfig.m_tabPages[0])->m_cmbMP3.GetCurSel();

	m_tabConfig.EnableWindow(0);
	m_lstRole.EnableWindow(0);

	
	//SangDH - Begin Hook
	if (m_sCurName == L"")
		return;

	m_hCurrentWnd = GetHWNDByRoleName(m_sCurName);	// lấy handle của gamer hiện tại (trong listControl)
	if (!m_hCurrentWnd)
		return;

	// VịLH - get handle process
	DWORD dwPID;
	GetWindowThreadProcessId(m_hCurrentWnd, &dwPID);	
	m_hCurrentHandle = OpenProcess(PROCESS_VM_READ|PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!m_hCurrentHandle)
		return;

	if (InjectDll(m_hCurrentWnd))
		SetWindowTextW(L"Auto - " + m_sCurName);
	
	strName = m_sCurName;
	CheckToRestore();	
	SetTimer(ID_HP_RESTORE_TIMER, INTERVAL_RESTOREHPMP, NULL);

	CheckToUseSkill();		
}

/************************************************************************/
void CAutoPlayDlg::OnClose()
{
	UnmapDll(m_hCurrentWnd);
	if (m_hCurrentHandle)
	{
		CloseHandle(m_hCurrentHandle);
		m_hCurrentHandle = NULL;
	}
	CDialog::OnClose();
}

/************************************************************************
VịLH - GetCurrentPercent - 04/04/2013
	Thực hiện lấy phần trăm số hiện tại
params:
	+ int: số hiện tại
	+ int: số max
*************************************************************************/
int CAutoPlayDlg::GetCurrentPercent(int current, int max)
{
	return current * 100 / max;
}

/************************************************************************
VịLH - IsOffline - 07/05/2013
	Kiểm tra trạng thái online / offline của nhân vật
params:
	role: nhân vật hiện tại
Return:
	int: 0 -> offline
		 1 -> Online
*************************************************************************/
int CAutoPlayDlg::IsOffline(CCharacter role)
{	
	if (role.GetPlayerType() == OFFLINE || role.m_sName == L"")
		return OFFLINE;
	return 1;
}
/************************************************************************
VịLH - RestoreHP - 12/04/2013
	Thực hiện hồi phục máu với loại tương ứng
params:
	+ HWND: handle của cửa sổ chứa nhân vật cần hồi phục máu
	+ int:  loại index item(HP) mà gamer chọn trên giao diện comboBox
*************************************************************************/
void CAutoPlayDlg::RestoreHP(HWND hwnd,int iType)
{	
	::SendMessage(hwnd, WM_HOOK_WRITE, WPARAM_APPLYUSEITEM,  iType);
}

/************************************************************************
VịLH - RestoreMP - 04/04/2013
	Thực hiện hồi phục mana với loại tương ứng	
params:
	+ HWND: handle của cửa sổ chứa nhân vật cần hồi phục máu
	+ int:  loại index item(MP) mà gamer chọn trên giao diện comboBox
*************************************************************************/
void CAutoPlayDlg::RestoreMP(HWND hwnd,int iType)
{
	// iTyle là số thứ tự hiện tại của comboBox, vị trí bình mana đầu tiên là số 11
	::SendMessage(hwnd, WM_HOOK_WRITE, WPARAM_APPLYUSEITEM, iType + 11);	
}

/*************************************************************************
Lưu trữ thông tin của player (hồi phục HP, MP, skill)
*************************************************************************/
void CAutoPlayDlg::SaveData(){
	int row = m_lstRole.GetNextItem(-1, LVNI_SELECTED);	// lấy vị trí dòng hiện tại đang chọn của control
	m_sCurName = m_lstRole.GetItemText(row, 0);
	((CTabHP*) m_tabConfig.m_tabPages[0])->SaveCharacterInfo(m_sCurName);	// lưu trữ thông tin hồi phục HP, MP
	((CTabPK*) m_tabConfig.m_tabPages[1])->SaveCharacterSkill(m_sCurName);	// lưu trữ thông tin sử dụng kỹ năng của từng nhân vật
}

void CAutoPlayDlg::OnOK(){
}

void CAutoPlayDlg::OnCancel()
{
	if ( MessageBox(L"Bạn muốn thoát ứng dụng Auto?",L"Thông Báo", MB_YESNO) == IDYES )
		 CDialog::OnCancel();
}

/**************************************************************************
ViLH - GetHandleByRoleName - 21/05/2013
	Lấy handle theo role được chọn trong danh sách
params:
	CString: Tên của role cần lấy handle (cửa số cần lấy handle)
return:
	HANDLE: handle của role được chọn
***************************************************************************/
HANDLE CAutoPlayDlg::GetHandleByRoleName(CString roleName)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	HANDLE hHandle = NULL;

	CCharacter cRole;

	if (Process32First(snapshot, &entry) == TRUE)
	{
		do
		{
			if (wcscmp(entry.szExeFile, L"game.exe")==0)
			{  
				HANDLE hCurHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

				// VịLH - gán lại handle mới 
				//m_cRole.m_hHandle = hCurHandle;
				//CString name = m_cRole.ReadCharacterName();	// tìm tên với handle tương ứng

				cRole.m_hHandle = hCurHandle;
				CString name = cRole.ReadCharacterName();	// tìm tên với handle tương ứng
								
				if(name == roleName)	// nếu tên character trùng với tên đang chọn thì trả về handle đó
				{					
					if( entry.th32ProcessID > 0 )
						EnumWindows(EnumWindowsProc, entry.th32ProcessID);	// nếu tồn tại process có id = "game.exe" thì gán vào theHwnd

					if(theHwnd)
						hHandle = hCurHandle;
												
					break;
				}
			}
		} while (Process32Next(snapshot, &entry) == TRUE);	// duyệt hết tất cả process trong Windows
	}
	CloseHandle(snapshot);
	return hHandle;
}

/**************************************************************************
ViLH - CheckToUseSkill - 21/05/2013
	Tính năng ra skill
***************************************************************************/

void CAutoPlayDlg::CheckToUseSkill(void)
{
	// lấy thông số trong tab PK		
	
	m_cRole.m_hHandle = m_hCurrentHandle;	// do nhấn nút 'bắt đầu' bên autoplay dlg thì tự động gán currentHandle

	CString sSkillName = ((CTabPK*) m_tabConfig.m_tabPages[1])->m_lstSelSkill.GetItemText(0, 0);
	m_iSkillID = m_cRole.GetSkillIDFromName(sSkillName);

	if (m_iSkillID == 0)
		MessageBox(L"bạn chưa thiết lập skill", L"Infomation", MB_OK | MB_ICONWARNING);

	CString sRadius;
	((CTabPK*) m_tabConfig.m_tabPages[1])->m_txtRadius.GetWindowText(sRadius);	
	m_iRadius = _tstoi(sRadius);

	m_bPK = ((CTabPK*) m_tabConfig.m_tabPages[1])->m_cbPK.GetCheck();
	m_bMonster = ((CTabPK*) m_tabConfig.m_tabPages[1])->m_cbMonster.GetCheck();

	/*CString str;
	str.Format(L"m_iRadius = %d - m_bPK = %d - m_bMonster = %d", m_iRadius, m_bPK, m_bMonster);
	MessageBox(str, L"Info", MB_OK);*/

	if (m_cRole.m_iID == 0)	// không thực hiện pk do chưa đầy đủ dữ liệu
		return;

	// tính khoảng cách của vị trí nhân vật hiện tại so với bán kính 
	// áp dụng trường hợp ngoài thành (đang chiến đấu)		
	m_cRole.GetCurrentPlayerInfo();
	
	if (m_arrCEnemies.GetCount() > 0)
		m_arrCEnemies.RemoveAll();


	if (m_bPK == TRUE && m_bMonster == FALSE)
		m_cRole.GetMonsterIndexInRadius(m_iRadius, m_arrCEnemies, IM_PLAYER);

	else if (m_bMonster == TRUE && m_bPK == FALSE)
		m_cRole.GetMonsterIndexInRadius(m_iRadius, m_arrCEnemies, IM_MONSTER);

	else if ((m_bMonster == TRUE && m_bPK == TRUE) || (m_bMonster == FALSE && m_bPK == FALSE))
		m_cRole.GetMonsterIndexInRadius(m_iRadius, m_arrCEnemies, IM_PLAYER);
	
	m_cNearestEnemy = m_cRole.GetIndexOfMinDistance(m_arrCEnemies);
	SetTimer(ID_USESKILL_TIMER, INTERVAL_USESKILL, NULL);	// set skill			
	m_iTimerDelay = 0;
}
