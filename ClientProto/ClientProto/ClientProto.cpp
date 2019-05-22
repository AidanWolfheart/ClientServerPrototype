/*
* Author: Kiril Chomaniuk
*/

#include "pch.h"
#include "Client.h"

int main()
{
	Client client(12300, 20, MAKEWORD(2, 2), "127.0.0.1");
	client.Start();

	return 0;
}

