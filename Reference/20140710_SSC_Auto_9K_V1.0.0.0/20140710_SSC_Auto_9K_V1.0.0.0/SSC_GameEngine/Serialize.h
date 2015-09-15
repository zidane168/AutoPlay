#pragma once
// CSerialize


#include "UserData.h"
#include "Markup.h"
#include <map>


using namespace std;


class __declspec(dllexport) XMLSerialized
{
public:
	XMLSerialized(const CString &fileName);
	~XMLSerialized();
	

	void Mapping(const CString &sNode,const CString &sParent,TagType cTagType,DataType cType,DWORD dwValue);
	bool Serialize();
	bool Deserialize();
	void StringToIntArray(const CString& strInput, const CString& strDelim, int* arrInt);
	void IntArrayToString(CString& strOutput, const CString& strDelim, int* arrInt);
	void StringToByteArray(const CString& strInput, const CString& strDelim, byte* arrInt);
	void ByteArrayToString(CString& strOutput, const CString& strDelim, byte* arrByte);
	bool FindPath(CString sPath,CMarkup* xml);
	CMarkup OpenFile();
	static CMarkup OpenFile(CString sPath);
	void SaveFile(CMarkup* xml);
	void SaveInfo();
	void SaveInfo(const CString& sName,const CString& sValue,TagType tagType,const CString& parent);
	void SaveInfo(const CString& sName,const int& iValue,TagType tagType,const CString& parent);
	CString LoadInfo(const CString& sName,TagType tagType,const CString parent);
	static void ConvertFileName(CString &sPath);
	static wstring UTF8ToUnicode( const string &str );
	static string UnicodeToUTF8( const wstring& str );

private:
	std::map<int,DATA*> m_mapping;
	CString sFileName;
};