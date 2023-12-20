#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string>
#include <iostream>
#include <Winsock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

SOCKET connection;

void serverHandler()
{
	char msg[256];
	while (true)
	{
		recv(connection, msg, sizeof(msg), NULL);
		std::cout << msg << std::endl << ">_ ";
	}
}

int client()
{
	WSAData wsa_data;
	WORD dll_version = MAKEWORD(2, 1);
	if (WSAStartup(dll_version, &wsa_data) != 0)
	{
		std::cout << "Error. Library didnt load.\n";
		return EXIT_FAILURE;
	}

	SOCKADDR_IN addr;
	int size_of_addr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(2345);
	addr.sin_family = AF_INET;

	connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		std::cout << "Error. Connection error (client)\n";
		return 1;
	}

	std::thread handler(serverHandler);
	std::cout << "Connection successful (client).\n";

	char msg2[256];
	while (true)
	{
		std::cout << ">_ ";
		std::cin.getline(msg2, sizeof(msg2));
		send(connection, msg2, sizeof(msg2), NULL);
		Sleep(10);
	}

	handler.join();
	system("pause");
	return 0;
}

int main()
{
	system("chcp 1251");
	client();

	return 0;
}