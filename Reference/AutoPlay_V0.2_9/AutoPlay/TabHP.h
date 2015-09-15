#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "INI.h"


// CTabHP dialog

class CTabHP : public CDialog
{
	DECLARE_DYNAMIC(CTabHP)

public:
	CTabHP(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabHP();

// Dialog Data
	enum { IDD = IDD_TAB_HP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sldLimitHP;
	CSliderCtrl m_sldLimitMP;
	CEdit m_txtPercentHP;
	CEdit m_txtPercentMP;
	CComboBox m_cmbHP1;
	CComboBox m_cmbHP2;
	CComboBox m_cmbHP3;
	CComboBox m_cmbMP1;
	CComboBox m_cmbMP2;
	CComboBox m_cmbMP3;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeTxtPercentHp();
	afx_msg void OnEnChangeTxtPercentMp();

	//ThongPH - 2013038 - Load & Save Potion to ComboBox
	CIniReader m_iniReader;
	BOOL LoadPotionToList();
	void SaveCharacterInfo(CString strName);
	BOOL LoadCharacterPotion(CString strName);
};
