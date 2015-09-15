// Filename: WndConverter.h
// 15-Nov-2013 SSC copy right.
#pragma once

#include <map>

class CWndControl;

//Kiểu con trỏ hàm để tạo thể hiện CWnd mới.
typedef CWnd* (*CreateWndFn)();
//Kiểu con trỏ hàm để tạo thể hiện CWndControl mới.
// Function pointer type for creating a new CWndControl instance. 
typedef CWndControl* (*CreateControlFn)();
//Class CWndControlItem là một lớp đơn giản dùng để chứa 2 con trỏ hàm.
class CWndControlItem
{
	public:
		__declspec(dllexport) CWndControlItem()
		{
			m_createWndFn = NULL;
			m_createControlFn = NULL;
		}
	    
		CreateWndFn     m_createWndFn;
		CreateControlFn m_createControlFn;
};

//Class CWndConverter có thể dùng để tạo thể hiện CWnd hoặc CWndControl bằng cách
//truyền vào một chuổi. Nếu muôn hỗ trợ kiểu CWnd hoặc CWndControl mới thì cần
//gọi hàm RegisterCreateFunction từ bên ngoài hoặc bên trong constructor của lớp.

class CWndConverter
{
public:
    // Constructor / destructor.
    __declspec(dllexport) CWndConverter();
    __declspec(dllexport) ~CWndConverter();
    
    // Register new create functions.
    __declspec(dllexport) bool RegisterCreateFunctions(const CString& strType, CreateWndFn createWndFn, CreateControlFn createControlFn);
    
    // Top-level factory methods.
   __declspec(dllexport)  CWnd*        CreateWnd(const CString& strType);
    __declspec(dllexport) CWndControl* CreateControl(const CString& strType);
    
private:    
    // Store the registered function pointers.
    std::map<CString,CWndControlItem> m_createFunctions;
};


