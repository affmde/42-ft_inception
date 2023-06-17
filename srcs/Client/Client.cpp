/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:27:34 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/17 07:51:55 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


Client::Client(void)
{
	this->clientFd = 0;
	this->username = "";
	this->nickname = "";
	this->totalMessages = 0;
	this->buffer = "";
	this->connected = false;
}
Client::Client(const Client &other){*this = other;}
Client	&Client::operator=(const Client &other)
{
	this->clientFd = other.clientFd;
	this->totalMessages = other.totalMessages;
	this->connected = other.connected;
	this->username = other.username;
	this->nickname = other.username;
	this->buffer = other.buffer;
	return (*this);
}
Client::~Client(void){}


//Member Functions

int	Client::getClientFd(void)
{
	return (this->clientFd);
}

int	Client::getTotalMessages(void)
{
	return (this->totalMessages);
}

std::string	Client::getUsername(void)
{
	return (this->username);
}

std::string	Client::getNickname(void)
{
	return (this->nickname);
}

bool		Client::isConnected(void)
{
	return (this->connected);
}

void		Client::setUsername(std::string username)
{
	this->username = username;
}

void		Client::setNickname(std::string nickname)
{
	this->nickname = nickname;
} 

void		Client::setConnected(bool input)
{
	this->connected = input;
}

void		Client::setClientFd(int fd)
{
	this->clientFd = fd;
}

void		Client::increaseTotalMessages(void)
{
	this->totalMessages++;
}

std::string	Client::getBuffer(void)
{
	return (this->buffer);
}

void		Client::setBuffer(std::string str)
{
	this->buffer += str;
}