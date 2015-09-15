// Filename: WndEvent.h
// 14-Nov-2013 SSC copy right.

#pragma once

#include <map>

// Forward class declarations.
class CWndControl;

// Class sCWndEvent

class CWndEvent 
{
	public:
		// Constructor / destructor.
		__declspec(dllexport) CWndEvent(CWndControl* sender, const CString& text);
		__declspec(dllexport) ~CWndEvent();
	    
		// Public methods.
		__declspec(dllexport) CWndControl* GetSender() const;
		__declspec(dllexport) CString      GetText() const;
		__declspec(dllexport) void         AddProperty(const CString& name, const CString& value);
		__declspec(dllexport) bool         GetProperty(const CString& name, CString& value) const;

	private:
		// Event sender.
		CWndControl* m_sender;

		// Event text.
		CString m_text;

		// Event properties.
		std::map<CString,CString> m_properties;
};


// IWndEventHandler interface.
class IWndEventHandler
{
	public:
		__declspec(dllexport) virtual void HandleWndEvent(const CWndEvent& ev) = 0;
};