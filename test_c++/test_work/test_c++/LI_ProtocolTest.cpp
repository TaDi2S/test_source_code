#include "LI_ProtocolTest.h"
#include "API.h"


LI_ProtocolTest::LI_ProtocolTest() {

}

LI_ProtocolTest::~LI_ProtocolTest() {

}

int LI_ProtocolTest::MakeHeader(PACKET_HEADER *header, int CMDType, unsigned int agentId, int dataSize) {

	memset(header, 0, sizeof(PACKET_HEADER));

	header->sh_ProtocolID = 0x1234;
	header->sh_ProtocolVer = PROTOCOL_VERSION_10;
	header->ul_AgentID = agentId;
	header->us_ProtocolCommand = CMDType;
	header->us_ProtocolDataSize = dataSize;

}

int LI_ProtocolTest::InitReq() {
	PACKET_HEADER tmpHeader;
	MakeHeader(&tmpHeader, CMD_INIT_REQ, 0, 10);

	unsigned char eBuf[1024];
	API		api;

	PACKET_INIT_INTEG_REQ stRequest;

	memset(&stRequest, 0, sizeof(PACKET_INIT_INTEG_REQ));

	stRequest.ulDecryptCheck = 0xCC12;
	stRequest.lNonce = 0x2523;
	stRequest.ul_ServiceIDSize

}