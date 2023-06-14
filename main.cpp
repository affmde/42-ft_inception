#include <iostream>

#include "Server.hpp"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv port password" << std::endl;
		exit(1);
	}

	Server server(argv[1]);
	server.poll_client_events();
}
