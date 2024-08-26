#include "TSPacketSlice.h"


TSPacketSlice::TSPacketSlice() {

}

TSPacketSlice::TSPacketSlice(string inFilePath, string outFilePath, unsigned long long packetCnt) {
	m_FileInPath = inFilePath;
	m_FileOutPath = outFilePath;
	m_PacketCnt = packetCnt;
}

TSPacketSlice::~TSPacketSlice() {

}

int TSPacketSlice::SliceTSPacket() {
	   
	m_ReadFile = fopen(m_FileInPath.c_str(), "rb");
	if (m_ReadFile == NULL) {
		perror("Error opening file");
		return -1;
	}

	// 파일 크기를 구합니다.
	fseek(m_ReadFile, 0, SEEK_END);
	m_FileSize = ftell(m_ReadFile);
	rewind(m_ReadFile);

	// 파일 크기 + 1 만큼의 메모리를 할당합니다.
	m_Buffer = (char *)malloc(sizeof(char) * (m_FileSize + 1));
	if (m_Buffer == NULL) {
		perror("Memory error");
		fclose(m_ReadFile);
		return -1;
	}

	// 파일을 읽습니다.
	size_t bytesReadSize = fread(m_Buffer, 1, m_FileSize, m_ReadFile);
	m_Buffer[bytesReadSize] = '\0'; // 널 종단 문자를 추가합니다.

	// 메모리를 해제하고 파일을 닫습니다.

	fclose(m_ReadFile);

	// 파일 열기 (쓰기 모드, 없으면 새로 생성)
	m_WriteFile = fopen(m_FileOutPath.c_str(), "wb"); // 이진 모드로 쓰기
	if (m_WriteFile == NULL) {
		perror("파일 열기 실패");
		return -1;
	}

	int cnt = 0;
	int packatCnt = 0;
	stringstream strStream;

	cnt = 1;
	for (int i = 0; i < m_FileSize; i++) {

		char data = m_Buffer[i];
		if (i != 0 && i % PACKET_SIZE == 0) {
			cnt = 0;
			strStream << "\n";
			string tmpStr = strStream.str();

			size_t dataSize = tmpStr.size();

			// 파일에 데이터 쓰기
			size_t written = fwrite(tmpStr.c_str(), 1, dataSize, m_WriteFile);
			if (written != dataSize) {
				perror("파일 쓰기 실패");
				fclose(m_WriteFile);
				return -1;
			}
			strStream.str("");
			strStream << hex << setw(2) << setfill('0') << static_cast<int>(static_cast<unsigned char>(data));
			strStream << ",";
			packatCnt++;
			printf("pCnt : %d\n", packatCnt);
			cnt = 1;
			if (packatCnt == m_PacketCnt) break;
		}
		else {
			strStream << hex << setw(2) << setfill('0') << static_cast<int>(static_cast<unsigned char>(data));
			strStream << ",";
			cnt++;
		}
		if (i == m_FileSize - 1) {
			string tmpStr = strStream.str();

			size_t dataSize = tmpStr.size();

			// 파일에 데이터 쓰기
			size_t written = fwrite(tmpStr.c_str(), 1, dataSize, m_WriteFile);
			if (written != dataSize) {
				perror("파일 쓰기 실패");
				fclose(m_WriteFile);
				return -1;
			}
		}
	}

	free(m_Buffer);
	// 파일 닫기
	fclose(m_WriteFile);

	printf("파일 쓰기 성공\n");


	return 0;
}