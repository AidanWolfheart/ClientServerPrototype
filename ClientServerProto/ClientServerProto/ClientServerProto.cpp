/*
* Author: Kiril Chomaniuk
*/

#include "pch.h"
#include "Server.h"

int main()
{
	Server server(12300, 20, MAKEWORD(2,2),"127.0.0.1");
	server.Listen();

	return 0;
}
