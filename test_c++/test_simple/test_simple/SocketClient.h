#pragma once
#include "def.h"

class SocketClient
{
public:
	SocketClient();
	~SocketClient();

	void Init(const std::string& server_ip, int port);
	void CreateSocket();
	void Connect();
	void SendData(char* buffer, size_t size);
	void receiveData(char* buffer, size_t size);
	void CloseSocket();
private:
	std::string m_server_ip;
	int m_port;
	int m_sock;
};

