// Filename: GroupControl.h
// 15-Nov-2013 SSC copy right.

#pragma once

#include <list>

class CWndControl;
class CGroupControl;

// CGroupControl

class CGroupControl 
{
	public:
    // Constructor / destructor.
    __declspec(dllimport) CGroupControl();
    __declspec(dllimport) ~CGroupControl();
    
    // Group attributes.
    __declspec(dllimport) void SetName(const CString& name);
    __declspec(dllimport) const CString& GetName() const;

    // Manage sub-groups.
    __declspec(dllimport) void AddGroup(CGroupControl* pGroup);
    __declspec(dllimport) void RemoveGroup(CGroupControl* pGroup);
    __declspec(dllimport) void RemoveAllGroups();
    __declspec(dllimport) void GetGroups(std::list<CGroupControl*>& groupList) const;
    __declspec(dllimport) int  GetNumGroups() const;

    // Manage controls.
    __declspec(dllimport) void AddControl(CWndControl* pControl);
    __declspec(dllimport) void RemoveControl(CWndControl* pControl);
    __declspec(dllimport) void RemoveAllControls();       
    __declspec(dllimport) void GetControls(std::list<CWndControl*>& controlList) const;
    __declspec(dllimport) int  GetNumControls() const;

private:
    CString m_groupName;

    // List of control groups.
    std::list<CGroupControl*> m_groupList;

    // List of controls.
    std::list<CWndControl*> m_controlList;
};


