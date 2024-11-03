#pragma once

#define SHLM HKEY_LOCAL_MACHINE
#define SHCU HKEY_CURRENT_USER
#define SHCR HKEY_CLASSES_ROOT

#include "def.h"

class RegUtil
{
public:
	static BOOL SHRegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
	static BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize);
	static BOOL SHRegReadSize(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD &nSize);
	int GetSearchKeyCount(LPCTSTR lpKey);
	BOOL SetSearchKeyFirstPos();
	int GetSearchKeyData(CString &strKeyName);

	int GetSearchKeyNameValueCount(LPCTSTR lpKey);
	BOOL SetSearchKeyNameValueFirstPos();
	int GetSearchKeyNameValueData(CString &strName, CString &strValue);

	BOOL GetValueBinary();
	BOOL GetValueDWORD();
	BOOL GetValueString(LPCTSTR lpKey, LPCTSTR lpName, CString &strValue);
	BOOL GetValueInt();

	BOOL SetValueBinary();
	BOOL SetValueDWORD();
	BOOL SetValueString(LPCTSTR lpKey, LPCTSTR lpName, LPCTSTR lpValue);
	BOOL SetValueInt();


	BOOL Initialize(HKEY hKey, CString strRootReg);
	void Finalize();

	RegUtil();
	virtual ~RegUtil();

private:
	HKEY	m_hKey;
	HKEY	m_hSearchKey;
	HKEY	m_hSearchSubKey;

	int		m_nSearchKeyCnt;
	int		m_nSearchKeyCurCnt;
	int		m_nSearchSubKeyCnt;
	int		m_nSearchSubKeyCurCnt;

	CString m_strRootReg;
};

