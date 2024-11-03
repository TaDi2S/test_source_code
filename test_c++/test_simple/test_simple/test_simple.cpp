// test_simple.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "def.h"
#include "RegUtil.h"
#include "Protocol.h"


PACKET_HEADER	m_ProtocolHeader;
PACKET_INFO_RES g_stSplunkInfo;

int main()
{	

	if (true) {
		int a, r;
		scanf("%d%d", &a, &r);

		long double rev = 0;
		long double k = 1;

		while (a != 0) {
			rev += a % r * k;
			k *= 10.;
			a /= r;
		}

		printf("%f\n", rev);

		return 0;
	}

	if (true) {
		PACKET_HEADER tempHeader;

		tempHeader.sh_ProtocolID = 0x1234;
		tempHeader.ul_AgentID = 123456;
		tempHeader.sh_ProtocolVer = 0x0003;
		tempHeader.us_ProtocolCommand = CMD_INFO_REQ;
		tempHeader.us_ProtocolDataSize = 0;

		SOCKET_OBJ lpsockobj;

		strncpy(lpsockobj.sz_client_ip, "127.0.0.1", 10);

		memcpy(lpsockobj.buf, &tempHeader, sizeof(tempHeader));

		CString strRegRoot;
			   
		HKEY hKey;
				
		strRegRoot = "SOFTWARE\\CORETRUST\\CoreCrypt for IPTV\\IPS Server_1300";

		DWORD dwSize = 0;

		RegUtil::SHRegReadSize(HKEY_LOCAL_MACHINE, strRegRoot, "SplunkURL", NULL, NULL,  dwSize);

		g_stSplunkInfo.sz_SplunkUrl = (char*)malloc(dwSize);
		g_stSplunkInfo.ul_SplunkUrlLen = dwSize;

		RegUtil::SHRegReadString(HKEY_LOCAL_MACHINE, strRegRoot, "SplunkURL", "", g_stSplunkInfo.sz_SplunkUrl, dwSize);
		printf("SplunkURL: %s, Len:%d\n", g_stSplunkInfo.sz_SplunkUrl, g_stSplunkInfo.ul_SplunkUrlLen);

		RegUtil::SHRegReadSize(HKEY_LOCAL_MACHINE, strRegRoot, "Reserved", NULL, NULL, dwSize);

		g_stSplunkInfo.sz_Reserved= (char*)malloc(dwSize);
		g_stSplunkInfo.ul_ReservedLen = dwSize;

		RegUtil::SHRegReadString(HKEY_LOCAL_MACHINE, strRegRoot, "Reserved", "", g_stSplunkInfo.sz_Reserved, dwSize);
		printf("Reserved: %s, Len:%d\n", g_stSplunkInfo.sz_Reserved, g_stSplunkInfo.ul_ReservedLen);

		RegUtil::SHRegReadSize(HKEY_LOCAL_MACHINE, strRegRoot, "BaseGenre", NULL, NULL, dwSize);

		g_stSplunkInfo.sz_BaseGenre = (char*)malloc(dwSize);
		g_stSplunkInfo.ul_BaseGenreLen= dwSize;

		RegUtil::SHRegReadString(HKEY_LOCAL_MACHINE, strRegRoot, "BaseGenre", "", g_stSplunkInfo.sz_BaseGenre, dwSize);
		printf("BaseGenre: %s, Len:%d\n", g_stSplunkInfo.sz_BaseGenre, g_stSplunkInfo.ul_BaseGenreLen);

		char szData[8];

		RegUtil::SHRegReadString(HKEY_LOCAL_MACHINE, strRegRoot, "EMMCacheFlag", "", szData, 8);
		g_stSplunkInfo.ul_EMMCacheFlag = atoi(szData);
		printf("ul_EMMCacheFlag: %d\n", g_stSplunkInfo.ul_EMMCacheFlag);

		RegUtil::SHRegReadString(HKEY_LOCAL_MACHINE, strRegRoot, "TestBedFlag", "", szData, 8);
		g_stSplunkInfo.ul_TestBedFlag = atoi(szData);
		printf("ul_TestBedFlag: %d\n", g_stSplunkInfo.ul_TestBedFlag);

		Protocol tmpProtocol;

		tmpProtocol.CMD_InfoReq_RegUse(&lpsockobj);

		return 0;
	}

	PACKET_INFO_RES_TEST1 test1;
	PACKET_INFO_RES_TEST2 test2;
	int size1 = sizeof(test1);
	int size2 = sizeof(test2);

	printf("size 1, 2 : %d, %d\n", size1, size2);

	test1.sz_SplunkUrl = (unsigned char*)malloc(6);

	test1.sz_Reserved = (unsigned char*)malloc(6);


	strncpy((char *)test1.sz_SplunkUrl, "abcde", 6);
	strncpy((char *)test2.sz_SplunkUrl, "abcde", 6);
	strncpy((char *)test1.sz_Reserved, "abcde", 6);

	size1 = sizeof(test1);
	printf("size 1, 2 : %d, %d\n", size1, size2);

	printf("%s\n", test1.sz_SplunkUrl);

	test1.ul_SplunkUrlLen = strlen((const char*)test1.sz_SplunkUrl) + 1;
	test1.ul_ReservedLen = strlen((const char*)test1.sz_Reserved) + 1;
	test1.ul_BaseGenre = 10;
	
	

	unsigned char * aa = reinterpret_cast<unsigned char*>(&test1);

	for (int i = 0; i < 45; i++) {
		printf("%02x ", aa[i]);
	}
	printf("\n");

	/*
	char* a = NULL;
	printf("NOTTING=%s,\\0=%s,\"\"=%s,NULL=%s,nullptr=%s\n", (char *)"NOTTHING", '\0', "", NULL, nullptr);
	printf("NOTTING=%s,\\0=%s,\"\"=%s,NULL=%s,nullptr=%s\n", (char *)"NOTTHING",'\0',"", a ? a : "" ,nullptr);
	*/

	
}





