#pragma once
#include "header.h"

using namespace std;

class CompareFile {
public:
	string m_File1Path;
	string m_File2Path;
	string m_FileOutPath;
	
	FILE* m_File1;
	FILE* m_File2;
	FILE* m_FileOut;

	char* m_File1Buffer;
	char* m_File2Buffer;
	char* m_FileOutBuffer;

	unsigned long long m_File1Size;
	unsigned long long m_File2Size;
	unsigned long long m_FileOutSize;

public:
	CompareFile();
	~CompareFile();

	// 어느 패킷이 다른지 비교해서 두줄 씩
	int ComparePacketFromCSV(string File1Path, string File2Path, string FileOutPath);
private:



};