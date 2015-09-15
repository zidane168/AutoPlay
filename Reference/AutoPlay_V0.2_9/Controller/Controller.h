#if !defined INJECT_EX__H
	#define INJECT_EX__Info
	#ifdef INJECT_EX_EXPORTS
		#define HOOKDLL_API extern "C" __declspec(dllexport)
	#else
		#define HOOKDLL_API extern "C" __declspec(dllimport)
	#endif

	const UINT WM_HOOK_WRITE = RegisterWindowMessage(L"WM_HOOK_WRITE");
	const UINT WM_HOOKEX = RegisterWindowMessage(L"WM_HOOKEX_RK");
	
	#define pCW ((CWPSTRUCT*)lParam)
	
	HINSTANCE hModuleDll;
	HHOOK hHook;
	BOOL bHooked = 0;
	int iIdSkill = 0;
	int iPosition = 0;

	WNDPROC				OldWndProc = NULL;
	LRESULT CALLBACK	NewWndProc(HWND,UINT,WPARAM,LPARAM);
	HOOKDLL_API int InjectDll(HWND hWnd);
	HOOKDLL_API int UnmapDll(HWND hWnd);
	
	// VịLH - Define prototype name --------
	DWORD GetItemIndex(DWORD, int);
	int ItemPosOnOneDimension_FirstPackage(DWORD, DWORD dwIndex);
	int ItemPosOnOneDimension_SeconcePackage(DWORD, DWORD dwIndex);
	void ApplyUseItem(DWORD, int);
	void ItemPosOnFirstPackage(DWORD, DWORD dwIndex, int& row, int& col);
	void ItemPosOnSeconcePackage(DWORD, DWORD dwIndex, int& row, int& col);
	void LockSomeOneUseSkill(DWORD pid, int iPos, int iIdSkill);
	void SpilitPosAndIdSkill(wchar_t* s);

#endif