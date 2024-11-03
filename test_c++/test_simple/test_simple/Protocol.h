#pragma once


#include "def.h"
#include "DBProcessor.h"

extern PACKET_INFO_RES g_stSplunkInfo;

class Protocol
{
public:
	Protocol();
	virtual ~Protocol();

	void SetCommandToHeader(WORD cmdCode, SOCKET_OBJ *lpsockobj);

	DWORD CMD_InfoReq_dbUse(SOCKET_OBJ *lpsockobj);
	DWORD CMD_InfoReq_RegUse(SOCKET_OBJ *lpsockobj);
protected:
	PACKET_HEADER	m_ProtocolHeader;
	DBProcessor dbProcessor;
};

