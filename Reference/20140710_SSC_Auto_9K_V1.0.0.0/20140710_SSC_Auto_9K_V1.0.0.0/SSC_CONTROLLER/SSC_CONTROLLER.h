// SSC_CONTROLLER.h : main header file for the SSC_CONTROLLER DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSSC_CONTROLLERApp
// See SSC_CONTROLLER.cpp for the implementation of this class
//

class CSSC_CONTROLLERApp : public CWinApp
{
public:
	CSSC_CONTROLLERApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
