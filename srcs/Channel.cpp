/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 19:27:10 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/27 20:51:13 by andrferr         ###   ########.fr       */
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

void Channel::addUser(Client &client)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (client.getNickname() == clients[i]->getNickname())
			throw AlreadyUserException("Already USer");
		std::cout << "TEST" << std::endl;
	}
	clients.push_back(&client);
}

std::string Channel::getListClientsNicknames() const
{
	std::string list;
	std::cout << "HERE" << std::endl;
	std::cout << "size: " << clients.size() << std::endl;
	for (int i = 0; i < clients.size(); i++)
	{
		std::cout << clients[i] << std::endl;
		if (i == 0)
			list += "@"; //Change this to check for OPER instead of being the first one!!!!!!!
		list += clients[i]->getNickname() + " ";
	}
	return (list);
}
