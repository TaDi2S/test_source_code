#include "DBProcessor.h"



DBProcessor::DBProcessor()
{

}

DBProcessor::~DBProcessor()
{
	DisConnet();
}

int DBProcessor::Connect() {
	// ODBC 환경 핸들 할당 및 초기화
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) {
		std::cerr << "Unable to allocate environment handle" << std::endl;
		return false;
	}
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) {
		std::cerr << "Unable to allocate connection handle" << std::endl;
		return false;
	}
	std::wstring connStr = L"DRIVER={SQL Server};SERVER=127.0.0.1;DATABASE=CC4IPTV_1310;UID=cc4iptv;PWD=cc4iptv;";
	// 데이터베이스 연결
	SQLWCHAR outConnStr[1024];
	SQLSMALLINT outConnStrLen;
	SQLRETURN ret = SQLDriverConnectW(hDbc, NULL, (SQLWCHAR*)connStr.c_str(), SQL_NTS, outConnStr, sizeof(outConnStr), &outConnStrLen, SQL_DRIVER_COMPLETE);
	if (SQL_SUCCEEDED(ret)) {
		std::wcout << L"Connected successfully to the database!" << std::endl;
		return true;
	}
	else {
		std::wcerr << L"Failed to connect to the database." << std::endl;
		return false;
	}
	
}

void DBProcessor::DisConnet() {
	if (hStmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
	}
	if (hDbc) {
		SQLDisconnect(hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		hDbc = NULL;
	}
	if (hEnv) {
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		hEnv = NULL;
	}
}

bool DBProcessor::executeQuery(const std::wstring& query) {
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) {
		std::cerr << "Unable to allocate statement handle" << std::endl;
		return false;
	}

	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
	if (SQL_SUCCEEDED(ret)) {
		return true;
	}
	else {
		std::wcerr << L"Error executing query" << std::endl;
		return false;
	}
}

void DBProcessor::fetchResults(PACKET_INFO_RES* packetStruct) {
	if (!hStmt) {
		std::cerr << "Statement handle is not initialized." << std::endl;
		return;
	}
	// 변수 선언: 각 컬럼에 대응하는 변수들
	SQLINTEGER splunkUrlLen;
	SQLINTEGER reservedLen;
	char splunkUrl[350];
	char reserved[350];
	char baseGenre[350];
	SQLINTEGER baseGenreLen;

	SQLSMALLINT numCols;
	SQLNumResultCols(hStmt, &numCols);

	for (SQLUSMALLINT i = 1; i <= numCols; i++) {
		SQLWCHAR colName[256];
		SQLSMALLINT nameLength, dataType, decimalDigits, nullable;
		SQLULEN colSize;

		SQLDescribeColW(hStmt, i, colName, sizeof(colName) / sizeof(SQLWCHAR), &nameLength, &dataType, &colSize, &decimalDigits, &nullable);

		std::wcout << L"Column " << i << L": " << colName << L", DataType: " << dataType  << L", Size: "<< colSize << std::endl;
	}

	// 데이터를 가져오기
	while (SQLFetch(hStmt) == SQL_SUCCESS) {

		// 버퍼 초기화
		memset(splunkUrl, 0, sizeof(splunkUrl));
		memset(reserved, 0, sizeof(reserved));
		SQLLEN indicator;
		// splunkUrl 가져오기
		SQLRETURN ret = SQLGetData(hStmt, 3, SQL_C_CHAR, splunkUrl, sizeof(splunkUrl), &indicator);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			if (indicator == SQL_NULL_DATA) {
				std::cout << "NULL 데이터입니다." << std::endl;
			}
			else {
				std::cout << "읽은 데이터: " << splunkUrl << std::endl;
			}
		}
		else {
			// 오류 발생, 진단 정보 가져오기
			char sqlState[6];
			SQLINTEGER nativeError;
			char messageText[256];
			SQLSMALLINT textLength;
			SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, (SQLCHAR*)sqlState, &nativeError, (SQLCHAR*)messageText, sizeof(messageText), &textLength);

			std::cout << "SQLState: " << sqlState << std::endl;
			std::cout << "NativeError: " << nativeError << std::endl;
			std::cout << "indicator: " << indicator << std::endl;
			std::cout << "Message: " << messageText << std::endl;
		}

		// packetStruct에 splunkUrl 저장
		packetStruct->sz_SplunkUrl = (char*)malloc(strlen(splunkUrl) + 1);
		strcpy(packetStruct->sz_SplunkUrl, splunkUrl);

		// reserved 가져오기
		SQLGetData(hStmt, 4, SQL_C_CHAR, reserved, sizeof(reserved), NULL);

		// packetStruct에 reserved 저장
		packetStruct->sz_Reserved = (char*)malloc(strlen(reserved) + 1);
		strcpy(packetStruct->sz_Reserved, reserved);

		// baseGenre 가져오기
		SQLGetData(hStmt, 5, SQL_C_CHAR, baseGenre, sizeof(baseGenre), NULL);

		// packetStruct에 reserved 저장
		packetStruct->sz_BaseGenre = (char*)malloc(strlen(baseGenre) + 1);
		strcpy(packetStruct->sz_BaseGenre, baseGenre);
		
		// splunkUrlLen 가져오기
		SQLGetData(hStmt, 6, SQL_C_LONG, &splunkUrlLen, 0, NULL);
		packetStruct->ul_SplunkUrlLen = splunkUrlLen;
			   
		// reservedLen 가져오기
		SQLGetData(hStmt, 7, SQL_C_LONG, &reservedLen, 0, NULL);
		packetStruct->ul_ReservedLen = reservedLen;

		// baseGenreLen 가져오기
		SQLGetData(hStmt, 8, SQL_C_LONG, &baseGenreLen, 0, NULL);
		packetStruct->ul_BaseGenreLen = baseGenreLen;

		std::wcout << L"Splunk URL: " << splunkUrl << std::endl;
		std::wcout << L"reserved: " << reserved << std::endl;
	}
}

DWORD DBProcessor::GetSplunkInfo(unsigned long ul_AgentID, PACKET_INFO_RES* packetStruct) {
	if (Connect()) {
		if (executeQuery(L"SELECT  *, LEN(splunkUrl) AS SplunkUrlLen, LEN(reserved) AS ReservedLen, FROM TempSplunkTestTable WHERE agentId = 123456")) {
			fetchResults(packetStruct);

			// 결과 출력
			std::cout << "Splunk URL Length: " << packetStruct->ul_SplunkUrlLen << std::endl;
			std::cout << "Base Genre: " << packetStruct->sz_BaseGenre << std::endl;
			std::cout << "Reserved Length: " << packetStruct->ul_ReservedLen << std::endl;
			std::cout << "Splunk URL: " << packetStruct->sz_SplunkUrl << std::endl;
			std::cout << "Reserved: " << packetStruct->sz_Reserved << std::endl;

		}
		DisConnet();
	}
	return SUCCESS_LICENSEISSUER;
}