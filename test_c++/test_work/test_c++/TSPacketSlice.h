#pragma once
#include "header.h"

using namespace std;

class TSPacketSlice
{
public:
	string m_FileInPath;
	string m_FileOutPath;

	FILE* m_ReadFile;
	FILE* m_WriteFile;
	char* m_Buffer;

	unsigned long long m_FileSize;

	unsigned long long m_PacketCnt;

public:
	TSPacketSlice(string inFilePath, string outFilePath, unsigned long long paketCnt);
	TSPacketSlice();
	~TSPacketSlice();

	int SliceTSPacket();
};

