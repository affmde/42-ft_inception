/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:27:34 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/05 10:54:25 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
: clientFD(0)
, totalMessages(0)
, connected(false)
, activeStatus(NOT_CONNECTED)
{}

Client::Client(int fd)
: clientFD(fd)
, totalMessages(0)
, connected(true)
, activeStatus(CONNECTED)
{}

Client::Client(const Client &other) { *this = other; }

Client::~Client() {}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		clientFD = other.clientFD;
		totalMessages = other.totalMessages;
		connected = other.connected;
		realname = other.realname;
		username = other.username;
		nickname = other.nickname;
		servername = other.servername;
		hostname = other.hostname;
		buffer = other.buffer;
		activeStatus = other.activeStatus;
	}
	return (*this);
}

//Member Functions

int Client::getClientFD() const { return clientFD; }
void Client::setClientFD(int fd) { clientFD = fd; }

std::string Client::getRealname() const { return realname; }
void Client::setRealname(std::string realname) { this->realname = realname; }

std::string Client::getHostname() const { return hostname; }
void Client::setHostname(std::string hostname) { this->hostname = hostname; }

std::string Client::getServername() const { return servername; }
void Client::setServername(std::string servername) { this->servername = servername; }

std::string Client::getNickname() const { return nickname; }
void Client::setNickname(std::string nickname) { this->nickname = nickname; }

std::string Client::getUsername() const { return username; }
void Client::setUsername(std::string username) { this->username = username; }

bool Client::isConnected() const { return connected; }
void Client::setConnected(bool connected) { this->connected = connected; }

int Client::getTotalMessages() const { return totalMessages; }
void Client::increaseTotalMessages() { totalMessages++; }

std::string Client::getBuffer() const { return buffer; }
void Client::setBuffer(std::string str) { buffer = str; }
void Client::resetBuffer() { buffer = ""; }

int Client::getActiveStatus() const { return activeStatus; }
void Client::setActiveStatus(int status) { activeStatus = status; }

bool Client::isReadyToSend() const
{
	size_t pos;
	if ((pos = buffer.find("\n")) != std::string::npos) //STILL CHECK THIS TO DECIDE ABOUT ACCEPT \n or \r\n!!!!!!
		return true;
	else
		return false;
}

void Client::addChannelInvite(std::string channelName) { channelInviteList.push_back(channelName); }

bool Client::isInvited(std::string channelName)
{
	for(std::vector<std::string>::iterator it = channelInviteList.begin(); it != channelInviteList.end(); ++it)
	{
		if (*it == channelName)
			return true;
	}
	return false;
}

std::vector<std::string>::iterator Client::removeInvite(std::string channelName)
{
	for(std::vector<std::string>::iterator it = channelInviteList.begin(); it != channelInviteList.end(); ++it)
	{
		if (*it == channelName)
			return channelInviteList.erase(it);
	}
	return channelInviteList.end();
}

std::vector<std::string> Client::getChannelInvites() const { return channelInviteList; }