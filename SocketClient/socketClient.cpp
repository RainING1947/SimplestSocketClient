#include "socketClient.h"

int main()
{
	SocketClient *socketClient = new SocketClient();
	socketClient->Prepare();
	std::string str;
	do {
		std::cout << "Please input string to send: ";
		std::cin >> str;
		if (str != "$")
			socketClient->Send(str);
	} while (str != "$");
	socketClient->Shutdown(); //Shutdown means Client has finished transfering NOW Server can send
	while (socketClient->Receive())
	{
		std::cout << "recv string: " << socketClient->GetRecvStr() << std::endl;
	}	
	socketClient->Cleanup();
	delete socketClient;

	std::cin.get();
	return 0;
}