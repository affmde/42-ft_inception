#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "Server.hpp"
#include "Message.hpp"
#include "Parser.hpp"

Server::Server(const char *port, std::string pass)
{
	this->pass = pass;
	addrinfo hints;
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	addrinfo *servinfo;
	int status = getaddrinfo(NULL, port, &hints, &servinfo);
	if (status != 0)
		throw InitException(std::string("Error: getaddrinfo: ") + gai_strerror(status));
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (sockfd == -1)
		throw InitException(std::string("Error: socket: ") + strerror(errno));

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw InitException(std::string("Error: fcntl: ") + strerror(errno));

	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw InitException(std::string("Error: setsockopt: ") + strerror(errno));

	if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
		throw InitException(std::string("Error: bind: ") + strerror(errno));

	freeaddrinfo(servinfo);

	if (listen(sockfd, listenTimeout) == -1)
		throw InitException(std::string("Error: listen: ") + strerror(errno));

	pollfd server_poll_fd;
	server_poll_fd.fd = sockfd;
	server_poll_fd.events = POLLIN;
	pollfds.push_back(server_poll_fd);
}


Server::~Server() { close(sockfd); }

void Server::pollClientEvents()
{
	int event_count = 0;
	while ((event_count = poll(pollfds.data(), pollfds.size(), -1)) != -1)
	{
		for (std::vector<pollfd>::iterator it = pollfds.begin();
			it != pollfds.end() && event_count > 0; it++)
		{
			if (it->revents == 0)
				continue;
			event_count--;
			if (it->fd == sockfd)
				registerNewUser();
			else
				handleClientMessage(*findClientByFD(it->fd));
		}
		eraseDisconnectedUsers();
	}
	std::cerr << "Error: poll: " << strerror(errno) << std::endl;
}

void Server::registerNewUser()
{
	std::cout << "New connection received!" << std::endl;
	sockaddr_storage client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	int client_fd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_size);
	if (client_fd == -1)
		throw UserRegistrationException(std::string("Error: accept: ") + strerror(errno));
	pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	pollfds.push_back(client_pollfd);
	clients.push_back(Client(client_fd));
}

void Server::eraseDisconnectedUsers()
{
	std::vector<Client>::iterator it = clients.begin();
	while (it != clients.end())
	{
		if (it->isConnected() == false)
			it = eraseUserByFD(it->getClientFD());
		else
			it++;
	}
}

void Server::handleClientMessage(Client &client)
{
	int bytes_read = recv(client.getClientFD(), buffer, sizeof(buffer) - 1, 0);
	if (bytes_read == -1)
		throw RecvException(std::string("Error: recv: ") + strerror(errno));
	if (bytes_read == 0)
	{
		std::cout << "Client closed connection!" << std::endl;
		client.setConnected(false);
		return;
	}
	buffer[bytes_read] = '\0';
	std::cout << "buffer: \n" << buffer << "\n-----------------" << std::endl;
	Parser parser;
	parser.setInput(std::string(buffer));
	std::vector<std::string>	args = parser.parseInput();
	int i = 0;
	for(std::vector<std::string>::iterator it = args.begin();
		it != args.end(); ++it, i++)
	{
		if (it->find("PASS ") != std::string::npos && client.isConnected() && client.getActiveStatus() == CONNECTED)
		{
			try{
				parser.parsePass(*it, pass);
				client.setActiveStatus(PASS_ACCEPTED);
			} catch (Parser::NoPassException &e){
				Message msg;
				msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, "*", "PASS");
				client.setConnected(false);
				std::cerr << e.what() << std::endl;
			} catch (Parser::WrongPassException &e){
				Message msg;
				msg.reply(NULL, client, ERR_PASSWDMISMATCH_CODE, SERVER, ERR_PASSWDMISMATCH, "*");
				client.setConnected(false);
				std::cerr << e.what() << std::endl;
				break ;
			}
		}
		else if (it->find("NICK ") != std::string::npos && client.isConnected() && client.getActiveStatus() == PASS_ACCEPTED)
		{
			std::string nick;
			try {
				parser.parseNick(*it, nick);
				if (nick.empty())
				{
					Message msg;
					msg.reply(NULL, client, ERR_NONICKNAMEGIVEN_CODE, SERVER, ERR_NONICKNAMEGIVEN);
					break ;
				}
				checkDuplicateNick(nick);
				client.setNickname(nick);
				if (!client.getUsername().empty())
					client.setActiveStatus(REGISTERED);
			} catch (Parser::NoNickException &e){
				Message msg(e.what());
				msg.sendData(client.getClientFD());
			} catch (DuplicateNickException &e){
				Message msg;
				msg.reply(NULL, client, ERR_NICKNAMEINUSE_CODE, SERVER, ERR_NICKNAMEINUSE, nick.c_str());
				break;
			} catch (Parser::InvalidNickException &e){
				Message msg;
				msg.reply(NULL, client, ERR_ERRONEUSNICKNAME_CODE, SERVER, ERR_ERRONEUSNICKNAME, "*", nick.c_str());
				break;
			}
		}
		else if (it->find("USER ") != std::string::npos && client.isConnected() && client.getActiveStatus() == PASS_ACCEPTED)
		{
			try {
				Parser parser;
				parser.parseUser(*it, client);
				if (!client.getNickname().empty())
					client.setActiveStatus(REGISTERED);
			} catch (Parser::EmptyUserException &e) {
				Message msg;
				msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, "*", "USER");
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			client.setBuffer( client.getBuffer() + *it);
			//CHANGE THIS TO SEND THE INPUT TO THE COMMAND ANALYZER IN THE FUTURE!!
			//ALSO MAKE SURE THAT ONLY IF THE ACTIVE STATATUS == REGISTERED WE PROCCEED IT!!!
		}
		if (client.getActiveStatus() == REGISTERED)
		{
			Message msg;
			std::cout << client.getNickname() << " registered successfuly." << std::endl;
			msg.reply(NULL, client, RPL_WELCOME_CODE, SERVER, RPL_WELCOME, client.getNickname().c_str(), client.getNickname().c_str());
			msg.reply(NULL, client, RPL_YOURHOST_CODE, SERVER, RPL_YOURHOST, client.getNickname().c_str());
			std::string date = creationTime.getDateAsString();
			msg.reply(NULL, client, RPL_CREATED_CODE, SERVER, RPL_CREATED, client.getNickname().c_str(), date.c_str());
			msg.reply(NULL, client, RPL_ISUPPORT_CODE, SERVER, RPL_ISUPPORT, "CASEMAPPING=ascii", client.getNickname().c_str());
		}
		if (client.isReadyToSend())
		{
			std::cout << "Client sent something!" << std::endl;
			std::cout << client.getBuffer() << std::endl;
			emit(client.getClientFD(), client.getBuffer());
			client.increaseTotalMessages();
			client.resetBuffer();
			std::cout << "User " << client.getClientFD() << " has sent " << client.getTotalMessages() << " messages" << std::endl;
		}
	}
}

void Server::emit(int client_fd, std::string msg)
{
	Message message(msg);
	for (std::vector<Client>::iterator it = clients.begin();
		it != clients.end(); ++it)
	{
		if (it->getClientFD() != sockfd && it->getClientFD() != client_fd)
			message.sendData(it->getClientFD());
	}
}

std::vector<pollfd>::iterator Server::findPollfdByFD(int fd)
{
	for (std::vector<pollfd>::iterator it = pollfds.begin();
		it != pollfds.end(); ++it)
		if (it->fd == fd) return it;
	return pollfds.end();
}

std::vector<Client>::iterator Server::findClientByFD(int fd)
{
	for (std::vector<Client>::iterator it = clients.begin();
		it != clients.end(); ++it)
		if (it->getClientFD() == fd) return it;
	return clients.end();
}

std::vector<Client>::iterator Server::eraseUserByFD(int fd)
{
	close(fd);

	std::vector<pollfd>::iterator poll_fd = findPollfdByFD(fd);
	if (poll_fd == pollfds.end()) return clients.end();
	pollfds.erase(poll_fd);

	std::vector<Client>::iterator client = findClientByFD(fd);
	if (client == clients.end()) return clients.end();
	return clients.erase(client);
}


void Server::checkDuplicateNick(std::string nick)
{
	std::string clientNick = nick;
	for (int i = 0; i < clientNick.length(); i++)
		clientNick[i] = std::tolower(clientNick[i]);
	std::string nickToCompare;
	for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		nickToCompare = it->getNickname();
		for (int i = 0; i < nickToCompare.length(); i++)
			nickToCompare[i] = std::tolower(nickToCompare[i]);
		if (nickToCompare == clientNick)
			throw DuplicateNickException("Duplicate nick");
	}
}
