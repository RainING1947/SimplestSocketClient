#include "socketClient.h"

int main()
{
	SocketClient *socketClient = new SocketClient();
	socketClient->Prepare();
	socketClient->SendAndShutdown("I am sending");	
	socketClient->ReceiveAndCleanup();
	std::cout << "recv string: " << socketClient->GetRecvStr() << std::endl;
	
	std::cin.get();
	return 0;
}