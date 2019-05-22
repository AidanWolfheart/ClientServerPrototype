/*
* Author: Kiril Chomaniuk
* Credits given to https://bitbucket.org/sloankelly/youtube-source-repository/src/master/cpp/networking/
* and https://docs.microsoft.com/en-us/windows/desktop/winsock/getting-started-with-winsock
* for tutorials and sample code
*/

#include "pch.h"
#include "Server.h"

int Server::clientSize = sizeof(client);
int Server::activeConnections = 0;
sockaddr_in Server::client;

unsigned __stdcall Server::ClientSession(void * data)
{
	SOCKET clientSocket = (SOCKET)data;

	// Client name
	char host[NI_MAXHOST];		

	// The port that client connects to
	char service[NI_MAXSERV];	

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	// Get connection info
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " connected on port " << service << std::endl;
		activeConnections++;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " <<
			ntohs(client.sin_port) << std::endl;
	}

	///////////////////////////////////////////////

	char buf[4096];
	unsigned char md[SHA256_DIGEST_LENGTH];
	unsigned char output[SHA256_DIGEST_LENGTH];

	// Accept message and send back hashed
	while (true)
	{
		ZeroMemory(buf, 4096);
		ZeroMemory(md, SHA256_DIGEST_LENGTH);
		ZeroMemory(output, SHA256_DIGEST_LENGTH);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cout << "Error in receiving. Disconnecting " << service << std::endl;
			activeConnections--;
			return 1;
		}

		if (bytesReceived == 0)
		{
			std::cout << "Client disconnected " << service << std::endl;
			activeConnections--;
			return 1;
		}

		std::cout << service << ": " << std::string(buf, 0, bytesReceived) << std::endl;

		if (EncryptWithSHA256(buf, sizeof(buf), md))
		{
			for (int hashRound = 0; hashRound < HASH_ROUNDS - 1; hashRound++)
			{
				if (!EncryptWithSHA256(md, sizeof(md), md))
					break;
			}
		}

		// Send encrypted hash back to client
		send(clientSocket, reinterpret_cast<const char*>(md), bytesReceived + 1, 0);

	}

	// Close and clean up the socket
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}

bool Server::EncryptWithSHA256(void * input, unsigned long length, unsigned char * md)
{
	SHA256_CTX context;
	if (!SHA256_Init(&context))
		return false;

	if (!SHA256_Update(&context, (unsigned char*)input, length))
		return false;

	if (!SHA256_Final(md, &context))
		return false;

	return true;
}

void Server::Listen()
{
	WSADATA wsaData;
	struct sockaddr_in	hints;
	struct addrinfo *result = NULL, hintinfo;

	ZeroMemory(&hintinfo, sizeof(hintinfo));
	hintinfo.ai_family = AF_INET;
	hintinfo.ai_socktype = SOCK_STREAM;
	hintinfo.ai_protocol = 0;

	int wsaResult = WSAStartup(winSockVersion, &wsaData);
	if (wsaResult != 0)
	{
		std::cout << "WSAStartup failed: " << wsaResult;
	}

	hints.sin_family = AF_INET;
	hints.sin_port = htons(defaultPort);
	hints.sin_addr.S_un.S_addr = INADDR_ANY;

	// Create socket
	SOCKET ConnectSocket = socket(hintinfo.ai_family, hintinfo.ai_socktype, 0);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	bind(ConnectSocket, (sockaddr*)&hints, sizeof(hints));
	listen(ConnectSocket, SOMAXCONN);

	// Keep listening and accepting
	SOCKET clientSocket;
	while (clientSocket = accept(ConnectSocket, (sockaddr*)&client, &clientSize))
	{
		if (activeConnections < activeConnectionsLimit)
		{
			unsigned threadID;
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &Server::ClientSession, (void*)clientSocket, 0, &threadID);
		}
		else
		{
			std::cout << "Connection limit reached." << std::endl;
			char msg[64] = "Connection limit reached. Please try again";
			send(clientSocket, msg, sizeof(msg), 0);
		}
	}
}