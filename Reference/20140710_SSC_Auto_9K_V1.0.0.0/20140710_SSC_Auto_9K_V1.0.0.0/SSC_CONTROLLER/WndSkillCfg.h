#pragma once

#include "../SSC_AutoGame/Resource.h"
#include <map>
#include "../SSC_GameEngine/UserData.h"
class CMainDlg;
// CWndSkillCfg dialog

class CWndSkillCfg : public CDialog
{
	DECLARE_DYNAMIC(CWndSkillCfg)

public:
	CWndSkillCfg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWndSkillCfg();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	void LoadSkillList(bool isBuff);
	void OnBtnAddClick();
	void OnBtnRemoveClick();
	void OnBtnUpClick();
	void OnBtnDownClick();
	void OnCbnSelchange();
	void OnBtnSaveClick();

// Dialog Data
	enum { IDD = IDD_SKILLCFGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CMainDlg* m_wndParent;
	std::map<int,SKILL_CFG*> m_mSkill;
	std::map<int,CString> mSkill;
	bool	bIsBuff;
	bool	bIsFighting;
	bool	bIsAutoQuest;
};
