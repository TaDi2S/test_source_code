#include "TCPSocketClient.h"

TCPSocketClient::TCPSocketClient() {
	m_ConnectSocket = INVALID_SOCKET;
	
	m_ThreadFlag = false;
}

TCPSocketClient::~TCPSocketClient() {
	SocketCleanUp();
}

int TCPSocketClient::SocketCleanUp() {
	closesocket(m_ConnectSocket);
	WSACleanup();
	return 0;
}

int TCPSocketClient::SocketInit() {
	// winsock 초기화
	m_iResult = WSAStartup(MAKEWORD(2, 3), &m_WsaData);
	if (m_iResult != 0) {
		std::cerr << "WSAStartup failed with error: " << m_iResult << std::endl;
		return 1;
	}

	ZeroMemory(&m_hints, sizeof(m_hints));
	m_hints.ai_family = AF_INET;
	m_hints.ai_socktype = SOCK_STREAM;
	m_hints.ai_protocol = IPPROTO_TCP;
	   
	return 0;
}

int TCPSocketClient::SocketConnect(PCSTR ip, PCSTR port) {
	// 서버 주소 및 포트 확인
	m_iResult = getaddrinfo(ip, port, &m_hints, &m_result);
	if (m_iResult != 0) {
		std::cerr << "getaddrinfo failed with error: " << m_iResult << std::endl;
		WSACleanup();
		return 1;
	}

	// getaddrinfo 호출로 반환된 첫 번째 주소에 연결을 시도합니다.
	for (m_ptr = m_result; m_ptr != NULL; m_ptr = m_ptr->ai_next) {
		// Create a SOCKET for connecting to the server
		m_ConnectSocket = socket(m_ptr->ai_family, m_ptr->ai_socktype, m_ptr->ai_protocol);
		if (m_ConnectSocket == INVALID_SOCKET) {
			std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return 1;
		}

		// 서버에 연결
		m_iResult = connect(m_ConnectSocket, m_ptr->ai_addr, (int)m_ptr->ai_addrlen);
		if (m_iResult == SOCKET_ERROR) {
			closesocket(m_ConnectSocket);
			m_ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(m_result);

	if (m_ConnectSocket == INVALID_SOCKET) {
		std::cerr << "Unable to connect to server!" << std::endl;
		WSACleanup();
		return 1;
	}

	return 0;
}

int TCPSocketClient::SocketSend(const string& data) {
	// Send an initial buffer
	m_iResult = send(m_ConnectSocket, data.c_str(), (int)data.length(), 0);
	if (m_iResult == SOCKET_ERROR) {
		std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
		closesocket(m_ConnectSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Bytes Sent: " << m_iResult << std::endl;

	return 0;
}

void TCPSocketClient::SocketReceivedThread() {
	char recvbuf[1024];
	int recvbuflen = 1024;
	while (m_ThreadFlag) {
		m_iResult = recv(m_ConnectSocket, recvbuf, recvbuflen, 0); // recv는 데이터 수신까지 기다림 -> 리턴값 양수가 정상, 0은 소켓 연결 종료, -1은 에러
		if (m_iResult > 0) {
			std::cout << "Bytes received: " << m_iResult << std::endl;
			std::cout << "Received data: " << std::string(recvbuf, m_iResult) << std::endl;
		}
		else if (m_iResult == 0) {
			std::cout << "Connection closed" << std::endl;
			break;
		}
		else {
			int error_code = WSAGetLastError();
			if (error_code == WSAEWOULDBLOCK) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Wait and retry
				continue;
			}
			else {
				std::cerr << "recv failed with error: " << error_code << std::endl;
				break;
			}
		}
	}
	std::cout << "Receive thread is stopping...\n";
}


void TCPSocketClient::StartSocketRecivedThread() {
	m_ThreadFlag = true;
	m_recvThread = std::thread(&TCPSocketClient::SocketReceivedThread, this);

}

void TCPSocketClient::StopSocketRecivedThread() {
	m_ThreadFlag = false;
	if (m_recvThread.joinable()) m_recvThread.join();
}



int TCPSocketClient::SocketSendShutDown() {

	// 더이상 보내지 않을거면 SD_SEND로 송신 shutdown *한번 종료하면 다시 연결 못함~
	m_iResult = shutdown(m_ConnectSocket, SD_SEND);
	if (m_iResult == SOCKET_ERROR) {
		std::cerr << "shutdown failed with error: " << WSAGetLastError() << std::endl;
		closesocket(m_ConnectSocket);
		WSACleanup();
		return 1;
	}

	return 0;
}

string TCPSocketClient::ReadFile(const string& filePath) {
	ifstream file(filePath);
	if (!file) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return "";
	}

	std::string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	return content;
}