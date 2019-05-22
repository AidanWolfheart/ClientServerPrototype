/*
* Author: Kiril Chomaniuk
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma once

#include "pch.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
private:
	SOCKET connectSocket;
	int defaultPort;
	WORD winSockVersion;
	std::string ipAddress;
public:
	Client(int port, int connLimit, WORD wsVer, std::string ip)
	{
		defaultPort = port;
		winSockVersion = wsVer;
		ipAddress = ip;
	}

	~Client()
	{
		ZeroMemory(&defaultPort, sizeof(defaultPort));
		ZeroMemory(&winSockVersion, sizeof(winSockVersion));
		ZeroMemory(&ipAddress, sizeof(ipAddress));
	}

	Client(const Client& other)
	{
		defaultPort = other.defaultPort;
		winSockVersion = other.winSockVersion;
		ipAddress = other.ipAddress;
	}

	Client& operator=(const Client& other)
	{
		if (this != &other)
		{
			ZeroMemory(&defaultPort, sizeof(defaultPort));
			ZeroMemory(&winSockVersion, sizeof(winSockVersion));
			ZeroMemory(&ipAddress, sizeof(ipAddress));

			defaultPort = other.defaultPort;
			winSockVersion = other.winSockVersion;
			ipAddress = other.ipAddress;
		}

		return *this;
	}
	void ProcessLoop();
	void Start();
	
};

