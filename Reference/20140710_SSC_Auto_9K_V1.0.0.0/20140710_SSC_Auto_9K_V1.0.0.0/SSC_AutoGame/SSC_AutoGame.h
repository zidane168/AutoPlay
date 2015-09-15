// SSC_AutoGame.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSSC_AutoGameApp:
// See SSC_AutoGame.cpp for the implementation of this class
//

class CSSC_AutoGameApp : public CWinApp
{
public:
	CSSC_AutoGameApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSSC_AutoGameApp theApp;