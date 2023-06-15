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
	{
		close(sockfd);
		throw InitFailed(std::string("Error: fcntl: ") + strerror(errno));
	}

	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		close(sockfd);
		throw InitFailed(std::string("Error: setsockopt: ") + strerror(errno));
	}

	if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		close(sockfd);
		throw InitFailed(std::string("Error: bind: ") + strerror(errno));
	}

	freeaddrinfo(servinfo);

	if (listen(sockfd, listen_timeout) == -1)
	{
		close(sockfd);
		throw InitFailed(std::string("Error: listen: ") + strerror(errno));
	}

	pollfd server_poll_fd;
	server_poll_fd.fd = sockfd;
	server_poll_fd.events = POLLIN;
	pollfds.push_back(server_poll_fd);
}

Server::~Server()
{
	if (close(sockfd) == -1)
	{
		std::cerr << "Error: close: " << strerror(errno) << std::endl;
		exit(1);
	}
}

void Server::poll_client_events()
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
				register_new_user();
			else
				handle_client_msg(&pollfd_iter->fd);
		}
		// TODO(Hans): erase disconnected users from pollfds vector
	}
	std::cerr << "Error: poll: " << strerror(errno) << std::endl;
}

void Server::register_new_user()
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

void Server::handle_client_msg(int *client_fd)
{
	std::cout << "Client sent something!" << std::endl;
	// TODO(Hans): Find way to detect/handle messages longer than msg_buf_size
	int bytes_read = recv(*client_fd, msg_buf, sizeof(msg_buf) - 1, 0);
	if (bytes_read == -1)
		throw RecvFailed(std::string("Error: recv: ") + strerror(errno));
	if (bytes_read == 0)
	{
		std::cout << "Client closed connection!" << std::endl;
		close(*client_fd);
		*client_fd = -1;
		return;
	}
	msg_buf[bytes_read] = '\0';
	std::cout << msg_buf << std::endl;
	emit(msg_buf, *client_fd);
}

void	Server::emit(unsigned char *msg, int sender)
{
	int	len = strlen((const char *)msg);
	for (std::vector<pollfd>::iterator it = this->pollfds.begin();
		it != this->pollfds.end(); ++it)
	{
		int dest_fd = (*it).fd;
		if (dest_fd != sockfd && dest_fd != sender)
			if (sendAllData(dest_fd, msg, &len) == -1)
			{
				std::cerr << "Error: send " << strerror(errno) << std::endl;
				exit(1);
			}
	}
}

int	Server::sendAllData(int socket, unsigned char *msg, int *len)
{
	int	total = 0;
	int	bytes_left = *len;
	int	n;

	while (total < *len)
	{
		n = send(socket, msg, bytes_left, 0);
		if (n == -1)
			break;
		total += n;
		bytes_left -= n;
	}

	*len = total;
	if (n == -1)
		return (-1);
	return (0);
}
