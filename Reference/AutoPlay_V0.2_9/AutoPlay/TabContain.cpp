// TabContain.cpp : implementation file
//

#include "stdafx.h"
#include "AutoPlay.h"
#include "TabContain.h"
#include "TabHP.h"
#include "TabPK.h"
// CTabContain

IMPLEMENT_DYNAMIC(CTabContain, CTabCtrl)

CTabContain::CTabContain()
{
	m_tabPages[0]=new CTabHP;
	m_tabPages[1]=new CTabPK;
}

CTabContain::~CTabContain()
{
	for(int nCount=0; nCount < PAGES; nCount++){
		delete m_tabPages[nCount];
	}
}


BEGIN_MESSAGE_MAP(CTabContain, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CTabContain message handlers

void CTabContain::Init()
{
	m_tabCurrent=0;

	m_tabPages[0]->Create(IDD_TAB_HP, this);
	m_tabPages[1]->Create(IDD_TAB_PK, this);

	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);

	SetRectangle();	
}

void CTabContain::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < PAGES; nCount++){
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
	
}

void CTabContain::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurSel()){
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurSel();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
		
	}
}

