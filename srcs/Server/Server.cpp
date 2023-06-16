#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <algorithm>
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
	for(std::vector<Client*>::iterator it = this->clientsList.begin();
		it != this->clientsList.end(); ++it)
		delete *it;
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
			Client *client = findClientByFD(pollfd_iter->fd);
			if (pollfd_iter->revents == 0)
				continue;
			event_count--;
			if (!client)
				registerNewUser();
			else
				handleClientMessage(*client);
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
	Client *newClient = new Client;
	newClient->setClientFd(client_fd);
	newClient->setConnected(true);
	clientsList.push_back(newClient);
}

void Server::handleClientMessage(Client &c)
{
	std::cout << "Client sent something!" << std::endl;
	int bytes_read = recv(c.getClientFd(), buffer, sizeof(buffer) - 1, 0);
	if (bytes_read == -1)
		throw RecvFailed(std::string("Error: recv: ") + strerror(errno));
	if (bytes_read == 0)
	{
		std::cout << "Client closed connection!" << std::endl;
		removeClientByFD(c.getClientFd());
		return;
	}
	buffer[bytes_read] = '\0';
	std::cout << "message from : " << c.getClientFd() << std::endl;
	std::cout << buffer << std::endl;
	emit(c.getClientFd(), buffer);
	c.increaseTotalMessages();
	std::cout << "User " << c.getClientFd() << " has sent " << c.getTotalMessages() << " messages" << std::endl;
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
	for (std::vector<Client *>::iterator it = this->clientsList.begin();
		it != this->clientsList.end(); ++it)
	{
		if ((*it)->getClientFd() != sockfd && (*it)->getClientFd() != client_fd)
			sendAllData((*it)->getClientFd(), msg);
	}
}

Client	*Server::findClientByFD(int fd)
{
	for (std::vector<Client *>::iterator it = this->clientsList.begin();
		it != this->clientsList.end(); ++it)
	{
		if ((*it)->getClientFd() == fd)
			return (*it);
	}
	return(NULL);
}

pollfd	*Server::findPollEventByFD(int fd)
{
	for (std::vector<pollfd>::iterator it = this->pollfds.begin();
		it != this->pollfds.end(); ++it)
	{
		if (it->fd == fd)
			return (&(*it));
	}
	return (NULL);
}

void	Server::removeClientByFD(int fd)
{
	for (std::vector<Client *>::iterator it = clientsList.begin(); it != clientsList.end(); ++it)
		if (fd == (*it)->getClientFd())
		{
			this->clientsList.erase(it);
			close((*it)->getClientFd());
			int	*fd = &findPollEventByFD((*it)->getClientFd())->fd;
			*fd = -1;
			delete findClientByFD((*it)->getClientFd());
			return ;
		}
}
