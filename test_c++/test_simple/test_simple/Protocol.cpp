#include "Protocol.h"


Protocol::Protocol() {

}

Protocol::~Protocol() {

}

void Protocol::SetCommandToHeader(WORD cmdCode, SOCKET_OBJ *lpsockobj)
{
	// Socket�� Header setting
	m_ProtocolHeader.sh_ProtocolID = PROTOCOL_ID;
	//	m_ProtocolHeader.sh_ProtocolVer			= PROTOCOL_VERSION; 
	m_ProtocolHeader.us_ProtocolCommand = cmdCode;
	m_ProtocolHeader.us_ProtocolDataSize = 0;

	memcpy(lpsockobj->buf, &m_ProtocolHeader, sizeof(PACKET_HEADER));
	lpsockobj->buflen = sizeof(PACKET_HEADER);
}

DWORD Protocol::CMD_InfoReq_dbUse(SOCKET_OBJ *lpsockobj)
{
	memset(&m_ProtocolHeader, 0, sizeof(PACKET_HEADER));
	memcpy(&m_ProtocolHeader, (const void*)lpsockobj->buf, sizeof(PACKET_HEADER));

	lpsockobj->dwBeginTime = GetTickCount();

	DWORD retVal = SUCCESS_LICENSEISSUER;

	char ResultMessage[50] = { 0, };

	//-------------------------------------------------------------------
	// 	����� ������ �����͸� ���� �ʿ� ����
	//-------------------------------------------------------------------

	PACKET_INFO_RES stResponse;


	// DB�κ��� ul_AgentID�� ���� splunkUrl �ּ� DB���� �������� �ҷ����� �Լ� ����
	retVal = dbProcessor.GetSplunkInfo(m_ProtocolHeader.ul_AgentID, &stResponse);

	unsigned short totalSize = (sizeof(PACKET_INFO_RES) + stResponse.ul_SplunkUrlLen + stResponse.ul_ReservedLen - (sizeof(unsigned char*) * 2));

	unsigned short paddingSize = (16 - (totalSize % 16) % 16);

	// Info ����.
	if (retVal == SUCCESS_LICENSEISSUER)
	{
		// Header
		m_ProtocolHeader.sh_ProtocolID = PROTOCOL_ID;
		m_ProtocolHeader.ul_AgentID = m_ProtocolHeader.ul_AgentID;
		m_ProtocolHeader.us_ProtocolCommand = CMD_INFO_RES;
		m_ProtocolHeader.us_ProtocolDataSize = totalSize + paddingSize;

		unsigned char *eBuf = (unsigned char*)malloc(totalSize + paddingSize);
		int eBufSize = totalSize + paddingSize;

		// ��ȣȭ�� ���� �ӽú���
		memset(eBuf, 0, eBufSize);
		memcpy(eBuf, &stResponse, sizeof(PACKET_INFO_RES) - (sizeof(unsigned char*) * 2));

		memcpy(eBuf, stResponse.sz_SplunkUrl, stResponse.ul_SplunkUrlLen);
		memcpy(eBuf + stResponse.ul_SplunkUrlLen, stResponse.sz_Reserved, stResponse.ul_ReservedLen);
		memset(eBuf + stResponse.ul_SplunkUrlLen + stResponse.ul_ReservedLen, 0, paddingSize);
		/*
		// ��ȣȭ
		api.AES_Encrypt((unsigned char*)lpsockobj->sz_DHEncKey,	// enc key
			(unsigned char*)(eBuf), sizeof(eBuf));
			*/
			// Copy to buffer
		memcpy(lpsockobj->buf, &m_ProtocolHeader, sizeof(PACKET_HEADER));
		memcpy(lpsockobj->buf + sizeof(PACKET_HEADER), &eBuf, eBufSize);
		lpsockobj->buflen = sizeof(PACKET_HEADER) + eBufSize;
	}
	// ��û�� Agent ID �� DB�� �������� ���� ==> Server/Agent ������ ��߱��϶�.
	else if (retVal == ERROR_CSPTERMINAL_NOT_EXIST)
	{
		SetCommandToHeader(CMD_CERT_UPDATE_RES, lpsockobj);
	}
	// ��û�� Agent ID �� DIVICE_ID_DISABLED ������ ==> ������ �����ϰ� ���������϶�.
	else if (retVal == ERROR_DEVICE_ID_DISABLED)
	{
		SetCommandToHeader(CMD_DEVICE_ID_DISABLE_RES, lpsockobj);
	}
	else
	{
		//SetErrorMessage(retVal, &m_ProtocolHeader, lpsockobj);
	}

	lpsockobj->dwFinishTime = GetTickCount();

	//GetErrorLogMessage(retVal, ResultMessage);



	printf("[%s] CMD_INFO_REQ end ===== [AgentID=%ld, SplunkUrl=%s, baseGnere=%s, Reserved=%s, Message=%s, DurationTime=%.3f]",
		lpsockobj->sz_client_ip, m_ProtocolHeader.ul_AgentID, stResponse.sz_SplunkUrl, stResponse.sz_BaseGenre, stResponse.sz_Reserved, ResultMessage, float(lpsockobj->dwFinishTime - lpsockobj->dwBeginTime) / 1000);

	return retVal;
}

DWORD Protocol::CMD_InfoReq_RegUse(SOCKET_OBJ *lpsockobj) {
	memset(&m_ProtocolHeader, 0, sizeof(PACKET_HEADER));
	memcpy(&m_ProtocolHeader, (const void*)lpsockobj->buf, sizeof(PACKET_HEADER));

	lpsockobj->dwBeginTime = GetTickCount();

	DWORD retVal = SUCCESS_LICENSEISSUER;

	char ResultMessage[50] = { 0, };

	//-------------------------------------------------------------------
	// 	����� ������ �����͸� ���� �ʿ� ����
	//-------------------------------------------------------------------
		
	unsigned short totalSize = (sizeof(PACKET_INFO_RES) + g_stSplunkInfo.ul_SplunkUrlLen + g_stSplunkInfo.ul_ReservedLen - (sizeof(unsigned char*) * 3));

	unsigned short paddingSize = (16 - (totalSize % 16) % 16);

	// Info ����.
	if (retVal == SUCCESS_LICENSEISSUER)
	{
		// Header
		m_ProtocolHeader.sh_ProtocolID = PROTOCOL_ID;
		m_ProtocolHeader.ul_AgentID = m_ProtocolHeader.ul_AgentID;
		m_ProtocolHeader.us_ProtocolCommand = CMD_INFO_RES;
		m_ProtocolHeader.us_ProtocolDataSize = totalSize + paddingSize;

		unsigned char *eBuf = (unsigned char*)malloc(totalSize + paddingSize);
		int eBufSize = totalSize + paddingSize;

		// ��ȣȭ�� ���� �ӽú���
		memset(eBuf, 0, eBufSize);
		memcpy(eBuf, &g_stSplunkInfo, sizeof(PACKET_INFO_RES) - (sizeof(unsigned char*) * 3));

		memcpy(eBuf, g_stSplunkInfo.sz_SplunkUrl, g_stSplunkInfo.ul_SplunkUrlLen);
		memcpy(eBuf + g_stSplunkInfo.ul_SplunkUrlLen, g_stSplunkInfo.sz_Reserved, g_stSplunkInfo.ul_ReservedLen);
		memcpy(eBuf + g_stSplunkInfo.ul_SplunkUrlLen + g_stSplunkInfo.ul_ReservedLen, g_stSplunkInfo.sz_BaseGenre, g_stSplunkInfo.ul_BaseGenreLen);
		memcpy(eBuf + g_stSplunkInfo.ul_SplunkUrlLen + g_stSplunkInfo.ul_ReservedLen, g_stSplunkInfo.sz_BaseGenre, g_stSplunkInfo.ul_BaseGenreLen);
		memset(eBuf + g_stSplunkInfo.ul_SplunkUrlLen + g_stSplunkInfo.ul_ReservedLen + g_stSplunkInfo.ul_BaseGenreLen, 0, paddingSize);

		/*
		// ��ȣȭ
		api.AES_Encrypt((unsigned char*)lpsockobj->sz_DHEncKey,	// enc key
			(unsigned char*)(eBuf), sizeof(eBuf));
			*/
			// Copy to buffer
		memcpy(lpsockobj->buf, &m_ProtocolHeader, sizeof(PACKET_HEADER));
		memcpy(lpsockobj->buf + sizeof(PACKET_HEADER), &eBuf, eBufSize);
		lpsockobj->buflen = sizeof(PACKET_HEADER) + eBufSize;
	}
	// ��û�� Agent ID �� DB�� �������� ���� ==> Server/Agent ������ ��߱��϶�.
	else if (retVal == ERROR_CSPTERMINAL_NOT_EXIST)
	{
		SetCommandToHeader(CMD_CERT_UPDATE_RES, lpsockobj);
	}
	// ��û�� Agent ID �� DIVICE_ID_DISABLED ������ ==> ������ �����ϰ� ���������϶�.
	else if (retVal == ERROR_DEVICE_ID_DISABLED)
	{
		SetCommandToHeader(CMD_DEVICE_ID_DISABLE_RES, lpsockobj);
	}
	else
	{
		//SetErrorMessage(retVal, &m_ProtocolHeader, lpsockobj);
	}

	lpsockobj->dwFinishTime = GetTickCount();

	//GetErrorLogMessage(retVal, ResultMessage);


	printf("[%s] CMD_INFO_REQ end ===== [AgentID=%ld, SplunkUrl=%s, baseGnere=%s, Reserved=%s, EMMCacheFlag=%d, TestBedFlag=%d, Message=%s, DurationTime=%.3f]",
		lpsockobj->sz_client_ip, m_ProtocolHeader.ul_AgentID, g_stSplunkInfo.sz_SplunkUrl, g_stSplunkInfo.sz_BaseGenre, g_stSplunkInfo.sz_Reserved, g_stSplunkInfo.ul_EMMCacheFlag, g_stSplunkInfo.ul_TestBedFlag, ResultMessage, float(lpsockobj->dwFinishTime - lpsockobj->dwBeginTime) / 1000);

	return retVal;
}