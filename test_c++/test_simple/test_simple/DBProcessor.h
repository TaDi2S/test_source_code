#pragma once

#include "def.h"
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

class DBProcessor
{
public:
	SQLHENV hEnv = NULL;
	SQLHDBC hDbc = NULL;
	SQLHSTMT hStmt = NULL;
	SQLRETURN ret;
	SQLSMALLINT outConnStrLen;

	DBProcessor();
	virtual ~DBProcessor();
	
	int Connect();

	void DisConnet();

	bool executeQuery(const std::wstring& query);

	void fetchResults(PACKET_INFO_RES* packetStruct);

	DWORD GetSplunkInfo(unsigned long ul_AgentID, PACKET_INFO_RES* packetStruct);

};

