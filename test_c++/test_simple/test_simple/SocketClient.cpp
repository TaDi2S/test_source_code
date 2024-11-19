#include "SocketClient.h"


SocketClient::SocketClient() {
	m_sock = 0;
}

SocketClient::~SocketClient() {

}

void SocketClient::Init(const std::string& server_ip, int port) {
	m_server_ip = server_ip;
	m_port = port;
}
void SocketClient::CreateSocket() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	if ((m_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cerr << "Socket creation error" << std::endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
}
void SocketClient::Connect() {
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_port);

	if (inet_pton(AF_INET, m_server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address/ Address not supported" << std::endl;
		closesocket(m_sock);
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	if (connect(m_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		std::cerr << "Connection Failed" << std::endl;
		closesocket(m_sock);
		WSACleanup();
		exit(EXIT_FAILURE);
	}
}
void SocketClient::SendData(char* buffer, size_t size) {
	send(m_sock, buffer, size, 0);
	std::cout << "Data sent." << std::endl;
}

void SocketClient::receiveData(char* buffer, size_t size) {
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(m_sock, &readfds);

	timeval timeout;
	timeout.tv_sec = 5;  // 5초 대기 시간 설정
	timeout.tv_usec = 0;

	int activity = select(m_sock + 1, &readfds, nullptr, nullptr, &timeout);

	if (activity > 0 && FD_ISSET(m_sock, &readfds)) {
		int received_size = recv(m_sock, buffer, size, 0);
		if (received_size > 0) {
			std::cout << "Data received: Size=" << received_size << " bytes." << std::endl;
		}
		else {
			std::cerr << "Receive error or connection closed." << std::endl;
		}
	}
	else if (activity == 0) {
		std::cerr << "Receive timeout." << std::endl;
	}
	else {
		std::cerr << "Select error." << std::endl;
	}
}

void SocketClient::CloseSocket() {
	if (m_sock != 0) {
		closesocket(m_sock);
		WSACleanup();
		m_sock = 0;
	}
}