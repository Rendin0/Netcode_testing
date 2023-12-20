#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string>
#include <iostream>
#include <Winsock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

void serverHandler(SOCKET server_connection)
{
	char msg[256];

	while (true)
	{
		recv(server_connection, msg, sizeof(msg), NULL);
		std::cout << msg << std::endl << ">_ ";
	}

}

int server()
{
	WSAData wsa_data;
	WORD dll_version = MAKEWORD(2, 1);
	if (WSAStartup(dll_version, &wsa_data) != 0)
	{
		std::cout << "Error. Library error.\n";
		return EXIT_FAILURE;
	}
	SOCKADDR_IN addr;
	int size_of_addr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("192.168.0.12");
	addr.sin_port = htons(2345);
	addr.sin_family = AF_INET;

	SOCKET s_listen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(s_listen, (SOCKADDR*)&addr, sizeof(addr));
	listen(s_listen, SOMAXCONN);


	SOCKET server_connection;
	server_connection = accept(s_listen, (SOCKADDR*)&addr, &size_of_addr);

	if (server_connection == 1)
	{
		std::cout << "Error. No connection.\n";
		return 1;
	}
	std::thread srv_handler(serverHandler, server_connection);
	std::cout << "Connected (server).\n";

	while (true);
	srv_handler.join();

	return 0;
}

int client()
{

	WSAData wsa_data;
	WORD dll_version = MAKEWORD(2, 1);
	if (WSAStartup(dll_version, &wsa_data) != 0)
	{
		std::cout << "Error. Library didnt load.\n";
		return 1;
	}

	SOCKADDR_IN addr;
	int size_of_addr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("192.168.0.12");
	addr.sin_port = htons(2345);
	addr.sin_family = AF_INET;

	SOCKET client_connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(client_connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		std::cout << "Error. Connection error (client)\n";
		return 1;
	}
	std::cout << "Connected (client).\n";
	char msg[256];

	while (true)
	{
		std::cout << ">_ ";
		std::cin.getline(msg, sizeof(msg));
		send(client_connection, msg, sizeof(msg), NULL);
		Sleep(10);
	}

	return 0;
}

int main()
{
	system("chcp 1251");


	std::thread cln(client);
	std::thread srv(server);

	srv.join();
	cln.join();

	return 0;
}