/*
* Author: Kiril Chomaniuk
* Credits given to https://bitbucket.org/sloankelly/youtube-source-repository/src/master/cpp/networking/
* and https://docs.microsoft.com/en-us/windows/desktop/winsock/getting-started-with-winsock
* for tutorials and sample code
*/

#include "pch.h"
#include "Client.h"

void Client::ProcessLoop()
{
	char buf[4096];
	std::string userInput;

	do
	{
		// Get user text
		std::cout << "> ";
		std::getline(std::cin, userInput);

		if (userInput.size() > 0)		// If user typed anything
		{
			// Send the text
			int sendResult = send(connectSocket, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(connectSocket, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					// Echo response to console
					std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
				}
			}
		}

	} while (userInput.size() > 0);
}

void Client::Start()
{
	WSADATA wsaData;
	struct addrinfo *result = NULL, *ptr = NULL, hintinfo;
	struct sockaddr_in	hints;

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
	inet_pton(AF_INET, std::string(ipAddress).c_str(), &hints.sin_addr);

	/////////////////////////////////////////////////////

	connectSocket = socket(hintinfo.ai_family, hintinfo.ai_socktype, 0);

	if (connectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	int connResult = connect(connectSocket, (sockaddr*)&hints, sizeof(hints));
	if (connResult == SOCKET_ERROR)
	{
		std::cout << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(connectSocket);
		WSACleanup();
	}

	ProcessLoop();

	// Close and clean up
	closesocket(connectSocket);
	WSACleanup();
}

