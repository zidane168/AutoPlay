// Serialize.cpp : implementation file
//

#include "stdafx.h"
#include "Serialize.h"
#include "Shlwapi.h"


XMLSerialized::XMLSerialized(const CString &fileName){
	m_mapping.clear();
	if(fileName.GetLength()==0)
		sFileName = L"NoName.xml";
	else
		sFileName = fileName;
	ConvertFileName(sFileName);
	
}

XMLSerialized::~XMLSerialized(){
	m_mapping.clear();
}

void XMLSerialized::Mapping(const CString &sNode,const CString &sParent,TagType cTagType,DataType cType,DWORD dwValue){
	DATA* data = new DATA;
	data->_dataType = cType;
	data->_value = dwValue;
	data->_parent = sParent;
	data->_tagName = sNode;
	data->_tagType = cTagType;

	m_mapping.insert(make_pair(m_mapping.size(),data));	
}
bool XMLSerialized::FindPath(CString sPath,CMarkup* xml){
	if(sPath.GetLength()==0)
		return false;
	int iPos = sPath.Find(L"/");
	if(iPos==0){
		xml->ResetPos();
		sPath.Delete(0,1);
	}
	iPos = sPath.Find(L"/");
	while(iPos!=-1){
		CString tag = sPath.Left(iPos);
		if(xml->FindElem(tag))
			xml->IntoElem();
		sPath.Delete(0,iPos+1);
		iPos = sPath.Find(L"/");
	}
	if(xml->FindElem(sPath)){
		xml->IntoElem();
		return true;
	}
	return false;
}

bool XMLSerialized::Serialize(){
	CMarkup* xml = new CMarkup();
	if(xml==NULL){
		return false;
	}
	std::map<int,DATA*>::const_iterator item;
	for(item=  m_mapping.begin(); item != m_mapping.end(); item ++){
		DATA* data = item->second;
		if(data->_tagType == TagType::Root){
			xml->AddElem( data->_tagName);
			continue;
		}
		if(data->_tagType == TagType::Element){
			xml->ResetPos();			
			if(FindPath(data->_parent,xml)){
				xml->AddElem( data->_tagName);
				if(data->_dataType!= DataType::None){
					CString result;
					switch(data->_dataType){
						case DataType::String:
							xml->SetData(*(CString*)data->_value);
						break;
						case DataType::Int:
							xml->SetData(*(int*)data->_value);
						break;
						case DataType::Bool:
							xml->SetData((int)*(bool*)data->_value);
						break;
						case DataType::ByteArray:
							ByteArrayToString(result,L"|",(byte*)data->_value);
							xml->SetData(result);
						break;
						case DataType::IntArray:
							IntArrayToString(result,L"|",(int*)data->_value);
							xml->SetData(result);
						break;
					}
				}
				continue;
			}
		}
		if(data->_tagType == TagType::Attribute){
			if(FindPath(data->_parent,xml)){
				xml->OutOfElem();
				CString result;
				
				switch(data->_dataType){
					case DataType::String:
						xml->AddAttrib(data->_tagName,*(CString*)data->_value);
					break;
					case DataType::Int:
						xml->AddAttrib(data->_tagName,*(int*)data->_value);
					break;
					case DataType::Bool:
						xml->AddAttrib(data->_tagName,(int)*(bool*)data->_value);
					break;
					case DataType::ByteArray:
						ByteArrayToString(result,L"|",(byte*)data->_value);
						xml->AddAttrib(data->_tagName,result);
					break;
					case DataType::IntArray:
						IntArrayToString(result,L"|",(int*)data->_value);
						xml->AddAttrib(data->_tagName,result);
					break;
				}
			}
		}		
	}
	xml->Save(sFileName);
	delete xml;
	xml=NULL;
	return true;
}

bool  XMLSerialized::Deserialize(){
	if(m_mapping.size()==0)
		return false;
	CMarkup * xml = new CMarkup();
	if(!xml->Load(sFileName)){
		delete xml;
		xml = NULL;
		return false;
	}
	std::map<int,DATA*>::const_iterator item;
	for(item=  m_mapping.begin(); item != m_mapping.end(); item ++){
		DATA* data = item->second;
	
		if(data->_tagType== TagType::Attribute){
			if(FindPath(data->_parent,xml)){
				xml->OutOfElem();
				switch(data->_dataType){
					case DataType::Int:
						(*(int*)data->_value) = _ttoi(xml->GetAttrib(data->_tagName));
					break;
					case DataType::Bool:
						(*(bool*)data->_value) = (bool)_ttoi(xml->GetAttrib(data->_tagName));
					break;
					case DataType::ByteArray:
						StringToByteArray(xml->GetAttrib(data->_tagName),L"|",(byte*)data->_value);
					break;
					case DataType::IntArray:
						StringToIntArray(xml->GetAttrib(data->_tagName),L"|",(int*)data->_value);
					break;
					case DataType::String:
						(*(CString*)data->_value) = (CString)(xml->GetAttrib(data->_tagName));
					break;
				}
			}
		}
		if(data->_tagType== TagType::Element && data->_dataType!=DataType::None){
			if(FindPath(data->_parent,xml)){
				if(xml->FindElem(data->_tagName)){
					switch(data->_dataType){
						case DataType::Int:
							(*(int*)data->_value) = _ttoi(xml->GetData());
						break;
						case DataType::Bool:
							(*(bool*)data->_value) = (bool)_ttoi(xml->GetData());
						break;
						case DataType::ByteArray:
							StringToByteArray(xml->GetData(),L"|",(byte*)data->_value);
						break;
						case DataType::IntArray:
							StringToIntArray(xml->GetData(),L"|",(int*)data->_value);
						break;
						case DataType::String:
							(*(CString*)data->_value) = (CString)(xml->GetData());
						break;
					}
				}
			}
		}
	}
	delete xml;
	xml = NULL;
	return true;
}

void XMLSerialized::StringToIntArray(const CString& strInput, const CString& strDelim, int* arrInt)
{
    // Check for empty input string.
    if ( strInput.IsEmpty() )
        return;

    // Check for empty delimiter.
    if ( strDelim.IsEmpty() && !strInput.IsEmpty() )
    {
        arrInt[0] = _ttoi(strInput);
        return;
    }

    // Proceed with tokenizing.
    CString currToken = _T("");
    int nInputLen = strInput.GetLength();
	byte nIdx = 1;
    for(int i = 0; i < nInputLen; ++i)
    {
        // Check if there is a delimiter at position i.
        if ( i < (nInputLen - strDelim.GetLength() + 1) &&
             strInput.Mid(i, strDelim.GetLength()).Compare(strDelim) == 0 )
        {
			if ( !currToken.IsEmpty() ){
                arrInt[nIdx] = _ttoi(currToken);
				nIdx++;
			}
            currToken = _T("");

            i += (strDelim.GetLength() - 1);
        } 
        else
        {
            currToken += strInput.GetAt(i);
        }
    }
    if ( !currToken.IsEmpty() )
        arrInt[nIdx] = _ttoi(currToken);

}
void XMLSerialized::StringToByteArray(const CString& strInput, const CString& strDelim, byte* arrInt)
{
    // Check for empty input string.
    if ( strInput.IsEmpty() )
        return;

    // Check for empty delimiter.
    if ( strDelim.IsEmpty() && !strInput.IsEmpty() )
    {
        arrInt[0] = _ttoi(strInput);
        return;
    }

    // Proceed with tokenizing.
    CString currToken = _T("");
    int nInputLen = strInput.GetLength();
	byte nIdx = 1;
    for(int i = 0; i < nInputLen; ++i)
    {
        // Check if there is a delimiter at position i.
        if ( i < (nInputLen - strDelim.GetLength() + 1) &&
             strInput.Mid(i, strDelim.GetLength()).Compare(strDelim) == 0 )
        {
			if ( !currToken.IsEmpty() ){
                arrInt[nIdx] = _ttoi(currToken);
				nIdx++;
			}
            currToken = _T("");

            i += (strDelim.GetLength() - 1);
        } 
        else
        {
            currToken += strInput.GetAt(i);
        }
    }
    if ( !currToken.IsEmpty() )
        arrInt[nIdx] = _ttoi(currToken);

}
void XMLSerialized::IntArrayToString(CString& strOutput, const CString& strDelim, int* arrInt)
{
	
	strOutput=_T("");
	CString strTemp;
    // Proceed with tokenizing.
	for(int i = 1; i <= arrInt[0]; i++)
    {
		strTemp.Format(L"%d",arrInt[i]);
        strOutput += strTemp+ strDelim;
    }
}
void XMLSerialized::ByteArrayToString(CString& strOutput, const CString& strDelim, byte* arrByte)
{
	
	strOutput=_T("");
	CString strTemp;
    // Proceed with tokenizing.
	for(int i = 1; i <= arrByte[0]; i++)
    {
		strTemp.Format(L"%d",arrByte[i]);
        strOutput += strTemp+ strDelim;
    }
}



void   XMLSerialized::ConvertFileName(CString& sFileName){
	char temp[MAX_PATH];
	CString m_strDir;
	//Tạo UI từ file XML
	if(GetModuleFileNameA( (HMODULE) AfxGetInstanceHandle(), temp, sizeof(temp)))
	{
		char* slash = strrchr(temp, '\\'); // remove the current EXE name from the path
		if(slash)
		{
			*(slash + 1) = '\0';
			m_strDir = temp; // we got the application path 
			m_strDir += L"\\"+sFileName; // now add the other exe's path
			sFileName=m_strDir; // return TRUE  unless you set the focus to a control
		}
	}
}
CMarkup XMLSerialized::OpenFile(){
	CMarkup xml;
//nếu file chưa tồn  tại thì kiểm tra trong listRole
	bool exist =PathFileExists(sFileName);
	if(!exist){
		int begin = sFileName.ReverseFind('\\')+1;
		int end = sFileName.ReverseFind(L'.');
		CString roleName = sFileName.Mid(begin,end-begin);
		if(roleName.Left(4)=="Role")
			return NULL;
		CString szLstRole = L"Character\\ListRole.xml";
		ConvertFileName(szLstRole);
		//nếu không tồn tại file listRole thì tạo note root
		if(!PathFileExists(szLstRole)){
			xml.AddElem(L"ListRoles");			
		}
		else{
			xml.Load(szLstRole);
			xml.FindElem(L"ListRoles");
		}
		
		xml.IntoElem();
		while(xml.FindElem(L"Role")){
			if(xml.GetAttrib(L"ID")==roleName){
				sFileName=L"Character\\"+xml.GetAttrib(L"File");
				ConvertFileName(sFileName);
				exist =true;
				break;
			}
			
		}
		//nếu trong listRole không có thì lưu trong listRole và tạo file
		if(!exist){
			time_t timer;
			time(&timer);
			CTime t = timer;
			sFileName=t.Format(L"Role%d%m%Y%H%M%S.xml");
			xml.AddElem(L"Role");
			xml.AddAttrib(L"ID",roleName);
			xml.AddAttrib(L"File",sFileName);
			xml.Save(szLstRole);
			sFileName=L"Character\\"+sFileName;
			ConvertFileName(sFileName);
			return NULL;
		}
		
	}		
	xml.Load(sFileName);
	if(xml.GetDoc().GetLength()>0)
		return xml;
	return NULL;
}
CMarkup XMLSerialized::OpenFile(CString sFile){
	CMarkup xml;
	XMLSerialized::ConvertFileName(sFile);
	xml.Load(sFile);
	if(xml.GetDoc().GetLength()>0)
		return xml;
	return NULL;
}
void XMLSerialized::SaveFile(CMarkup* xml){
	xml->Save(sFileName);
}
void XMLSerialized::SaveInfo(){
	CMarkup xml = OpenFile();
	if(xml.GetDoc().GetLength()==0){
		Serialize();
		return;
	}
	std::map<int,DATA*>::const_iterator item;
	for(item=  m_mapping.begin(); item != m_mapping.end(); item ++){
		DATA* data = item->second;
		if(data->_tagType == TagType::Root){
			continue;
		}
		if(data->_tagType == TagType::Element){
			xml.ResetPos();			
			if(FindPath(data->_parent,&xml)){
				if(!xml.FindElem(data->_tagName)){
					xml.AddElem(data->_tagName);
				}
				if(data->_dataType!= DataType::None){
					CString result;
					switch(data->_dataType){
						case DataType::String:
							xml.SetData(*(CString*)data->_value);
						break;
						case DataType::Int:
							xml.SetData(*(int*)data->_value);
						break;
						case DataType::Bool:
							xml.SetData((int)*(bool*)data->_value);
						break;
						case DataType::ByteArray:
							ByteArrayToString(result,L"|",(byte*)data->_value);
							xml.SetData(result);
						break;
						case DataType::IntArray:
							IntArrayToString(result,L"|",(int*)data->_value);
							xml.SetData(result);
						break;
					}
				}
				continue;
			}
		}
		if(data->_tagType == TagType::Attribute){
			if(FindPath(data->_parent,&xml)){
				xml.OutOfElem();
				CString result;
				
				switch(data->_dataType){
					case DataType::String:
						xml.SetAttrib(data->_tagName,*(CString*)data->_value);
					break;
					case DataType::Int:
						xml.SetAttrib(data->_tagName,*(int*)data->_value);
					break;
					case DataType::Bool:
						xml.SetAttrib(data->_tagName,(int)*(bool*)data->_value);
					break;
					case DataType::ByteArray:
						ByteArrayToString(result,L"|",(byte*)data->_value);
						xml.SetAttrib(data->_tagName,result);
					break;
					case DataType::IntArray:
						IntArrayToString(result,L"|",(int*)data->_value);
						xml.SetAttrib(data->_tagName,result);
					break;
				}
			}
		}		
	}
	xml.Save(sFileName);
}
void XMLSerialized::SaveInfo(const CString& sName,const CString& sValue,TagType tagType,const CString& parent){
	CMarkup xml = OpenFile();
	if(FindPath(parent,&xml)){
		if(tagType== TagType::Element)
		{
			if(!xml.FindElem(sName))
				xml.AddElem( sName);
			if(sValue.GetLength()>0)
				xml.SetData(sValue);
			return;
		}
		if(tagType== TagType::Attribute)
		{
			xml.SetAttrib(sName,sValue);
			return;
		}
	}
	xml.Save(sFileName);
}
void XMLSerialized::SaveInfo(const CString& sName,const int& iValue,TagType tagType,const CString& parent){
	CMarkup xml = OpenFile();
	if(FindPath(parent,&xml)){
		if(tagType== TagType::Element)
		{
			if(!xml.FindElem(sName))
				xml.AddElem( sName);
			xml.SetData(iValue);
			return;
		}
		if(tagType== TagType::Attribute)
		{
			xml.SetAttrib(sName,iValue);
			return;
		}
	}
	xml.Save(sFileName);
}

CString XMLSerialized::LoadInfo(const CString& sName,TagType tagType,const CString parent=L""){
	CMarkup xml = OpenFile();
	
	if(tagType== TagType::Element){
		if(FindPath(sName,&xml))
			return xml.GetData();
	}
	if(tagType== TagType::Attribute){
		if(FindPath(parent,&xml))
			return xml.GetAttrib(sName);
	}
	return L"";
}


wstring XMLSerialized::UTF8ToUnicode( const string &str )
{
 int  len = 0;
 len = str.length();
 int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,
            0,
            str.c_str(),
            -1,
            NULL,
            0 ); 
 wchar_t *  pUnicode; 
 pUnicode = new  wchar_t[unicodeLen+1]; 
 memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
 ::MultiByteToWideChar( CP_UTF8,
         0,
         str.c_str(),
         -1,
         (LPWSTR)pUnicode,
         unicodeLen ); 
 wstring  rt; 
 rt = ( wchar_t* )pUnicode;
 delete  pUnicode;
   
 return  rt; 
}

string XMLSerialized::UnicodeToUTF8( const wstring& str )
{
 char*     pElementText;
 int    iTextLen;
 // wide char to multi char
 iTextLen = WideCharToMultiByte( CP_UTF8,
         0,
         str.c_str(),
         -1,
         NULL,
         0,
         NULL,
         NULL );
 pElementText = new char[iTextLen + 1];
 memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
 ::WideCharToMultiByte( CP_UTF8,
         0,
         str.c_str(),
         -1,
         pElementText,
         iTextLen,
         NULL,
         NULL );
 string strText;
 strText = pElementText;
 delete[] pElementText;
 return strText;
}