#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>
#include <string>
#include <process.h>
#include <openssl/sha.h>

#pragma comment(lib, "Ws2_32.lib")

#define HASH_ROUNDS 10000
#pragma once

class Server
{
private:
	// Socket client 
	static sockaddr_in client;

	// Size of the client socket
	static int clientSize;

	// Amount of current active connections - limit would be defined in macros
	static int activeConnections;

	int defaultPort, activeConnectionsLimit;
	WORD winSockVersion;
	std::string ipAddress;

public:
	Server(int port, int connLimit, WORD wsVer, std::string ip)
	{
		defaultPort = port;
		activeConnectionsLimit = connLimit;
		winSockVersion = wsVer;
		ipAddress = ip;
	}

	~Server()
	{
		ZeroMemory(&defaultPort, sizeof(defaultPort));
		ZeroMemory(&activeConnectionsLimit, sizeof(activeConnectionsLimit));
		ZeroMemory(&winSockVersion, sizeof(winSockVersion));
		ZeroMemory(&ipAddress, sizeof(ipAddress));
	}

	Server(const Server& other)
	{
		defaultPort = other.defaultPort;
		activeConnectionsLimit = other.activeConnectionsLimit;
		winSockVersion = other.winSockVersion;
		ipAddress = other.ipAddress;
	}

	Server& operator=(const Server& other)
	{
		if (this != &other)
		{
			ZeroMemory(&defaultPort, sizeof(defaultPort));
			ZeroMemory(&activeConnectionsLimit, sizeof(activeConnectionsLimit));
			ZeroMemory(&winSockVersion, sizeof(winSockVersion));
			ZeroMemory(&ipAddress, sizeof(ipAddress));

			defaultPort = other.defaultPort;
			activeConnectionsLimit = other.activeConnectionsLimit;
			winSockVersion = other.winSockVersion;
			ipAddress = other.ipAddress;
		}	

		return *this;
	}

	// Encrypts char[] with SHA
	static bool EncryptWithSHA256(void* input, unsigned long length, unsigned char* md);
	// Assigns thread to a client session
	static unsigned __stdcall ClientSession(void *data);
	void Listen();
};

