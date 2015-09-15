// AutoPlayDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "tabcontain.h"
#include "afxwin.h"

#include "LibraryShared.h"

// CAutoPlayDlg dialog
class CAutoPlayDlg : public CDialog
{
// Construction
public:
	CAutoPlayDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AUTOHP_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	HWND m_hCurrentWnd;	// HWND cửa sổ	
	HANDLE m_hCurrentHandle;	// handle của process (Open Process)

	int m_iHPPercentConfig;
	int m_iMPPercentConfig;
	int m_iTypeHP1;
	int m_iTypeHP2;
	int m_iTypeHP3;
	int m_iTypeMP1;
	int m_iTypeMP2;
	int m_iTypeMP3;

	int m_iTimerDelay;
	int m_iRadius;
	int m_iSkillID;

	bool m_bPK;
	bool m_bMonster;
	bool m_bIsRunning;	

public:
	CListCtrl m_lstRole;
	void AddData(CListCtrl &ctrl, int row, int col,CString str);
	CTabContain m_tabConfig;

	HANDLE GetHandleOfGame(void);
	HANDLE GetHandleByRoleName(CString roleName);
	HWND GetHWNDByRoleName(CString roleName);	

	afx_msg void OnLvnItemchangedLstrole(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnBnClickedButtonExec();
	afx_msg void OnClose();	

	CString strName;	
	CString m_sCurName;

	CCharacter m_cRole;
	CCharacter m_cNearestEnemy;
	CCharacter m_cCurrentMonster;

	CArray<CCharacter, CCharacter&> m_arrCEnemies;

	int GetCurrentPercent(int current, int max);
	int CheckAddUserToListCtrl(CString curName);
	int GetProcessTotal(void);
	int IsOffline(CCharacter);

	DWORD FindProcessId(CString processName);
	
	void AddUserToList(CCharacter role);
	void CheckToRestore();
	void GetAllRole();
	void RestoreMP(HWND hwnd,int iType);
	void RestoreHP(HWND hwnd,int iType);	
	void SaveData();
	void CheckToUseSkill(void);
	
};
