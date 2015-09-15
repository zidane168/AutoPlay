// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Controller.h"
#include "..\\AutoPlay\\GameInfo.h"


/************************************************************************/
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hModuleDll = (HINSTANCE) hModule;
		DisableThreadLibraryCalls(hModuleDll);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

/************************************************************************
VịLH - LockSomeOneUseSkill (09/05/2013)
	Hàm ra skill với nhân vật khác 
param:
	DWORD: process id	
	int: index thuộc mảng của NPC
	int: index của skill	
************************************************************************/
void LockSomeOneUseSkill(DWORD pid, int iPos, int iIdSkill)
{
	HANDLE phandle = OpenProcess(PROCESS_VM_READ,   0, pid); // Get permission to read
	if (!phandle)	// if handle == NULL --> thoát
		return;		

	/*
	CPU Disasm
	Address   Hex dump          Command                                  Comments	
	0056403D  |.  8B8424 B00000 MOV EAX,DWORD PTR SS:[ESP+0B0]
	00564044  |.  8B0D 48EF7500 MOV ECX,DWORD PTR DS:[75EF48]            ; PTR to ASCII "`GY"
	0056404A  |.  50            PUSH EAX
	0056404B  |.  8B4424 78     MOV EAX,DWORD PTR SS:[ESP+78]
	0056404F  |.  8B11          MOV EDX,DWORD PTR DS:[ECX]
	00564051  |.  50            PUSH EAX
	00564052  |.  FF52 38       CALL DWORD PTR DS:[EDX+38]               ; -- LockSomeoneUseSkill   CoreClient:CoreShell
	*/

	DWORD dwEcxAddress;		
	ReadProcessMemory(phandle, (void*)FN_ECX_BASEADDRESS_LOCKSOMEONEUSESKILL, &dwEcxAddress, (DWORD)sizeof(dwEcxAddress), 0);	
	CloseHandle(phandle);	

	DWORD dwFunctionAddress = FN_FUNCTION_BASEEADDRESS_LOCKSOMEONEUSESKILL;
	__asm
	{
		PUSHAD
		PUSHFD		
		PUSH iIdSkill	// para 2		
		PUSH iPos		// para 1
		MOV ECX, dwEcxAddress		
		CALL dwFunctionAddress	
		POPFD
		POPAD
	}
}



/************************************************************************
VịLH - ApplyUseItem - 11/04/2013
	Thực hiện việc sử dụng item để hồi phục HP, MP
param:
	+ DWORD: process id
	+ int: loại item (HP, MP) cần sử dụng mà user chọn trên giao diện comboBox
**************************************************************************/
#define TYPE_FIRSTPACKAGE	0x3
#define TYPE_SECONDPACKAGE	0x10
#define TYPE_THIRDPACKAGE	0xB
/*************************************************************************/
void ApplyUseItem(DWORD pid, int itemtype)
{	
	HANDLE phandle = OpenProcess(PROCESS_VM_READ,   0, pid); // Get permission to read
	if (!phandle)	// if handle == NULL --> thoát
		return;		

	/*
	VịLH - ApplyUseItem trong ASM (5 parameters)
		+ 1. Parameter 1: Id item (Hexa)
		+ 2. Parameter 2: vị trí đề (Hexa)	
		- 3: nằm trong hành trang 1
		- 10: nằm trong hành trang số 2
		- 0xB: nằm ở 4 vị trí bên dưới (hành trang số 3)
		+ 3. Vị trí cột trong hành trang (Dec)
		+ 4. Vị trí dòng trong hành trang (Dec)
		+ 5. luôn luôn bằng 0; (Dec)
	
	CPU Disasm
	Address   Hex dump          Command                                  Comments
	0058C4E5  |.  A1 942B9100   MOV EAX,DWORD PTR DS:[912B94]            ; |		
	0058C4EA  |.  8BCC          MOV ECX,ESP                              ; |
	0058C4EC  |.  56            PUSH ESI                                 ; |Arg1
	0058C4ED  |.  8911          MOV DWORD PTR DS:[ECX],EDX               ; |Arg2 => [LOCAL.13]
	0058C4EF  |.  8959 04       MOV DWORD PTR DS:[ECX+4],EBX             ; |Arg3
	0058C4F2  |.  8979 08       MOV DWORD PTR DS:[ECX+8],EDI             ; |Arg4
	0058C4F5  |.  8D88 C0BA0200 LEA ECX,[EAX+2BAC0]                      ; |		
	*/

	DWORD dwValueTemp;		
	ReadProcessMemory(phandle, (void*)FN_ECX_BASEADDRESS_APPLYUSEITEM, &dwValueTemp, (DWORD)sizeof(dwValueTemp), 0);	
	DWORD dw_EcxAddress = dwValueTemp + FN_ECX_OFFSET_APPLYUSEITEM;

	CloseHandle(phandle);	

	// kiểm tra hành trang số 1;
	int row, col;
	DWORD dwIndex = GetItemIndex(pid, itemtype);
	
	ItemPosOnFirstPackage(pid, dwIndex, row, col);
	DWORD dwTypePackage = TYPE_FIRSTPACKAGE;	// cập nhật loại hành trang

	if (row == -1 && col == -1)
	{
		// hành trang số 2
		ItemPosOnSeconcePackage(pid, dwIndex, row, col);
		dwTypePackage = TYPE_SECONDPACKAGE;		// cập nhật loại hành trang
	}
	
	DWORD dwFunctionAddress = FN_FUNCTION_BASEEADDRESS_APPLYUSEITEM;
	__asm
	{
		PUSHAD
		PUSHFD
		PUSH 0						// para 5 
		PUSH row					// para 4: chỉ số dòng
		PUSH col					// para 3: chỉ số cột
		PUSH dwTypePackage			// para 2: loại hành trang
		PUSH dwIndex				// para 1: index của item (index item trong khi load lên trùng với id của màng 1 chiều)
		MOV ECX, dw_EcxAddress		
		CALL dwFunctionAddress	
		POPFD
		POPAD
	}
}

/************************************************************************
VịLH - ItemPosOnOneDimension_FirstPackage - 11/4/2013
	Trả về vị trí item trong hành trang thứ 1
param:
	+ DWORD: process id
	+ DWORD: Index của item trong hành trang
return: 
	+ int: Vị trí item trong hành trang số 1 (trong mảng 1 chiều)
*************************************************************************/
int ItemPosOnOneDimension_FirstPackage(DWORD pid, DWORD dwIndex)
{
	HANDLE phandle = OpenProcess(PROCESS_VM_READ,   0, pid); // Get permission to read
	if (!phandle)	// if handle == NULL --> thoát
		return -1;		

	DWORD dwPackage;		
	ReadProcessMemory(phandle, (void*)PACKAGE_BASEADDRESS, &dwPackage, (DWORD)sizeof(dwPackage), 0);

	// LEA ECX,[EAX*8+ESI+3D98] => LEA ECX, [EAX*8 + DWORD PTR SS:[912B94] + 0x36F00 + 0x3D98 ]
	DWORD dwPackageAddress = EAX_PACKAGE_ONE*8 + dwPackage + FIRST_PACKAGE_OFFSET + SECOND_PACKAGE_OFFSET;
	DWORD dwPackageBaseValue;
	ReadProcessMemory(phandle, (void*)dwPackageAddress, &dwPackageBaseValue, (DWORD)sizeof(dwPackageBaseValue), 0);

	// Tìm trên hành trang
	int i=0;			// 35 ô trong hành trang
	DWORD dwIndexItem;
	while (i < 35)
	{
		ReadProcessMemory(phandle, (void*)dwPackageBaseValue, &dwIndexItem, (DWORD)sizeof(dwIndexItem), 0);

		if (dwIndexItem == dwIndex)
			break;
		i++;

		dwPackageBaseValue = dwPackageBaseValue + 4;

	}

	CloseHandle(phandle);
	return i;
}

/************************************************************************
VịLH - ItemPosOnOneDimension_SeconcePackage - 11/04/2013
	trả về vị trí item trong hành trang số 2
param:
	+ DWORD: process id
	+ DWORD: Index của item trong hành trang
return: 
	+ int: Vị trí item trong hành trang số 2 (trong mảng 1 chiều)
*************************************************************************/
int ItemPosOnOneDimension_SeconcePackage(DWORD pid, DWORD dwIndex)
{
	HANDLE phandle = OpenProcess(PROCESS_VM_READ,   0, pid); // Get permission to read
	if (!phandle)	// if handle == NULL --> thoát
		return -1;		

	DWORD dwPackage;		
	ReadProcessMemory(phandle, (void*)PACKAGE_BASEADDRESS, &dwPackage, (DWORD)sizeof(dwPackage), 0);

	// LEA ECX,[EAX*8+ESI+3D98] => LEA ECX, [EAX*8 + DWORD PTR SS:[912B94] + 0x36F00 + 0x3D98 ]
	DWORD dwPackageAddress = EAX_PACKAGE_TWO*8 + dwPackage + FIRST_PACKAGE_OFFSET + SECOND_PACKAGE_OFFSET;
	DWORD dwPackageBaseValue;
	ReadProcessMemory(phandle, (void*)dwPackageAddress, &dwPackageBaseValue, (DWORD)sizeof(dwPackageBaseValue), 0);
	
	int i=0;		// biến di chuyển trong hành trang
	DWORD dwIndexItem;
	while (i < 35)	// mỗi hành trang có 35 ô
	{
		ReadProcessMemory(phandle, (void*)dwPackageBaseValue, &dwIndexItem, (DWORD)sizeof(dwPackageBaseValue), 0);

		if (dwIndexItem == dwIndex)
			break;
		i++;

		dwPackageBaseValue = dwPackageBaseValue + 4;	// một giá trị có 4 bytes

	}
	CloseHandle(phandle);
	return i;
}
/************************************************************************
VịLH - ItemPosOnFirstPackage - 12/04/2013
	Hàm trả về vị trí dòng, cột của hành trang
param
	+ DWORD: process id của cửa sổ game
	+ DWORD: Index item trong hành trang
	+ int&:	chỉ số dòng tương ứng (bắt đầu từ 0)
	+ int&:	chỉ số cột tương ứng (bắt đầu từ 0)
*************************************************************************/
void ItemPosOnFirstPackage(DWORD pid, DWORD dwIndex, int& row, int& col)
{	
	int val = ItemPosOnOneDimension_FirstPackage(pid, dwIndex);

	if (val == 35)	// tìm kiếm hết hành trang nhưng không tìm thấy
	{
		row = -1;
		col = -1;
		return;
	}

	row = val / 5;
	col = val % 5;	
}

/************************************************************************
VịLH - ItemPosOnSeconcePackage (4 params) - 12/04/2013
	Hàm trả về vị trí dòng, cột của hành trang
param
	+ DWORD: process id của cửa sổ game
	+ DWORD: index item trong hành trang
	+ int&:	chỉ số dòng tương ứng (bắt đầu từ 0)
	+ int&:	chỉ số cột tương ứng (bắt đầu từ 0)
*************************************************************************/
void ItemPosOnSeconcePackage(DWORD pid, DWORD dwIndex, int& row, int& col)
{
	int val = ItemPosOnOneDimension_SeconcePackage(pid, dwIndex);

	if (val == 35)	// tìm kiếm hết hành trang nhưng không tìm thấy
	{
		row = -1;
		col = -1;
		return;
	}

	row = val / 5;
	col = val % 5;	
}

/************************************************************************
VịLH - GetItemIndex - 11/4/2013
	Trả về index của item trong hành trang
param:
	+ DWORD: process id 
	+ int: loại item cần sử dụng mà gamer chọn trên giao diện comboBox
return:	
	+ DWORD: index item trong hành trang
*************************************************************************/
DWORD GetItemIndex(DWORD pid, int itemtype)
{	
	HANDLE phandle = OpenProcess(PROCESS_VM_READ,   0, pid); // Get permission to read
	if (!phandle)	// if handle == NULL --> thoát
		return -1;		

	DWORD dwValNumItemTmp;		
	ReadProcessMemory(phandle, (void*)ITEM_NUMBER_BASEADDRESS, &dwValNumItemTmp, (DWORD)sizeof(dwValNumItemTmp), 0);
	
	//	// ---- HP ----
	//	TIEU_HONG_DON 			= 0,
	//	TRUNG_HONG_DON 			= 1,
	//	DAI_HONG_DON 			= 2,
	//	THANH_LO_TIEU 			= 3,
	//	NHU_Y_THANH_LO_TIEU 	= 4,
	//	THANH_LO 				= 5,
	//	THANH_LO_NHU_Y 			= 6,
	//	BAO_TA_THANH_LO 		= 7,
	//	TA_THANH_LO_NHU_Y 		= 8,
	//	SOCOLA_HP				= 9,
	//	BAT_TU_DUOC 			= 10,
	//
	//	// ---- MP ----
	//	TIEU_HOAN_DON 			= 11,
	//	TRUNG_HOAN_DON 			= 12,
	//	DAI_HOAN_DON 			= 13,
	//	CHAN_KHI_TIEU 			= 14,
	//	NHU_Y_CHAN_KHI_TIEU 	= 15,
	//	CHAN_KHI 				= 16,
	//	CHAN_KHI_NHU_Y 			= 17,
	//	SON_THUY_CHAN_KHI 		= 18,
	//	THUY_CHAN_KHI_NHU_Y 	= 19,
	//	SOCOLA_MP 				= 20,
	//	VONG_DUYEN_DON 			= 21,	

	const int MAXLEN = 21;	
	TCHAR sItemNameArr[][MAXLEN] =  
	{
		// ---- HP ----		
		HP_TIEU_HONG_DON,
		HP_TRUNG_HONG_DON,
		HP_DAI_HONG_DON,
		HP_THANH_LO_TIEU,
		HP_NHU_Y_THANH_LO_TIEU,
		HP_THANH_LO,
		HP_THANH_LO_NHU_Y,
		HP_BAO_TA_THANH_LO,
		HP_TA_THANH_LO_NHU_Y,
		HP_SOCOLA,
		HP_BAT_TU_DUOC,

		// ---- MP ----
		MP_TIEU_HOAN_DON,
		MP_TRUNG_HOAN_DON,
		MP_DAI_HOAN_DON,
		MP_CHAN_KHI_TIEU,
		MP_NHU_Y_CHAN_KHI_TIEU,
		MP_CHAN_KHI, 
		MP_CHAN_KHI_NHU_Y,
		MP_SON_THUY_CHAN_KHI,
		MP_THUY_CHAN_KHI_NHU_Y,
		MP_SOCOLA,
		MP_VONG_DUYEN_DON		
	};
			
	TCHAR sItemName[MAXLEN];
	for (int i=0; i<MAXLEN; i++)
		sItemName[i] = sItemNameArr[itemtype][i];

	int len = sizeof(sItemName) / sizeof(TCHAR);
	for (int id=1; id <= 70; id++)	// 70 chỗ trong hành trang
	{	
		// MOV BYTE PTR DS:[EDX+ESI+64C]
		DWORD dwValNum = dwValNumItemTmp + ITEM_NUMBER_OFFSET + (DISTANCE_BETWEEN_ITEMNUMBER * id);

		DWORD dwPosItemNameAddress =  dwValNum - ITEM_NAME_BASEADDRESS;	
		BYTE bValName;

		int iSame = 0;				// biến iSame để kiểm tra hai item có trùng nhau không?
		int k=0;					// biến di chuyển trong mảng phần tử các bình máu
		for (int j=1; j<=MAXLEN+1; j++)
		{
			ReadProcessMemory(phandle, (void*)dwPosItemNameAddress, &bValName, (BYTE)sizeof(bValName), 0);
			if (bValName == sItemName[k])	// so sánh với tên item
				iSame++;
			k++;
			dwPosItemNameAddress = (DWORD)dwPosItemNameAddress + 0x1;

			if (k>len)
			{
				if (iSame == len)	
					return id;		// id của item;
				else
					break;			// tìm vi trí hành trang số hai
			}
		}
	}

	return -1;		
}

/************************************************************************/
LRESULT _stdcall CALLBACK HookProc (int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode<0)
		goto END;
	
	HWND hVLWnd = pCW->hwnd;
	if((pCW->message == WM_HOOKEX) && pCW->lParam)
	{
		UnhookWindowsHookEx(hHook);
		if (bHooked)
			goto END;
		TCHAR lib_name[MAX_PATH];
		GetModuleFileName(hModuleDll, lib_name, MAX_PATH);
		if(!LoadLibrary(lib_name))
			goto END;
		OldWndProc = (WNDPROC)SetWindowLong(hVLWnd, GWL_WNDPROC, (LONG)NewWndProc);
		if(OldWndProc==NULL) {
			FreeLibrary(hModuleDll);
		}
		else
		{
			bHooked = TRUE;
		}
	}
	else if(pCW->message == WM_HOOKEX) 
	{
		UnhookWindowsHookEx(hHook);
		if (!bHooked)
			goto END;
		if(!SetWindowLong(hVLWnd, GWL_WNDPROC, (LONG)OldWndProc))
			goto END;
		FreeLibrary(hModuleDll);
		bHooked = FALSE;
	}
	END:
		return CallNextHookEx((HHOOK)pCW->wParam,nCode, wParam,lParam);
}


/************************************************************************
VịLH - NewWndProc - 23/05/2013
	Hàm nhận các sự kiện bên chương trình chính gửi qua
params:
	+ HWND: HWnd cửa sổ đang chọn
	+ UINT: Message của chương trình
	+ WPARAM: Tham số wparam
	+ LPARAM: Tham số lparam
return:	
	
*************************************************************************/

LRESULT CALLBACK NewWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg) 
	{
		case WM_COPYDATA:	// get data from shared memory area
			COPYDATASTRUCT * pcds = (COPYDATASTRUCT *)lParam;	
			if (pcds->dwData == DATA_LOCKSOMEONEUSESKILL)
			{											
				iIdSkill = ((Locksomeoneuseskill *)(pcds->lpData))->iIndex;
				iPosition = ((Locksomeoneuseskill *)(pcds->lpData))->iPos;
			}
			break;	
	}


	if (!hWnd)
		return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
	DWORD pid;
	GetWindowThreadProcessId(hWnd, &pid);		

	// Ứng dụng Auto gửi rất nhiều message nhưng chỉ xử lý message WM_HOOK_WRITE thôi
	if (uMsg == WM_HOOK_WRITE)
	{		
		switch (wParam)
		{					
			case WPARAM_APPLYUSEITEM:	
				ApplyUseItem(pid, (int)lParam);
				break;

			case WPARAM_LOCKSOMEONEUSESKILL:
				if (iPosition > 0 && iIdSkill > 0)
					LockSomeOneUseSkill(pid, iPosition, iIdSkill);

				break;			
		}
	}
	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
}

/************************************************************************/
int InjectDll(HWND hWnd)
{	
	if (!IsWindow(hWnd))
		return 0;

	hHook = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC) HookProc, hModuleDll,  GetWindowThreadProcessId(hWnd,NULL));
	if(hHook == NULL)
	{
		 //Error
		MessageBox(NULL,L"Can't set Hook KeyboardProc", L"Error", MB_OK);
		return 0;
	}
	else{
		SendMessage(hWnd, WM_HOOKEX, WPARAM(hHook), 1);
	}
	return 1;
}

/************************************************************************/
int UnmapDll(HWND hWnd)
{	
	if (!IsWindow(hWnd))
		return 0;
	HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)HookProc, hModuleDll, GetWindowThreadProcessId(hWnd,NULL));
	if(hHook==NULL)
		return 0;
	SendMessage(hWnd, WM_HOOKEX, (WPARAM)hHook, 0);
	return 1;
}

