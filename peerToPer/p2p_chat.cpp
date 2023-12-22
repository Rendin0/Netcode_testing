#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string>
#include <iostream>
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

int counter = 20;

void serverHandler(SOCKET server_connection)
{
	int msg_size;

	while (true)
	{
		recv(server_connection, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';

		recv(server_connection, msg, msg_size, NULL);
		std::cout << "\u001b[s\u001b[100Z";

		printf("\u001b[%dA\u001b[2K", counter);
		std::cout << "Smn: " << msg << std::endl;
		counter--;
		std::cout << "\u001b[u";
		delete[] msg;
	}

}

int server(const char my_ip[256])
{
	WSAData wsa_data;
	WORD dll_version = MAKEWORD(2, 2);
	if (WSAStartup(dll_version, &wsa_data) != 0)
	{
		std::cout << "Error. Library didnt load. #" << WSAGetLastError() << std::endl;
		return 1;
	}
	SOCKADDR_IN addr;
	int size_of_addr = sizeof(addr);
	inet_pton(AF_INET, my_ip, &addr.sin_addr);
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
	Sleep(2000);

	while (true);
	srv_handler.join();

	closesocket(server_connection);
	closesocket(s_listen);
	WSACleanup();
	return 0;
}

int client(const char connection_ip[256])
{

	WSAData wsa_data;
	WORD dll_version = MAKEWORD(2, 2);
	if (WSAStartup(dll_version, &wsa_data) != 0)
	{
		std::cout << "Error. Library didnt load. #" << WSAGetLastError() << std::endl;
		return 1;
	}

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(connection_ip);
	addr.sin_port = htons(2345);
	addr.sin_family = AF_INET;

	SOCKET client_connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(client_connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		std::cout << "Error. Connection error (client)\n";
		return 1;
	}
	std::cout << "Connected (client).\n";
	std::string msg;
	Sleep(2000);
	for (int i = 0; i < counter; i++)
	{
		std::cout << std::endl;
	}
	while (true)
	{
		std::cout << ">_ ";
		std::getline(std::cin, msg);
		int msg_size = msg.size();
		send(client_connection, (char*)&msg_size, sizeof(int), NULL);
		if (send(client_connection, msg.c_str(), msg_size, NULL) != SOCKET_ERROR)
		{
			Sleep(40);
			if (counter < 10)
			{
				std::cout << "\u001b[1A\u001b[2K\u001b[1B";
				for (int i = 0; i < 20 - counter; i++)
				{
					std::cout << std::endl;
				}
				counter = 20;
			}
			// /*
			printf("\u001b[%dA\u001b[2K", counter + 1);
			std::cout << "You: " << msg << std::endl;
			counter--;
			printf("\u001b[%dB", counter);
			std::cout << "\u001b[2K";
			// */
		}
	}

	closesocket(client_connection);
	WSACleanup();
	return 0;
}

int main()
{
	system("chcp 1251");
	system("cls");

	const char connection_ip[256] = "10.5.2.8";
	const char my_ip[256] = "10.5.2.9";

	std::thread srv(server, my_ip);
	std::thread cln(client, connection_ip);

	srv.join();
	cln.join();

	return 0;
}
