/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 19:27:10 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/28 17:43:14 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> //JUST NEEDED FOR DEBUG! DELETE THIS!!!

#include "Channel.hpp"

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

std::string Channel::getListClientsNicknames() const
{
	std::string list;
	for (int i = 0; i < clients.size(); i++)
	{
		std::cout << "client " << i << ": " << clients[i] << std::endl;
		if (i == 0)
			list += "@"; //Change this to check for OPER instead of being the first one!!!!!!!
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
	if (it != clients.end())
		clients.erase(it);
}

void Channel::messageAll(Client *send, std::string message)
{
	for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		//INFORM AEVERY SINGLE CLIENT!!!
	}
}