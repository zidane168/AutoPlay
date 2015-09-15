// WndSkill.cpp : implementation file
//

#include "stdafx.h"
#include "SSC_AutoGame.h"
#include "WndSkill.h"


// CWndSkill dialog

IMPLEMENT_DYNAMIC(CWndSkill, CDialog)

CWndSkill::CWndSkill(CWnd* pParent /*=NULL*/)
	: CDialog(CWndSkill::IDD, pParent)
{

}

CWndSkill::~CWndSkill()
{
}

void CWndSkill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndSkill, CDialog)
END_MESSAGE_MAP()


// CWndSkill message handlers
