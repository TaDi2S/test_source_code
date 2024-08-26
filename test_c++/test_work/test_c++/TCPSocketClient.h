#pragma once

#include "header.h"
#pragma comment(lib, "Ws2_32.lib")


class TCPSocketClient
{
public:
	WSADATA m_WsaData;
	SOCKET m_ConnectSocket;
	struct addrinfo *m_result = NULL, *m_ptr = NULL, m_hints;
	char m_Recvbuf[1024];
	int m_iResult;
	int m_Recvbuflen;

	bool m_ThreadFlag;

	std::thread m_recvThread;


	TCPSocketClient();
	~TCPSocketClient();
	int SocketInit();
	int SocketConnect(PCSTR ip, PCSTR port);
	int SocketSend(const string& data);
	int SocketSendShutDown();
	int SocketCleanUp();
	void SocketReceivedThread();
	void StartSocketRecivedThread();
	void StopSocketRecivedThread();
	string ReadFile(const string& filePath);
};

