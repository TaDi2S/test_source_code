// RegUtil.cpp: implementation of the RegUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "RegUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RegUtil::RegUtil()
{
	m_hSearchKey = NULL;
	m_hSearchSubKey = NULL;
}

RegUtil::~RegUtil()
{

}

//*********************************************************************************************
// RegUtil::Initialize
//	  :	Registry의 HKEY
//  - Visibility : public  
//	- Parameters
//	  : hKey		- 아래 값중 하나 (define문 참조)
//						#define SHLM HKEY_LOCAL_MACHINE
//						#define SHCU HKEY_CURRENT_USER
//						#define SHCR HKEY_CLASSES_ROOT
//		strRootReg	- Open할 Registry Key 값의 일부분 (hKey 부분을 제외한 부분)
//	- Retrun 
//	  : 정상적으로 처리되면 TRUE, 그외 FALSE
//	- COMMENTS:
//	  : 예) pReg->Initialize(SHLM, "SOFTWARE\\Coretrustl\\ContentManager\\Media Keeper 4.0"); 	
//*********************************************************************************************
BOOL RegUtil::Initialize(HKEY hKey, CString strRootReg)
{
	if (hKey == NULL)
		m_hKey = HKEY_LOCAL_MACHINE;
	else
		m_hKey = hKey;

	if (strRootReg == "")
		m_strRootReg = "SOFTWARE\\Coretrust\\ContentsManager\\Media Keeper 4.0";
	else
		m_strRootReg = strRootReg;

	HKEY key;

	if (RegCreateKeyEx(m_hKey, m_strRootReg, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_WRITE | KEY_READ, NULL, &key, NULL) != ERROR_SUCCESS) {
		return FALSE;
	}

	RegCloseKey(key);

	if (m_hSearchKey != NULL)
	{
		RegCloseKey(m_hSearchKey);
		m_hSearchKey = NULL;
	}

	if (m_hSearchSubKey != NULL)
	{
		RegCloseKey(m_hSearchSubKey);
		m_hSearchSubKey = NULL;
	}

	m_nSearchKeyCnt = 0;
	m_nSearchKeyCurCnt = 0;
	m_nSearchSubKeyCnt = 0;
	m_nSearchSubKeyCurCnt = 0;

	return TRUE;
}

//*********************************************************************************************
// RegUtil::Initialize
//	  :	Registry HKEY 핸들 닫기 
//  - Visibility : public  
//*********************************************************************************************
void RegUtil::Finalize()
{
	if (m_hSearchKey != NULL)
	{
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}

	if (m_hSearchSubKey != NULL)
	{
		RegCloseKey(m_hSearchSubKey);
		m_hSearchSubKey = NULL;
	}
}

BOOL RegUtil::GetValueInt()
{
	return GetValueDWORD();
}

//*********************************************************************************************
// RegUtil::GetValueString
//	  :	Registry에서 문자열 값 읽기 
//  - Visibility : public  
//	- Parameters
//	  : lpKey	- Open할 Registry Key 값의 일부분 (m_strRootReg를 제외한 부분)
//				  주의) m_strRootReg + "\\" + lpKey = 탐색할 Registry Key
//		lpName	- 읽어들일 문자열 name 
//		strValue- 읽어들일 문자열 name의 값
//	- Retrun 
//	  : 정상적으로 처리되면 TRUE, 그외 FALSE
//	- COMMENTS:
//	  : 주의) lpName에 대값 값이 없을때는 "NULL"을 되돌린다. 
//	  : 예) CString strValue; 
//			pReg->GetValueString("Common", "ServerName", strValue); 	
//*********************************************************************************************
BOOL RegUtil::GetValueString(LPCTSTR lpKey, LPCTSTR lpName, CString &strValue)
{
	CString strTemp;

	if (lpKey == (LPCTSTR)"")
	{
		strTemp = m_strRootReg;
	}
	else
	{
		strTemp = m_strRootReg;
		strTemp.TrimLeft("\\");
		strTemp = strTemp + "\\";
		strTemp = strTemp + lpKey;
	}

	HKEY	key;
	DWORD	dwDisp;
	DWORD	Size = MAX_PATH;
	char	chData[MAX_PATH];
	ZeroMemory(chData, MAX_PATH);

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, strTemp, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return FALSE;

	if (RegQueryValueEx(key, lpName, 0, NULL, (LPBYTE)chData, &Size) != ERROR_SUCCESS)
	{
		strValue = "";
		RegCloseKey(key);
		return FALSE;
	}

	strValue.Format("%s", chData);

	RegCloseKey(key);
	return TRUE;
}

BOOL RegUtil::GetValueDWORD()
{

	return TRUE;
}

BOOL RegUtil::GetValueBinary()
{

	return TRUE;
}

BOOL RegUtil::SetValueInt()
{

	return TRUE;
}

//*********************************************************************************************
// RegUtil::SetValueString
//	  :	Registry에서 문자열 값 쓰기 
//  - Visibility : public  
//	- Parameters
//	  : lpKey	- Open할 Registry Key 값의 일부분 (m_strRootReg를 제외한 부분)
//				  주의) m_strRootReg + "\\" + lpKey = 탐색할 Registry Key
//		lpName	- 저장할 문자열 name 
//		lpValue	- 저장할 문자열 name의 값
//	- Retrun 
//	  : 정상적으로 처리되면 TRUE, 그외 FALSE
//	- COMMENTS:
//	  : 예) CString strData = "DRM Server"; 
//			pReg->SetValueString("Common", "ServerName", strData); 	
//*********************************************************************************************
BOOL RegUtil::SetValueString(LPCTSTR lpKey, LPCTSTR lpName, LPCTSTR lpValue)
{
	CString strTemp;

	if (lpKey == (LPCTSTR)"")
	{
		lpKey = (LPCTSTR)m_strRootReg;
	}
	else
	{
		strTemp = m_strRootReg;
		strTemp.TrimLeft("\\");
		strTemp = strTemp + "\\";
		strTemp = strTemp + lpKey;
	}

	HKEY	key;
	DWORD	dwDisp;
	DWORD	Size = MAX_PATH;
	char	chData[MAX_PATH];
	ZeroMemory(chData, MAX_PATH);

	if (RegCreateKeyEx(m_hKey, strTemp, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return FALSE;

	if (RegSetValueEx(key, lpName, 0, REG_SZ, (LPBYTE)lpValue, strlen(lpValue) + 1) != ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return FALSE;
	}

	RegCloseKey(key);
	return TRUE;
}

BOOL RegUtil::SetValueDWORD()
{

	return TRUE;
}

BOOL RegUtil::SetValueBinary()
{

	return TRUE;
}

//*********************************************************************************************
// RegUtil::GetSearchKeyCount
//	  :	해당하는 Registry의 하부 키(regedit에서 폴더로 표시되는 부분)의 하부키 개수를 얻음  
//  - Visibility : public  
//	- Parameters
//	  : lpKey	- Open할 Registry Key 값의 일부분 (m_strRootReg를 제외한 부분)
//				  주의) m_strRootReg + "\\" + lpKey = 탐색할 Registry Key
//	- Retrun 
//	  : 1 이상	- 하부키가 다수 존재하고 첫번째 인덱스를 리턴
//		0		- 하부키가 존재하지 않음
//		-1		- Error
//	- COMMENTS:
//	  : 참고) SetSearchKeyFirstPos, GetSearchKeyNext
//*********************************************************************************************
int RegUtil::GetSearchKeyCount(LPCTSTR lpKey)
{
	CString strTemp;

	if (lpKey == (LPCTSTR)"")
	{
		lpKey = (LPCTSTR)m_strRootReg;
	}
	else
	{
		strTemp = m_strRootReg;
		strTemp.TrimLeft("\\");
		strTemp = strTemp + "\\";
		strTemp = strTemp + lpKey;
	}

	DWORD	dwCnt;

	if (RegOpenKeyEx(m_hKey, strTemp, NULL, KEY_READ, &m_hSearchKey) != ERROR_SUCCESS)
		return -1;

	if (RegQueryInfoKey(m_hSearchKey, NULL, NULL, NULL, &dwCnt, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		RegCloseKey(m_hSearchKey);
		m_hSearchKey = NULL;
		return -1;
	}

	m_nSearchKeyCnt = dwCnt;

	return m_nSearchKeyCnt;
}

//*********************************************************************************************
// RegUtil::SetSearchKeyFirstPos
//	  :	해당하는 Registry의 하부 키(regedit에서 폴더로 표시되는 부분)의 하부키의 Name을 얻음 
//		Position 초기화 
//  - Visibility : public  
//	- Retrun 
//	  : 정상적으로 세팅되면 TRUE, 그외 FALSE
//	- COMMENTS:
//	  : 참고) GetSearchKeyCount, GetSearchKeyData
//*********************************************************************************************
BOOL RegUtil::SetSearchKeyFirstPos()
{
	if (m_nSearchKeyCnt == 0)
	{
		return FALSE;
	}
	else
	{
		m_nSearchKeyCurCnt = 0;
		return TRUE;
	}
}

//*********************************************************************************************
// RegUtil::GetSearchKeyData
//	  :	해당하는 Registry의 하부 키(regedit에서 폴더로 표시되는 부분)의 하부키의 Name을 얻음 
//  - Visibility : public  
//	- Parameters
//	  : strKeyName	- Registry의 key(regedit에서 폴더로 표시되는 부분) 하부에 있는 Key Name
//	- Retrun 
//	  : 1	- 하부키가 존재하여 Name을 읽음 
//		0	- 하부키가 존재하지 않음
//		-1	- Error
//	- COMMENTS:
//	  : 참고) GetSearchKeyCount, SetSearchKeyFirstPos
//*********************************************************************************************
int RegUtil::GetSearchKeyData(CString &strKeyName)
{
	if (m_nSearchKeyCnt == 0)
	{
		return 0;
	}
	else if (m_nSearchKeyCnt == m_nSearchKeyCurCnt)
	{
		return 0;
	}

	DWORD	Size = MAX_PATH;
	char	chSubKey[MAX_PATH];
	ZeroMemory(chSubKey, MAX_PATH);

	if (RegEnumKeyEx(m_hSearchKey, m_nSearchKeyCurCnt, chSubKey, &Size, NULL, NULL, 0, NULL) != ERROR_SUCCESS)
	{
		RegCloseKey(m_hSearchKey);
		m_hSearchKey = NULL;
		return -1;
	}
	else
	{
		m_nSearchKeyCurCnt++;
	}

	strKeyName.Format("%s", chSubKey);

	return 1;
}


//*********************************************************************************************
// RegUtil::GetSearchKeyNameValueCount
//	  :	해당하는 Registry의 key(regedit에서 폴더로 표시되는 부분) 하부에 있는 Name과 Value 쌍의  
//		개수 
//  - Visibility : public  
//	- Parameters
//	  : lpKey	- Open할 Registry Key 값의 일부분 (m_strRootReg를 제외한 부분)
//				  주의) m_strRootReg + "\\" + lpKey = 탐색할 Registry Key
//	- Retrun 
//	  : 1 이상	- Name과 Value 쌍의  개수 
//		0		- 존재하지 않음
//		-1		- Error
//	- COMMENTS:
//	  : 참고) SetSearchKeyNameValueFirstPos, GetSearchKeyNameValueData
//*********************************************************************************************
int RegUtil::GetSearchKeyNameValueCount(LPCTSTR lpKey)
{
	CString strTemp;

	if (lpKey == (LPCTSTR)"")
	{
		lpKey = (LPCTSTR)m_strRootReg;
	}
	else
	{
		strTemp = m_strRootReg;
		strTemp.TrimLeft("\\");
		strTemp = strTemp + "\\";
		strTemp = strTemp + lpKey;
	}

	DWORD	dwCnt;

	if (RegOpenKeyEx(m_hKey, strTemp, NULL, KEY_READ, &m_hSearchSubKey) != ERROR_SUCCESS)
		return -1;

	if (RegQueryInfoKey(m_hSearchSubKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwCnt, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		RegCloseKey(m_hSearchSubKey);
		m_hSearchSubKey = NULL;
		return -1;
	}

	m_nSearchSubKeyCnt = dwCnt;

	return m_nSearchSubKeyCnt;
}

//*********************************************************************************************
// RegUtil::GetSearchKeyNameValueData
//	  :	해당하는 Registry의 key(regedit에서 폴더로 표시되는 부분) 하부에 있는 Name과 Value를 얻음 
//  - Visibility : public  
//	- Parameters
//	  : strName		- Registry의 key(regedit에서 폴더로 표시되는 부분) 하부에 있는 Name
//		strValue	- Registry의 key(regedit에서 폴더로 표시되는 부분) 하부에 있는 Value
//	- Retrun 
//	  : 1	- 성공 
//		0	- 존재하지 않음
//		-1	- 실패 
//	- COMMENTS:
//	  : 참고) GetSearchKeyNameValueCount, SetSearchKeyNameValueFirstPos
//*********************************************************************************************
int RegUtil::GetSearchKeyNameValueData(CString &strName, CString &strValue)
{
	if (m_nSearchSubKeyCnt == 0)
	{
		RegCloseKey(m_hSearchSubKey);
		return 0;
	}
	else if (m_nSearchSubKeyCnt == m_nSearchSubKeyCurCnt)
	{
		return 0;
	}

	DWORD	SizeName = MAX_PATH;
	DWORD	SizeValue = MAX_PATH;
	char	chName[MAX_PATH];
	char	chValue[MAX_PATH];
	ZeroMemory(chName, MAX_PATH);
	ZeroMemory(chValue, MAX_PATH);

	if (RegEnumValue(m_hSearchSubKey, m_nSearchSubKeyCurCnt, chName, &SizeName, NULL, NULL,
		(unsigned char*)chValue, &SizeValue) != ERROR_SUCCESS)
	{
		RegCloseKey(m_hSearchSubKey);
		m_hSearchSubKey = NULL;
		return -1;
		//m_nSearchSubKeyCurCnt++; 
	}
	else
	{
		m_nSearchSubKeyCurCnt++;
	}

	strName.Format("%s", chName);
	strValue.Format("%s", chValue);

	return 1;
}

//*********************************************************************************************
// RegUtil::SetSearchKeyNameValueFirstPos
//	  :	해당하는 Registry의 key(regedit에서 폴더로 표시되는 부분) 하부에 있는 Name과 Value를 
//		Position 초기화  
//  - Visibility : public  
//	- Retrun 
//	  : 정상적으로 세팅되면 TRUE, 그외 FALSE; 
//	- COMMENTS:
//	  : 참고) GetSearchKeyNameValueCount, GetSearchKeyNameValueData
//*********************************************************************************************
BOOL RegUtil::SetSearchKeyNameValueFirstPos()
{
	if (m_nSearchSubKeyCnt == 0)
	{
		return FALSE;
	}
	else
	{
		m_nSearchSubKeyCurCnt = 0;
		return TRUE;
	}
}

BOOL RegUtil::SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize)
{
	HKEY	key;
	DWORD	dwDisp;
	DWORD	Size;
	Size = nSize;

	if (RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return FALSE;

	if ((RegQueryValueEx(key, lpValue, NULL, NULL, (LPBYTE)lpRet, &Size)) != ERROR_SUCCESS)
	{
		strcpy(lpRet, lpDefault);
		RegCloseKey(key);
		return FALSE;
	}

	RegCloseKey(key);
	return TRUE;
}

BOOL RegUtil::SHRegReadSize(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD &nSize)
{
	HKEY	key;
	DWORD	dwDisp;

	if (RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return FALSE;

	if ((RegQueryValueEx(key, lpValue, NULL, NULL, NULL, &nSize)) != ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return FALSE;
	}

	RegCloseKey(key);
	return TRUE;
}

BOOL RegUtil::SHRegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return FALSE;

	if (RegSetValueEx(key, lpValue, 0, REG_SZ, (LPBYTE)lpData, strlen(lpData) + 1) != ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return FALSE;
	}

	RegCloseKey(key);
	return TRUE;
}
