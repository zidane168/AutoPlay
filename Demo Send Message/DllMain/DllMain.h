#if _MSC_VER > 1000
#pragma once
#endif

#if !defined INJECT_EX__H
#define INJECT_EX__H


#ifdef INJECT_EX_EXPORTS
#define HOOKDLL_API extern "C" __declspec(dllexport)
#else
#define HOOKDLL_API extern "C" __declspec(dllimport)
#endif

//#define CHARACTER_NAME_ADDRESS	0x007420D0
//
//#define BASE_ADDRESS			0x008FA464
//#define CURRENT_HP_OFFSET		0x0000C6E4
//#define MAX_HP_OFFSET			0x0000C6E8
//#define CURRENT_MP_OFFSET		0x0000C6F0
//#define MAX_MP_OFFSET			0x0000C6F4

HOOKDLL_API int InjectDll(HWND hWnd);
HOOKDLL_API int UnmapDll(HWND hWnd);

HOOKDLL_API int ReadCharacterInfo(HANDLE handle, DWORD dwAddress, DWORD dwOffset);



#endif
