#include "socketClient.h"
#include <thread>

int main()
{
	SocketClient *socketClient = new SocketClient();
	socketClient->Prepare();
	do {
		socketClient->ClearRecvBuf();
		socketClient->Receive();
		std::cout << socketClient->GetRecvStr() << std::endl;
	} while (socketClient->GetRecvStr() != "permision");

	std::thread sendThread;
	std::thread recvThread;

	sendThread = std::thread(SendThreadFunc, socketClient);
	recvThread = std::thread(RecvThreadFunc, socketClient);
	sendThread.join();
	recvThread.join();

	socketClient->Cleanup();
	delete socketClient;

	std::cin.get();
	return 0;
}