#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BACKLOG 10

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv port password" << std::endl;
		exit(1);
	}

	addrinfo hints;
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	addrinfo *servinfo;
	int status = getaddrinfo(NULL, argv[1], &hints, &servinfo);
	if (status != 0)
	{
		std::cerr << "Error: getaddrinfo: " << gai_strerror(status) << std::endl;
		exit(1);
	}

	int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (sockfd == -1)
	{
		std::cerr << "Error: socket: " << strerror(errno) << std::endl;
		exit(1);
	}

	status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	if (status == -1)
	{
		std::cerr << "Error: bind: " << strerror(errno) << std::endl;
		exit(1);
	}

	status = listen(sockfd, BACKLOG);
	if (status == -1)
	{
		std::cerr << "Error: listen: " << strerror(errno) << std::endl;
		exit(1);
	}

	sockaddr_storage client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	int client_fd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_size);
	if (client_fd == -1)
	{
		std::cerr << "Error: accept: " << strerror(errno) << std::endl;
		exit(1);
	}

	unsigned char msg_buf[1024];
	int bytes_read = recv(client_fd, msg_buf, sizeof(msg_buf), 0);
	if (bytes_read == -1)
	{
		std::cerr << "Error: recv: " << strerror(errno) << std::endl;
		exit(1);
	}
	if (bytes_read == 0)
	{
		std::cout << "Client closed connection!" << std::endl;
		exit(1);
	}

	std::cout << msg_buf << std::endl;

	status = close(sockfd);
	if (status == -1)
	{
		std::cerr << "Error: listen: " << strerror(errno) << std::endl;
		exit(1);
	}

	freeaddrinfo(servinfo);
}
