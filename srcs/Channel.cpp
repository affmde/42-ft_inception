/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 19:27:10 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/29 13:43:16 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> //JUST NEEDED FOR DEBUG! DELETE THIS!!!

#include "Channel.hpp"
#include "Message.hpp"

Channel::Channel() {}
Channel::Channel(const Channel & other) { *this = other; }
Channel::~Channel() {}
Channel &Channel::operator=(const Channel &other)
{
	if (this == &other) return *this;
	pass = other.pass;
	name = other.name;
	topic = other.topic;
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
	messageAll(*it, "PART");
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

void Channel::messageAll(Client *sender, std::string message)
{
	Message msg;
	for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		//INFORM AEVERY SINGLE CLIENT!!!
		msg.reply(sender, **it, "0", CLIENT, "%s %s", message.c_str(), getName().c_str());
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