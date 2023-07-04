/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 19:27:10 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/04 12:48:02 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> //JUST NEEDED FOR DEBUG! DELETE THIS!!!
#include <stdarg.h>

#include "Channel.hpp"
#include "Message.hpp"
#include "Utils.hpp"

Channel::Channel(Server &server) :server(server)
{
	modes.invite = false;
	modes.limit = 2147483647;
	modes.topic = true;
	
}
Channel::Channel(const Channel & other) : server(other.server) { *this = other; }
Channel::~Channel() {}
Channel &Channel::operator=(const Channel &other)
{
	if (this == &other) return *this;
	pass = other.pass;
	name = other.name;
	topic = other.topic;
	server = other.server;
	modes.invite = other.modes.invite;
	modes.limit = other.modes.limit;
	modes.topic = other.modes.topic;
	return (*this);
}

std::string Channel::getName() const { return name; }
void Channel::setName(std::string name) { this->name = name; }

std::string Channel::getTopic() const { return topic; }
void Channel::setTopic(std::string topic) { this->topic = topic; }

std::string Channel::getPass() const { return pass; }
void Channel::setPass(std::string pass) { this->pass = pass; }

std::vector<Client*> Channel::getClients() const { return clients; }
std::vector<Client*> Channel::getOperators() const { return operators; }

bool Channel::getModesInvite() const { return modes.invite; }
void Channel::setModesInvite(bool invite) { modes.invite = invite; }

bool Channel::getModesTopic() const { return modes.topic; }
void Channel::setModesTopic(bool topic) { modes.topic = topic; }

int Channel::getModesLimit() const { return modes.limit; }
void Channel:: setModesLimit(int limit) { modes.limit = limit; }

bool Channel::getModesLimitRequired() const { return modes.limitRequired; }
void Channel::setModesLimitRequired(bool req) { modes.limitRequired = req; }

bool Channel::getModesPassRequired() const { return modes.passRequired; }
void Channel::setModesPassRequired(bool req) { modes.passRequired = req; }

std::string Channel::getCreationTimestampAsString() const { return toString(creationTime.getTimestamp()); }

void Channel::addUser(Client *client)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (client == clients[i])
			throw AlreadyUserException("Already USer");
	}
	clients.push_back(client);
}

std::string Channel::getListClientsNicknames()
{
	std::string list;
	for (int i = 0; i < clients.size(); i++)
	{
		std::string nick = clients[i]->getNickname();
		if (isOper(nick))
			list += "@";
		list += clients[i]->getNickname() + " ";
	}
	return (list);
}

std::vector<Client*>::iterator Channel::findClientByNick(std::string nick)
{
	for(std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == nick)
			return it;
	}
	return (clients.end());
}

void Channel::eraseClient(std::string nick, std::string reason, int code)
{
	std::vector<Client*>::iterator it = findClientByNick(nick);
	if (it == clients.end()) return;
	if (code == 0)
	{
		messageAll(*it, "%s %s :%s", "PART", getName().c_str(), reason.c_str());
		server.logMessage(1, "left channel " + getName(), nick);
	}
	else if (code == 1)
	{
		Message msg;
		msg.reply(NULL, **it, "0", SERVER, "KICK %s %s :%s", getName().c_str(), nick.c_str(), reason.c_str());
		messageAllOthers(*it, "KICK %s %s :%s", getName().c_str(), nick.c_str(), reason.c_str());
		server.logMessage(1, "KICK from channel " + getName(), nick);
	}
	clients.erase(it);
}

void Channel::addOper(Client *client)
{
	for(std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it)
	{
		if ((*it)->getNickname() == client->getNickname())
			return;
	}
	operators.push_back(client);
}

void Channel::messageAll(Client *sender, std::string format, ...)
{
	va_list args;
	va_start(args, format);
	while (format.find("%s") != std::string::npos)
		format.replace(format.find("%s"), 2, va_arg(args, char*));
	va_end(args);
	std::cout << "Format: " << format << std::endl;
	Message msg;
	for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		//INFORM EVERY SINGLE CLIENT!!!
		if ((*it)->getActiveStatus() == LOGGED)
			msg.reply(sender, **it, "0", CLIENT, format);
	}
}

void Channel::messageAllOthers(Client * client, std::string format, ...)
{
	va_list args;
	va_start(args, format);
	while (format.find("%s") != std::string::npos)
		format.replace(format.find("%s"), 2, va_arg(args, char*));
	va_end(args);
	Message msg;
	for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		//INFORM EVERY SINGLE CLIENT EXCEPT MYSELF!!!
		if ((*it)->getNickname() != client->getNickname() && (*it)->getActiveStatus() == LOGGED)
			msg.reply(client, **it, "0", CLIENT, format);
	}
}

void Channel::messageAllFromServer(std::string code, std::string format, ...)
{
	va_list args;
	va_start(args, format);
	while (format.find("%s") != std::string::npos)
		format.replace(format.find("%s"), 2, va_arg(args, char*));
	va_end(args);
	Message msg;
	for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getActiveStatus() == LOGGED)
			msg.reply(NULL, **it, code, CLIENT, format);
	}
}

bool Channel::isOper(std::string nick)
{
	for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it)
	{
		if ((*it)->getNickname() == nick)
			return (true);
	}
	return (false);
}

bool Channel::isClientInChannel(std::string nick)
{
	for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == nick)
			return (true);
	}
	return (false);
}

int Channel::totalClients() const
{
	return clients.size();
}

bool Channel::isClientBanned(std::string nick)
{
	for(std::vector<Client*>::iterator it = bannedList.begin(); it != bannedList.end(); ++it)
	{
		if ((*it)->getNickname() == nick)
			return (true);
	}
	return (false);
}

std::string Channel::getChannelModes() const
{
	std::string ret;
	std::string modeStr;
	std::string argsStr;
	if (modes.invite)
		modeStr += "i";
	if (modes.topic)
		modeStr += "t";
	if (modes.limitRequired)
	{
		modeStr += "l";
		argsStr += toString(modes.limit);
	}
	
	ret = modeStr + " " + argsStr;
	std::cout << "ret: " << ret << std::endl;
	return ret;
}