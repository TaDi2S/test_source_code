// test_simple.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "def.h"
#include "RegUtil.h"
#include "Protocol.h"
#include "SocketClient.h"
#include "API.h"

PACKET_HEADER	m_ProtocolHeader;
PACKET_INFO_RES g_stSplunkInfo;

int main()
{	
	if (true) {
		API api;
		SOCKET_OBJ  *lpsockobj = NULL;
		unsigned char uc_X[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
		unsigned char uc_Y[16] = { 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		lpsockobj = (SOCKET_OBJ *)malloc(sizeof(SOCKET_OBJ));


		// EncKey 생성하여 socket->sz_EncKey 저장.
		api.GenerateEncKey(uc_X, uc_Y, (unsigned char*)lpsockobj->sz_DHEncKey);
		
		SocketClient client;

		client.Init("192.168.10.33", 7845);
		PACKET_HEADER tempHeader;

		tempHeader.sh_ProtocolID = 0x1234;
		tempHeader.ul_AgentID = 123456;
		tempHeader.sh_ProtocolVer = PROTOCOL_VERSION;
		tempHeader.us_ProtocolCommand = CMD_INFO_REQ;
		tempHeader.us_ProtocolDataSize = 0;

		char buffer[1024] = { 0, };

		memcpy(buffer, &tempHeader, sizeof(PACKET_HEADER));
		
		int a = 1;
		while (a != 0) {
			client.CreateSocket();
			client.Connect();

			client.SendData(buffer, sizeof(PACKET_HEADER));

			char recvBuf[4096] = { 0, };
			PACKET_HEADER recvHeader;
			PACKET_INFO_RES recvData;
			memset(&recvData, 0, sizeof(PACKET_INFO_RES));
			memset(&recvHeader, 0, sizeof(PACKET_HEADER));
			unsigned char tmpBuf[1024] = { 0, };

			client.receiveData(recvBuf, 4096);

			std::cout << recvBuf << std::endl;

			memcpy(&recvHeader, recvBuf, sizeof(PACKET_HEADER));
			
			memcpy(&tmpBuf, recvBuf + sizeof(PACKET_HEADER), recvHeader.us_ProtocolDataSize);
			lpsockobj->sz_DHEncKey[16] = 0x00;
			api.AES_Decrypt((unsigned char*)lpsockobj->sz_DHEncKey, tmpBuf, recvHeader.us_ProtocolDataSize);			// enc key
			
			std::cout << std::endl;

			int ptr = sizeof(PACKET_INFO_RES) - (sizeof(char*) * 3);

			memcpy(&recvData, tmpBuf, sizeof(PACKET_INFO_RES) - (sizeof(char*) * 3));

			printf("recvData.ul_SplunkFlag:%u\nrecvData.ul_SplunkUrlLen:%u\nrecvData.ul_ReservedLen:%u\nrecvData.ul_BaseGenreLen:%u\nrecvData.ul_EMMCacheFlag:%u\nrecvData.ul_OperationMode:%u\n",
				recvData.ul_SplunkFlag, recvData.ul_SplunkUrlLen, recvData.ul_ReservedLen, recvData.ul_BaseGenreLen, recvData.ul_EMMCacheFlag, recvData.ul_OperationMode);

			recvData.sz_SplunkUrl = (char*)malloc(recvData.ul_SplunkUrlLen);
			memcpy(recvData.sz_SplunkUrl, tmpBuf + ptr, recvData.ul_SplunkUrlLen);
			ptr += recvData.ul_SplunkUrlLen;
			recvData.sz_BaseGenre = (char*)malloc(recvData.ul_SplunkUrlLen);
			memcpy(recvData.sz_BaseGenre, tmpBuf + ptr, recvData.ul_BaseGenreLen);
			ptr += recvData.ul_BaseGenreLen;
			recvData.sz_Reserved = (char*)malloc(recvData.ul_SplunkUrlLen);
			memcpy(recvData.sz_Reserved, tmpBuf + ptr, recvData.ul_ReservedLen);

			printf("recvData.sz_SplunkUrl:%s\nrecvData.sz_BaseGenre:%s\nrecvData.sz_Reserved:%s\n",
				recvData.sz_SplunkUrl, recvData.sz_BaseGenre, recvData.sz_Reserved);
			client.CloseSocket();

			std::cout << "다시 안 하려면 0 아니면 다른 숫자 입력" << std::endl;

			std::cin >> a;
		}

		return 0;
	}

	Protocol c_protocol;
	if (false) {

		return 0;
	}

	if (false) {
		PACKET_HEADER tempHeader;

		tempHeader.sh_ProtocolID = 0x1234;
		tempHeader.ul_AgentID = 123456;
		tempHeader.sh_ProtocolVer = 0x0003;
		tempHeader.us_ProtocolCommand = CMD_INFO_REQ;
		tempHeader.us_ProtocolDataSize = 0;

		SOCKET_OBJ lpsockobj;

		strncpy(lpsockobj.sz_client_ip, "127.0.0.1", 10);

		memcpy(lpsockobj.buf, &tempHeader, sizeof(tempHeader));

		c_protocol.CMD_InfoReq_dbUse(&lpsockobj);

		return 0;
	}

	if (false) {
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
		g_stSplunkInfo.ul_SplunkFlag = atoi(szData);
		printf("ul_TestBedFlag: %d\n", g_stSplunkInfo.ul_SplunkFlag);

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





