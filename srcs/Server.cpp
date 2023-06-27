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
#include "Command.hpp"
#include "Utils.hpp"

Server::Server(const char *port, std::string pass)
{
	logMessage(1, "Server started", "");
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
	logMessage(2, "Error poll: " + std::string(strerror(errno)), "");
}

void Server::registerNewUser()
{
	logMessage(1, "New connection received!", "");
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
		logMessage(1, "Client closed connection!", client.getNickname());
		client.setConnected(false);
		return;
	}
	buffer[bytes_read] = '\0';
	std::cout << "buffer: "<< buffer << std::endl;
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
				logMessage(2, e.what(), client.getNickname());
			} catch (Parser::WrongPassException &e){
				Message msg;
				msg.reply(NULL, client, ERR_PASSWDMISMATCH_CODE, SERVER, ERR_PASSWDMISMATCH, "*");
				client.setConnected(false);
				logMessage(2, e.what(), client.getNickname());
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
					logMessage(2, "Emptu nickname", client.getNickname());
					break ;
				}
				checkDuplicateNick(nick);
				client.setNickname(nick);
				if (!client.getUsername().empty())
					client.setActiveStatus(REGISTERED);
			} catch (Parser::NoNickException &e){
				Message msg;
				msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, "*", "NICK");
				logMessage(2, e.what(), client.getNickname());
			} catch (DuplicateNickException &e){
				Message msg;
				msg.reply(NULL, client, ERR_NICKNAMEINUSE_CODE, SERVER, ERR_NICKNAMEINUSE, client.getNickname().c_str(), nick.c_str());
				logMessage(2, e.what(), client.getNickname());
				break;
			} catch (Parser::InvalidNickException &e){
				Message msg;
				msg.reply(NULL, client, ERR_ERRONEUSNICKNAME_CODE, SERVER, ERR_ERRONEUSNICKNAME, "*", nick.c_str());
				logMessage(2, e.what(), client.getNickname());
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
				logMessage(2, e.what(), client.getNickname());
			}
		}
		else
		{
			//client.setBuffer( client.getBuffer() + *it);
			try{
				Command cmd(*it, client, this);
				cmd.checkCommands(clients);
			} catch(Command::AlreadyRegisteredException &e) {
				Message msg;
				msg.reply(NULL, client, ERR_ALREADYREGISTERED_CODE, SERVER, ERR_ALREADYREGISTERED, client.getNickname().c_str());
				logMessage(2, e.what(), client.getNickname());
			}
		}
		if (client.getActiveStatus() == REGISTERED)
		{
			Message msg;
			logMessage(1, "Registered successfuly", client.getNickname());
			msg.reply(NULL, client, RPL_WELCOME_CODE, SERVER, RPL_WELCOME, client.getNickname().c_str(), client.getNickname().c_str());
			msg.reply(NULL, client, RPL_YOURHOST_CODE, SERVER, RPL_YOURHOST, client.getNickname().c_str());
			std::string date = creationTime.getDateAsString();
			msg.reply(NULL, client, RPL_CREATED_CODE, SERVER, RPL_CREATED, client.getNickname().c_str(), date.c_str());
			msg.reply(NULL, client, RPL_MYINFO_CODE, SERVER, RPL_MYINFO, client.getNickname().c_str(), "IRCSERVER", "1.0.0");
			std::string supportedFeactures = "CHARSET=ascii CASEMAPPING=ascii NICKLEN=10 CHANNELLEN=50 TOPICLEN=390";
			msg.reply(NULL, client, RPL_ISUPPORT_CODE, SERVER, RPL_ISUPPORT, client.getNickname().c_str(), supportedFeactures.c_str());
			client.setActiveStatus(LOGGED);
		}
		else if (client.isReadyToSend() && client.getActiveStatus() == LOGGED)
		{
			logMessage(1, "Client sent something", client.getUsername());
			emit(client.getClientFD(), client.getBuffer());
			client.increaseTotalMessages();
			client.resetBuffer();
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

void Server::addChannel(Channel channel, Client &client)
{
	channels.push_back(channel);
	std::string message = channel.getName() + " was created. Now we have total of " + toString(channels.size()) + " channels active";
	logMessage(1, message, client.getNickname());
}

Channel *Server::searchChannel(std::string name)
{
	for(std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getName() == name)
			return (&(*it));
	}
	return (NULL);
}

Channel *Server::createChannel(std::string name, std::string topic, std::string pass, Client &client)
{
	Channel newChannel;
	newChannel.setName(name);
	newChannel.setPass(pass);
	newChannel.setTopic(topic);
	addChannel(newChannel, client);
	return (searchChannel(name));
}

void Server::logMessage(int fd, std::string msg, std::string nickname) const
{
	Time time;
	if (fd == 2)
		std::cerr << time.getDateAsString() + " " + nickname + ": " + msg << std::endl;
	else if (fd == 1)
		std::cout << time.getDateAsString() + " " + nickname + ": " + msg << std::endl;
}