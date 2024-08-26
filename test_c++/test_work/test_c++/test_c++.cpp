// test_c++.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "header.h"
#include "TSPacketSlice.h"
#include "TCPSocketClient.h"



int main()
{

	TCPSocketClient Client;

	Client.SocketInit();

	Client.SocketConnect("127.0.0.1","12124");

	Client.StartSocketRecivedThread();
	
	string input;
	while (true) {
		cout << "Enter any key to send file data (or 'exit' to quit): ";
		getline(cin, input);

		if (input == "exit") {
			break;
		}

		string fileData = Client.ReadFile("C:\\TestData\\text.txt");
		if (!fileData.empty()) {
			Client.SocketSend(fileData);
		}
	}

	Client.StopSocketRecivedThread();

	Client.~TCPSocketClient();


	return 0;
}
