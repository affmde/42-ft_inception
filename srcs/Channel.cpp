/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 19:27:10 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/29 18:18:02 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> //JUST NEEDED FOR DEBUG! DELETE THIS!!!
#include <stdarg.h>

#include "Channel.hpp"
#include "Message.hpp"

Channel::Channel(Server &server) :server(server) {}
Channel::Channel(const Channel & other) : server(other.server) { *this = other; }
Channel::~Channel() {}
Channel &Channel::operator=(const Channel &other)
{
	if (this == &other) return *this;
	pass = other.pass;
	name = other.name;
	topic = other.topic;
	server = other.server;
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

void Channel::eraseClient(std::string nick)
{
	std::vector<Client*>::iterator it = findClientByNick(nick);
	if (it == clients.end()) return;
	messageAll(*it, "%s %s", "PART", getName().c_str());
	server.logMessage(1, "left channel " + getName(), nick);
	clients.erase(it);
	if (clients.size() <= 0)
		server.removeChannel(getName());
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
	Message msg;
	for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		//INFORM EVERY SINGLE CLIENT!!!
		msg.reply(sender, **it, "0", CLIENT, format);
	}
}

void Channel::sendPRIVMSG(Client * client, std::string message)
{
	Message msg;
	for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		//INFORM EVERY SINGLE CLIENT EXCEPT MYSELF!!!
		if ((*it)->getNickname() != client->getNickname())
			msg.reply(client, **it, "0", CLIENT, "PRIVMSG " + getName() + " :" + "%s", message.c_str());
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