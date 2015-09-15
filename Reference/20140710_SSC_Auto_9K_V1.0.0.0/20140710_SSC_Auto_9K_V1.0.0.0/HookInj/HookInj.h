#pragma once


#if !defined INJECT_EX__H
#define INJECT_EX__H


#ifdef INJECT_EX_EXPORTS
#define HOOKDLL_API extern "C" __declspec(dllexport)
#else
#define HOOKDLL_API extern "C" __declspec(dllimport)
#endif


#define WM_FEEDBACK	WM_USER + 100
#define SHIFTED 0x8000
#define pCW ((CWPSTRUCT*)lParam)

#define UPB_BASE_ADD	0x0A352888;

HOOKDLL_API int InjectDll(HWND hWnd);
HOOKDLL_API int UnmapDll(HWND hWnd);

HINSTANCE			hDll;
WNDPROC				OldWndProc = NULL;
LRESULT CALLBACK	NewWndProc(HWND,UINT,WPARAM,LPARAM);


HINSTANCE hModuleDll;
HHOOK hHook;

BOOL bHooked = 0;
HWND hWndAuto;


#endif

