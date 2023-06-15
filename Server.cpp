#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "Server.hpp"

Server::Server(const char *port)
{
	addrinfo hints;
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	addrinfo *servinfo;
	int status = getaddrinfo(NULL, port, &hints, &servinfo);
	if (status != 0)
		throw InitFailed(std::string("Error: getaddrinfo: ") + gai_strerror(status));

	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (sockfd == -1)
		throw InitFailed(std::string("Error: socket: ") + strerror(errno));

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw InitFailed(std::string("Error: fcntl: ") + strerror(errno));

	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw InitFailed(std::string("Error: setsockopt: ") + strerror(errno));

	if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
		throw InitFailed(std::string("Error: bind: ") + strerror(errno));

	freeaddrinfo(servinfo);

	if (listen(sockfd, listenTimeout) == -1)
		throw InitFailed(std::string("Error: listen: ") + strerror(errno));

	pollfd server_poll_fd;
	server_poll_fd.fd = sockfd;
	server_poll_fd.events = POLLIN;
	pollfds.push_back(server_poll_fd);
}

Server::~Server()
{
	close(sockfd);
}

void Server::pollClientEvents()
{
	int event_count = 0;
	while ((event_count = poll(pollfds.data(), pollfds.size(), -1)) != -1)
	{
		for (std::vector<pollfd>::iterator pollfd_iter = pollfds.begin();
			pollfd_iter != pollfds.end() && event_count > 0; pollfd_iter++)
		{
			if (pollfd_iter->revents == 0)
				continue;
			event_count--;
			if (pollfd_iter->fd == sockfd)
				registerNewUser();
			else
				handleClientMessage(&pollfd_iter->fd);
		}
		// TODO(Hans): erase disconnected users from pollfds vector
	}
	std::cerr << "Error: poll: " << strerror(errno) << std::endl;
}

void Server::registerNewUser()
{
	std::cout << "New connection!" << std::endl;
	sockaddr_storage client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	int client_fd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_size);
	if (client_fd == -1)
	{
		close(sockfd);
		std::cerr << "Error: accept: " << strerror(errno) << std::endl;
		exit(1);
	}
	pollfd client_poll_fd;
	client_poll_fd.fd = client_fd;
	client_poll_fd.events = POLLIN;
	pollfds.push_back(client_poll_fd);
}

void Server::handleClientMessage(int *client_fd)
{
	std::cout << "Client sent something!" << std::endl;
	// TODO(Hans): Find way to detect/handle messages longer than msg_buf_size
	int bytes_read = recv(*client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read == -1)
		throw RecvFailed(std::string("Error: recv: ") + strerror(errno));
	if (bytes_read == 0)
	{
		std::cout << "Client closed connection!" << std::endl;
		close(*client_fd);
		*client_fd = -1;
		return;
	}
	buffer[bytes_read] = '\0';
	std::cout << buffer << std::endl;
	emit(*client_fd, buffer);
}

void Server::sendAllData(int client_fd, const char *msg)
{
	int	bytes_left = strlen(msg);
	while (bytes_left > 0)
	{
		int bytes_sent = send(client_fd, msg, bytes_left, 0);
		if (bytes_sent == -1)
			throw SendFailed(std::string("Error: send: ") + strerror(errno));
		bytes_left -= bytes_sent;
	}
}

void Server::emit(int client_fd, const char *msg)
{
	for (std::vector<pollfd>::iterator it = this->pollfds.begin();
		it != this->pollfds.end(); ++it)
	{
		if (it->fd != sockfd && it->fd != client_fd)
			sendAllData(it->fd, msg);
	}
}
