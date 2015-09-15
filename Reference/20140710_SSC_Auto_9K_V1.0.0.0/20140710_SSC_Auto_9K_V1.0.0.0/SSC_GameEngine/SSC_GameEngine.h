// SSC_GameEngine.h : main header file for the SSC_GameEngine DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSSC_GameEngineApp
// See SSC_GameEngine.cpp for the implementation of this class
//

class CSSC_GameEngineApp : public CWinApp
{
public:
	CSSC_GameEngineApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
