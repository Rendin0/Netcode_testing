#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string>
#include <iostream>
#include <Winsock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

SOCKET connections[10];
std::thread handlers[10];
size_t counter = 0;

void clientHandler(size_t index)
{
	char msg[256];
	while (true)
	{
		recv(connections[index], msg, sizeof(msg), NULL);
		for (int i = 0; i < counter; i++)
		{
			if (index == i)
				continue;

			send(connections[i], msg, sizeof(msg), NULL);
		}
	}
}

int server(char msg[256])
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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(2345);
	addr.sin_family = AF_INET;

	SOCKET s_listen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(s_listen, (SOCKADDR*)&addr, sizeof(addr));
	listen(s_listen, SOMAXCONN);


	SOCKET new_connection;
	for (int i = 0; i < 10; i++)
	{
		new_connection = accept(s_listen, (SOCKADDR*)&addr, &size_of_addr);
		if (new_connection == 0)
			std::cout << "Error. No connection.\n";
		else
		{
			handlers[i] = std::thread(clientHandler, i);
			std::cout << "New connection.\n";

			connections[i] = new_connection;
			counter++;
		}

	}
	system("pause");
}

int main()
{
	char msg[256];

	system("chcp 1251");
	std::thread serv(server, msg);
	while (true)
	{
	}
	serv.join();
	handlers->join();

	return 0;
}