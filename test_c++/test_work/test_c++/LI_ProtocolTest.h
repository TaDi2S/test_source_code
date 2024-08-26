#pragma once

#include "header.h"

class LI_ProtocolTest
{
public:




public:
	LI_ProtocolTest();
	~LI_ProtocolTest();
public:
	int MakeHeader(PACKET_HEADER *header, int CMDType, unsigned int agentId, int dataSize);
	int InitReq();
};

