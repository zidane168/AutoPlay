// HookInj.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "HookInj.h"
#include "Process.h"
#include "../SSC_GameEngine/GameInfo.h"

const UINT WM_HOOK_WRITE = RegisterWindowMessage(L"WM_HOOK_WRITE");
const UINT WM_HOOKEX = RegisterWindowMessage(L"WM_HOOKEX_RK");

CCharacter cChar;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD uReason, LPVOID lpReserved)
{
	if(uReason == DLL_PROCESS_ATTACH)
	{
		hDll = (HINSTANCE) hModule;
		DisableThreadLibraryCalls(hDll);
    }
    return TRUE;
}

LRESULT _stdcall CALLBACK HookProc (int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode<0)
		goto END;
	
	HWND hWndGame = pCW->hwnd;
	
	if((pCW->message == WM_HOOKEX) && pCW->lParam)
	{
		UnhookWindowsHookEx(hHook);
		if (bHooked)
			goto END;
		TCHAR lib_name[MAX_PATH];
		GetModuleFileName(hModuleDll, lib_name, MAX_PATH);
		if(!LoadLibrary(lib_name))
			goto END;
		OldWndProc = (WNDPROC)SetWindowLong(hWndGame, GWL_WNDPROC, (LONG)NewWndProc);
		if(OldWndProc==NULL) {
			FreeLibrary(hModuleDll);
		}
		else
		{
			bHooked = TRUE;
			cChar.Init();
		}
	}
	else if(pCW->message == WM_HOOKEX) 
	{
		UnhookWindowsHookEx(hHook);
		if (!bHooked)
			goto END;
		if(!SetWindowLong(hWndGame, GWL_WNDPROC, (LONG)OldWndProc))
			goto END;
		FreeLibrary(hModuleDll);
		bHooked = FALSE;
	}
	END:
		return CallNextHookEx((HHOOK)pCW->wParam,nCode, wParam,lParam);
}

LRESULT CALLBACK NewWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg==WM_KEYDOWN ){
		if((int)wParam==32 && !cChar.m_bAutoFight){
			if(hWndAuto){	
				PostMessageW(hWndAuto,MSG_CALLBACK,(int) hWnd, 1);
			}
		}	
	}
	if(uMsg==WM_KEYUP){
		if((int)wParam==32 && !cChar.m_bAutoFight){
			
			if(hWndAuto){				
				PostMessageW(hWndAuto,MSG_CALLBACK,(int) hWnd, 0);
			}
		}		
	}


	if (uMsg == WM_HOOK_WRITE)
	{
		CString str;
		switch(wParam){
			case MSG_MOVE:
				return cChar.Move(hWnd,lParam);
				break;
			case MSG_MOVE_AWAY:
				return cChar.MoveAway(hWnd,(int)lParam);
				break;
			case MSG_FIGHT:
				return cChar.SetTarget((int)lParam);				
				break;
			case MSG_SKILL:
				return cChar.SetSkill((int)lParam);
				break;
			case MSG_RIGHT_SKILL:
				return cChar.DoRightSkill(hWnd,(int)lParam);
				break;
			case MSG_RIDE:
				return cChar.RideHorse(hWnd);
				break;
			case MSG_RECORVER:
				return cChar.Recover(hWnd);
				break;
			case MSG_KEYDOWN:
				return cChar.KeyPress(hWnd,(int)lParam);
				break;
			case MSG_BACK_CITY:
				return cChar.BackToCity(hWnd);
				break;
			case MSG_QUIT:
				return cChar.Quit();
				break;
			case MSG_AUTOFIGHT:
				return cChar.m_bAutoFight = (bool)lParam;
				break;
			case MSG_INJECT:
				hWndAuto = (HWND)lParam;
				return cChar.GetWorldMapAddress();
				break;
			case MSG_MENU_CLICK:
				return cChar.SelectedMenu(hWnd,(int)lParam);
				break;
			case MSG_QUEST_ITEM_CLICK:
				return cChar.ClickOnItemLink(hWnd,(int)lParam);
				break;
			case MSG_QUEST_LINK_CLICK:
				return cChar.ClickOnLink(hWnd,(int)lParam);
				break;
			case MSG_AWARD_CLICK:
				return cChar.ChooseAward(hWnd,(int)lParam);
				break;
			case MSG_OPEN_MENU:
				return cChar.OpenMenu(hWnd);
				break;
			case MSG_ITEM_CLICK:
				return cChar.ClickOnItem(hWnd,(int)lParam);
				break;
			////NghiaLP//////////
			/*case MSG_CHECK_AUTO_PARTY:
				return cChar.CheckInterParty(hWnd,(bool)lParam);
				break;*/
			case MSG_AGREE_PARTY:
				return cChar.AgreeParty(hWnd,(bool)lParam);
				break;
			case MSG_INVITE_LIST:
				cChar.CreatePartyInList(hWnd,(DWORD)lParam);
				break;
			case  MSG_CHECK_AUTO_PARTY:
				cChar.CheckAutoParty(hWnd,(byte)lParam);
				break;
			//=============Van Tieu===========
			case MSG_CHOOSE_ADWARD:
				return cChar.RecivedAward(hWnd);
				break;
			//=========Dieu Doi============
			case MSG_OPEN_QUEST:
				return cChar.OpenDialogQuest();
				break;
			case MSG_CHOOSE_QUEST:
				return cChar.ChooseQuest((int)lParam);
				break;
			case MSG_TEXT_QUEST:
				return cChar.GetBaseText((int)lParam);
				break;
			case MSG_CHECK_FINISH:
				return cChar.CheckFinishQuest((int)lParam);
				break;
			
		}
		
		return 1;
	}

		
	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
}

int InjectDll(HWND hWnd)
{	
	if (!IsWindow(hWnd))
		return 0;
	HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)HookProc, hDll, GetWindowThreadProcessId(hWnd,NULL));
	if(hHook==NULL)
		return 0;
	SendMessage(hWnd, WM_HOOKEX, WPARAM(hHook), 1);
	return 1;
}

int UnmapDll(HWND hWnd)
{	
	if (!IsWindow(hWnd))
		return 0;
	HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)HookProc, hDll, GetWindowThreadProcessId(hWnd,NULL));
	if(hHook==NULL)
		return 0;
	SendMessage(hWnd, WM_HOOKEX, (WPARAM)hHook, 0);
	return 1;
}
