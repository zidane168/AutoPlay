#pragma once
#include "stdafx.h"
#include "../SSC_GameEngine/Character.h"
#include "WndControl.h"
#include <map>

using namespace std;
class CGUIMapping
{
public:
	CGUIMapping(CMainDlg* container){
		dlgContainer = container;
		dlgCtrlContainer = container->m_controlDlg;
	}
	void AddMapping(CString sCtrlName,  ControlProp cProp,DWORD dwValue){
		GUI_MAPPING* guiData = new GUI_MAPPING();
		guiData->_ctrlProp = cProp;
		guiData->_prop = dwValue;
		m_mLstMap.insert(make_pair(sCtrlName,guiData));
	}
	void ClearMapping(){
		m_mLstMap.clear();
	}
	void GUIToObj(){
		std::list<CWndControl*> lstCtrl;
		dlgContainer->m_tabWnd->GetSelectedGroup()->GetControls(lstCtrl);
		if(lstCtrl.size()==0)
			return;
		std::list<CWndControl*>::const_iterator lstItem;
		for(lstItem = lstCtrl.begin() ;lstItem!=lstCtrl.end();lstItem++){
			std::map<CString,GUI_MAPPING*>::iterator item;
			CWndControl* pControl = *lstItem;
			GUIToObj(pControl->GetName());
		}
	}
	void GUIToObj(CString ctrlName){
		std::map<CString,GUI_MAPPING*>::iterator item;
		item = m_mLstMap.find(ctrlName);
		if(item==m_mLstMap.end())
			return;
		GUI_MAPPING* data = item->second;
		SetCtrlToObj(ctrlName,data->_ctrlProp,data->_prop);

	}
	void ObjToGUI(){
		std::list<CWndControl*> lstCtrl;
		dlgContainer->m_tabWnd->GetSelectedGroup()->GetControls(lstCtrl);
		if(lstCtrl.size()==0)
			return;
		std::list<CWndControl*>::const_iterator lstItem;
		for(lstItem = lstCtrl.begin() ;lstItem!=lstCtrl.end();lstItem++){
			std::map<CString,GUI_MAPPING*>::iterator item;
			CWndControl* pControl = *lstItem;
			item = m_mLstMap.find(pControl->GetName() );
			if(item==m_mLstMap.end())
				continue;
			GUI_MAPPING* data = item->second;
			SetValToCtrl (pControl,data->_ctrlProp,data->_prop);
		}
	}

	void SetCtrlToObj(CString sCtrlName, ControlProp cProp,DWORD dwValue){
		CWndControl* ctrl = dlgCtrlContainer->GetControl(sCtrlName);
		if(ctrl==NULL)
			return;
		CString type = ctrl->GetTypeName();
		switch (cProp){
			case ControlProp::Enable:
				(*(bool*)dwValue) = ctrl->IsEnabled();
				break;
			case ControlProp::Check:
				if(type== L"CheckBox")
					(*(bool*)dwValue) = static_cast<CWndCheckBox*> (ctrl)->IsChecked();
				else if(type== L"RadioButton")
					(*(bool*)dwValue) = static_cast<CWndRadioButton*> (ctrl)->IsChecked();
				break;
			case ControlProp::Selected:
				if(type==  L"ComboBox")
					(*(int*)dwValue)=static_cast<CWndComboBox*> (ctrl)->GetSelectedIndex();
				break;
			case ControlProp::Text:
				if(type==  L"TextBox")
					(*(CString*)dwValue) = (static_cast<CWndTextBox*> (ctrl)->GetText());
				break;
			case ControlProp::Value:
				if(type==  L"TextBox")
					(*(int*)dwValue) = _ttoi(static_cast<CWndTextBox*> (ctrl)->GetText());
				break;
		}
	}
	void SetValToCtrl(CWndControl* ctrl,  ControlProp cProp,DWORD dwValue){
		CString type = ctrl->GetTypeName();
		CWndComboBox* cmb ;
		int idx;
		switch (cProp){
			case ControlProp::Enable:
				ctrl->SetEnabled((bool)*(bool*)dwValue);
				break;
			case ControlProp::Check:
				if(type==  L"CheckBox")
					static_cast<CWndCheckBox*> (ctrl)->SetChecked((bool)*(bool*)dwValue);
				else if(type==  L"RadioButton")
					static_cast<CWndRadioButton*> (ctrl)->SetChecked((bool)*(bool*)dwValue);
				break;
			case ControlProp::Selected:
				if(type==  L"ComboBox")
					cmb = static_cast<CWndComboBox*> (ctrl);
					idx = (*(int*)dwValue);
					if(cmb->GetCount()<=idx)
						cmb->SetSelectedIndex(cmb->GetCount()-1);
					else
						cmb->SetSelectedIndex(idx);
					
				break;
			case ControlProp::Text:
				if(type==  L"TextBox"){
					static_cast<CWndTextBox*> (ctrl)->SetText((*(CString*)dwValue));
				}
				break;
			case ControlProp::Value:
				if(type==  L"TextBox"){
					CString val;				
					val.Format(L"%d",(*(int*)dwValue));
					static_cast<CWndTextBox*> (ctrl)->SetText(val);
				}
				break;
		}
	}
private:
	std::map<CString, GUI_MAPPING*> m_mLstMap;
	CWndDlg* dlgCtrlContainer;
	CMainDlg* dlgContainer;
};