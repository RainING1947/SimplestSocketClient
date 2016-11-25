#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "advapi32.lib")

#pragma warning(disable:4996)

#define DEFAULT_BUFLEN 128
#define DEFAULT_PORT "27015"

class SocketClient
{
public:
	SocketClient() {}
	~SocketClient() {}
	void Prepare();
	void InitWinSock();
	void ResolveAddrPort();
	void ConnectToServer();
	void SendAndShutdown(std::string strSend);
	void Send(std::string strSend);
	void Shutdown();
	void ReceiveAndCleanup();
	//void Receive();
	int Receive();
	void Cleanup();
	std::string GetRecvStr();
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;
	char recvbuf[DEFAULT_BUFLEN] = { ' ' };
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
};

void SocketClient::Prepare()
{
	this->InitWinSock();
	this->ResolveAddrPort();
	this->ConnectToServer();
}

void SocketClient::InitWinSock()
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStarttup failed with error " << iResult << std::endl;
		system("pause");
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

void SocketClient::ResolveAddrPort()
{
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		std::cout << "Getaddrinfo failed with error " << iResult << std::endl;
		system("pause");
		return;
	}
}

void SocketClient::ConnectToServer()
{
	//Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		//Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family,
			ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
		{
			std::cout << "Socket failed with error " << WSAGetLastError() << std::endl;
			WSACleanup();
			system("pause");
			return;
		}

		//Connect to server
		iResult = connect(ConnectSocket, ptr->ai_addr,
			(int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		std::cout << "Unable to connect to server" << std::endl;
		WSACleanup();
		system("pause");
		return;
	}
}

void SocketClient::SendAndShutdown(std::string strSend)
{
	this->Send(strSend);
	this->Shutdown();
}

void SocketClient::Send(std::string strSend)
{
	size_t len = strSend.length();
	char *sendbuf = new char[len];
	strcpy(sendbuf, strSend.data());
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "Send failed with error " << WSAGetLastError() << std::endl;
		closesocket(ConnectSocket);
		WSACleanup();
		system("pause");
		return;
	}
	std::cout << "Bytes sent: " << iResult << " " << sendbuf << std::endl;
}

void SocketClient::Shutdown()
{
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "Shutdown failed with error " << WSAGetLastError() << std::endl;
		closesocket(ConnectSocket);
		WSACleanup();
		system("pause");
		return;
	}
}

void SocketClient::ReceiveAndCleanup()
{
	this->Receive();
	this->Cleanup();
}

//void SocketClient::Receive()
//{
//	do {
//		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
//		if (iResult > 0)
//			std::cout << "Bytes received: " << iResult << " " << recvbuf << std::endl;
//		else if (iResult == 0)
//			std::cout << "Connection closed" << std::endl;
//		else
//			std::cout << "recv failed with error " << WSAGetLastError() << std::endl;
//	} while (iResult > 0);
//}

int SocketClient::Receive()
{
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
		std::cout << "Bytes received: " << iResult << " " << recvbuf << std::endl;
	else if (iResult == 0)
		std::cout << "Connection closed" << std::endl;
	else
		std::cout << "recv failed with error " << WSAGetLastError() << std::endl;
	return iResult;
}

void SocketClient::Cleanup()
{
	closesocket(ConnectSocket);
	WSACleanup();
}

std::string SocketClient::GetRecvStr()
{
	std::string recvStr = this->recvbuf;
	return recvStr;
}

#endif //!SOCKET_CLIENT_H