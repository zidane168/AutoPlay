// Filename: XmlToControl.h
// 15-Nov-2013 SSC copy right.
#pragma once

#include "WndControl.h"
#include "WndConverter.h"
#include <list>
#include <map>

// Forward class declarations.
class CGroupControl;
class CWndControl;
class CWndRadioButton;
class CWndConverter;
class CMarkup;

// Con trỏ hàm dùng để chuyển xml thành control.
typedef CWndControl* (*ParseControlFn)(CMarkup& xml, CWndConverter* controlFactory);


//Class CXmlToControl dùng class CMarkup để chuyển đổi file
//xml thành control 
class CXmlToControl
{
public:
	 __declspec(dllimport) CXmlToControl();
    __declspec(dllimport) ~CXmlToControl();
    
    __declspec(dllimport) bool ParseXml(const CString& xmlFile,
                  std::list<CGroupControl*>& topLevelGroupsList,
                  std::list<CWndControl*>& allControlsList);

private:
    bool ReadFile(const CString& xmlFile, CString& xmlText);
    bool RegisterParseControlFunction(const CString& strType, ParseControlFn parseControlFn);

    void ParseGroups(CMarkup& xml, CGroupControl* parentGroup, std::list<CGroupControl*>& groupList, std::list<CWndControl*>& controlList);
    bool ParseGroup(CMarkup& xml, CGroupControl* parentGroup, std::list<CGroupControl*>& groupList, std::list<CWndControl*>& controlList);    

    void        ParseControls(CMarkup& xml, CGroupControl* parentGroup, std::list<CWndControl*>& controlList);
    static void ParseControl_Common(CMarkup& xml, CWndControl* pControl);    
    void        PerformLinking(CMarkup& xml, CWndControl* pControl, std::list<CWndControl*>& controlList);

    static CWndControl* ParseControl_Button(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_CheckBox(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_ComboBox(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_GroupBox(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_IpAddress(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_Label(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_ListBox(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_RadioButton(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_SpinButton(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_TextBox(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_TrackBar(CMarkup& xml, CWndConverter* controlFactory);
    static CWndControl* ParseControl_WebBrowser(CMarkup& xml, CWndConverter* controlFactory);

   
    CWndConverter* m_controlFactory;

    // Chứa con trỏ hàm để chuyển thành control.
    std::map<CString, ParseControlFn> m_parseControlFunctions;
};


