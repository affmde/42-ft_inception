#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>

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

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error: fcntl: " << strerror(errno) << std::endl;
		exit(1);
	}

	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		std::cerr << "Error: setsockopt: " << strerror(errno) << std::endl;
		exit(1);
	} 

	if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		std::cerr << "Error: bind: " << strerror(errno) << std::endl;
		exit(1);
	}

	freeaddrinfo(servinfo);

	if (listen(sockfd, BACKLOG) == -1)
	{
		std::cerr << "Error: listen: " << strerror(errno) << std::endl;
		exit(1);
	}

	const int poll_fd_count = 1;
	pollfd poll_fds[poll_fd_count];
	poll_fds[0].fd = sockfd;
	poll_fds[0].events = POLLIN;

	int event_count = 0;
	while ((event_count = poll(poll_fds, poll_fd_count, -1)) != -1)
	{
		for (size_t i = 0; i < poll_fd_count && event_count > 0; i++)
		{
			if (!(poll_fds[i].revents & POLLIN))
				continue;
			event_count--;
			sockaddr_storage client_addr;
			socklen_t client_addr_size = sizeof(client_addr);
			int client_fd = accept(poll_fds[i].fd, (sockaddr *)&client_addr, &client_addr_size);
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
		}
	}
	std::cerr << "Error: poll: " << strerror(errno) << std::endl;

	if (close(sockfd) == -1)
	{
		std::cerr << "Error: close: " << strerror(errno) << std::endl;
		exit(1);
	}
}
