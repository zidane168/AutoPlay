// Filename: WndControl.h
// 14-Nov-2013 SSC copy right.
#pragma once

#include "WndEvent.h"

#include <list>
#include <vector>

// Forward class declarations.
class CWndEvent;
class IWndEventHandler;

// Exported functions.
void ShowWnd(CWnd* pWnd);
void HideWnd(CWnd* pWnd);
void MoveWnd(CWnd* pWnd);
__declspec(dllimport) void TokenizeString(const CString& strInput, const CString& strDelim, std::list<CString>& tokenList);

// CWndControl base class (abstract).
class CWndControl : public IWndEventHandler
{
	public:
    // Constructor / destructor.
    __declspec(dllimport) CWndControl();
    __declspec(dllimport) virtual ~CWndControl();
    
    // Type string.
    __declspec(dllimport) const CString& GetTypeName() const { return m_typeName; }
    
    // General purpose name identifier.
    __declspec(dllimport) const CString& GetName() const { return m_name; }
    __declspec(dllimport) void  SetName(const CString& name);
    
    // Visibility.
    __declspec(dllimport) bool IsVisible() const { return m_visible; }
    __declspec(dllimport) void SetVisible(bool visible);
    
    // Enabled state.
   __declspec(dllimport)  bool IsEnabled() const { return m_enabled; }
    __declspec(dllimport) void SetEnabled(bool enabled);
    
    // Read-only state.
    __declspec(dllimport) bool IsReadOnly() const { return m_readOnly; }
    __declspec(dllimport) void SetReadOnly(bool readOnly);
    
    // Location.
    __declspec(dllimport) const CPoint& GetLocation() const { return m_location; }
    __declspec(dllimport) void  SetLocation(const CPoint& location);
    
    // Size. 
    __declspec(dllimport) const CSize& GetSize() const { return m_size; }
    __declspec(dllimport) void  SetSize(const CSize& size);
    __declspec(dllimport) CRect GetRect() const;
    
    // CWnd resource ID.
    __declspec(dllimport) UINT GetResourceId() const { return m_resourceId; }
    
    // CWnd attachment.
    __declspec(dllimport) void  AttachWnd(CWnd* pWnd);
    __declspec(dllimport) void  DetachWnd();
    __declspec(dllimport) CWnd* GetAttachWnd() { return m_attachWnd; }
    
    // CFont attachment.
    __declspec(dllimport) void AttachFont(CFont* pFont);
    
    // Events.
    __declspec(dllimport) void EnableEvents(bool enable);
    __declspec(dllimport) void SuspendEvents();
    __declspec(dllimport) void RestoreEvents();
    __declspec(dllimport) void AddEventHandler(IWndEventHandler* pEventHandler);
    __declspec(dllimport) void RemoveEventHandler(IWndEventHandler* pEventHandler);
    __declspec(dllimport) void RemoveAllEventHandlers();

    // Link to other CWndControls.
    __declspec(dllimport) void AddLinkedControl(CWndControl* pControl);
    __declspec(dllimport) void RemoveLinkedControl(CWndControl* pControl);
    __declspec(dllimport) void RemoveAllLinkedControls();

    // Pure virtual methods.
    __declspec(dllimport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId) = 0;
    __declspec(dllimport) virtual void UpdateWnd() = 0;
    __declspec(dllimport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) = 0;
    
    // IWndEventHandler overrides.
    __declspec(dllimport) virtual void HandleWndEvent(const CWndEvent& ev);

protected:
    bool NeedReposition();
    void NotifyEventHandlers(const CWndEvent& event);
    void UpdateEnabledState();
    void UpdateVisibleState();

    CString m_typeName;
    CString m_name;
    
    bool m_visible;
    bool m_enabled;
    bool m_readOnly;
    
    CPoint m_location;
    CSize  m_size;
    
    UINT   m_resourceId;
    CWnd*  m_attachWnd;
    CFont* m_attachFont;
    
    // Events.
    bool m_enableEvents;
    bool m_saveEnableEvents;
    std::list<IWndEventHandler*> m_eventHandlers;

	//Danh sách chứa các controls có sự tham chiếu cho phép xử 
	//lý các sự kiện liên quan đến những controls đó. Ví dụ Radio buttons
    std::list<CWndControl*> m_linkedControls;
};


//////////////////////////////////////////////////////////////////////////////////////

// CWndButton class.
class CWndButton : public CWndControl
{
public:
    __declspec(dllexport) CWndButton();
    __declspec(dllexport) virtual ~CWndButton();
    
    __declspec(dllexport) const CString& GetText() const { return m_text; }
    __declspec(dllexport) void           SetText(const CString& text);
    
    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndCheckBox class.
class CWndCheckBox : public CWndControl
{
public:
    __declspec(dllexport) CWndCheckBox();
    __declspec(dllexport) virtual ~CWndCheckBox();
    
    __declspec(dllexport) const CString& GetText() const { return m_text; }
    __declspec(dllexport) void  SetText(const CString& text);
    
    __declspec(dllexport) void  SetChecked(bool checked);
    __declspec(dllexport) bool  IsChecked() const { return m_checked; }

    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
    bool    m_checked;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndComboBox class.
class CWndComboBox : public CWndControl
{
public:
    __declspec(dllexport) CWndComboBox();
    __declspec(dllexport) virtual ~CWndComboBox();

    __declspec(dllexport) void    AddItem(const CString& item);
    __declspec(dllexport) void    AddItems(const std::list<CString>& itemList);
    __declspec(dllexport) void    ClearItems();
    
    __declspec(dllexport) CString GetItem(int index) const;
    __declspec(dllexport) int     GetCount() const;
    __declspec(dllexport) void    SetItem(int index, const CString& item);

    __declspec(dllexport) int     GetSelectedIndex() const;
    __declspec(dllexport) void    SetSelectedIndex(int index);

    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    std::vector<CString> m_items;

    int m_selectedIndex;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndGroupBox class.
class CWndGroupBox : public CWndControl
{
public:
    __declspec(dllexport) CWndGroupBox();
    __declspec(dllexport) virtual ~CWndGroupBox();
    
    __declspec(dllexport) const CString& GetText() const { return m_text; }
    __declspec(dllexport) void           SetText(const CString& text);
    
    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndIpAddress class.
class CWndIpAddress : public CWndControl
{
public:
    __declspec(dllexport) CWndIpAddress();
    __declspec(dllexport) virtual ~CWndIpAddress();

    __declspec(dllexport) void SetIpAddress(BYTE field1, BYTE field2, BYTE field3, BYTE field4); 
    __declspec(dllexport) void GetIpAddress(BYTE& field1, BYTE& field2, BYTE& field3, BYTE& field4) const;

    __declspec(dllexport) void    SetIpAddress(const CString& ipAddress);
    __declspec(dllexport) CString GetIpAddress() const;

    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    BYTE m_field1;
    BYTE m_field2;
    BYTE m_field3;
    BYTE m_field4;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndLabel class.
class CWndLabel : public CWndControl
{
public:
    __declspec(dllexport) CWndLabel();
    __declspec(dllexport) virtual ~CWndLabel();
    
    __declspec(dllexport) const CString& GetText() const { return m_text; }
    __declspec(dllexport) void           SetText(const CString& text);
    
    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndListBox class.
class CWndListBox : public CWndControl
{
public:
    __declspec(dllexport) CWndListBox();
    __declspec(dllexport) virtual ~CWndListBox();

    __declspec(dllexport) void    AddItem(const CString& item);
    __declspec(dllexport) void    AddItems(const std::list<CString>& itemList);
    __declspec(dllexport) void    ClearItems();
    
    __declspec(dllexport) CString GetItem(int index) const;
    __declspec(dllexport) int     GetCount() const;
    __declspec(dllexport) void    SetItem(int index, const CString& item);

    __declspec(dllexport) int     GetSelectedIndex() const;
    __declspec(dllexport) void    SetSelectedIndex(int index);

    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    std::vector<CString> m_items;

    int m_selectedIndex;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndRadioButton class.
class CWndRadioButton : public CWndControl
{
public:
    __declspec(dllexport) CWndRadioButton();
    __declspec(dllexport) virtual ~CWndRadioButton();
    
    __declspec(dllexport) const CString& GetText() const { return m_text; }
   __declspec(dllexport)  void           SetText(const CString& text);
    
    __declspec(dllexport) void  SetChecked(bool checked);
    __declspec(dllexport) bool  IsChecked() const { return m_checked; }

    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
   __declspec(dllexport)  virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
    // IWndEventHandler overrides.
    __declspec(dllexport) virtual void HandleWndEvent(const CWndEvent& ev);

private:
    void    UncheckLinkedControls(const CString& exceptName);

    CString m_text;
    bool    m_checked;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndSpinButton class.
class CWndSpinButton : public CWndControl
{
public:
    __declspec(dllexport) CWndSpinButton();
    __declspec(dllexport) virtual ~CWndSpinButton();

    __declspec(dllexport) void SetMinimum(int minimum); 
    __declspec(dllexport) int  GetMinimum() const;

    __declspec(dllexport) void SetMaximum(int maximum); 
    __declspec(dllexport) int  GetMaximum() const;

    __declspec(dllexport) void SetValue(int value); 
    __declspec(dllexport) int  GetValue() const;

    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    int m_minimum;
    int m_maximum;
    int m_value;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndTextBox class.
class CWndTextBox : public CWndControl
{
public:
    __declspec(dllexport) CWndTextBox();
    __declspec(dllexport) virtual ~CWndTextBox();
    
    __declspec(dllexport) const CString& GetText() const { return m_text; }
    __declspec(dllexport) void           SetText(const CString& text);
    
    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndTrackBar class.
class CWndTrackBar : public CWndControl
{
public:
    __declspec(dllexport) CWndTrackBar();
    __declspec(dllexport) virtual ~CWndTrackBar();

    __declspec(dllexport) void SetMinimum(int minimum); 
    __declspec(dllexport) int  GetMinimum() const;

    __declspec(dllexport) void SetMaximum(int maximum); 
    __declspec(dllexport) int  GetMaximum() const;

    __declspec(dllexport) void SetValue(int value); 
    __declspec(dllexport) int  GetValue() const;

    __declspec(dllexport) void SetTickFrequency(int tickFrequency);
    __declspec(dllexport) int  GetTickFrequency() const;

    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    int m_minimum;
    int m_maximum;
    int m_value;
    int m_tickFrequency;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndWebBrowser class.
class CWndWebBrowser : public CWndControl
{
public:
    __declspec(dllexport) CWndWebBrowser();
    __declspec(dllexport) virtual ~CWndWebBrowser();

    __declspec(dllexport) CString GetUrl() const;
    __declspec(dllexport) void    SetUrl(const CString& url);

    // CWndControl overrides.
    __declspec(dllexport) virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    __declspec(dllexport) virtual void UpdateWnd();
    __declspec(dllexport) virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    CString m_url;
};