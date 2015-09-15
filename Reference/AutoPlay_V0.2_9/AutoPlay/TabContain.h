#pragma once


// CTabContain

class CTabContain : public CTabCtrl
{
	#define PAGES 2
	DECLARE_DYNAMIC(CTabContain)
// Construction
public:
	CTabContain();
	CDialog *m_tabPages[PAGES];
	int m_tabCurrent;
	virtual ~CTabContain();
// Operations
public:
	void Init();
	void SetRectangle();
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};


