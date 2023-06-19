/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:27:34 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/19 16:16:45 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
: clientFD(0)
, totalMessages(0)
, connected(false)
, logged(false)
, banned(false)
{}

Client::Client(int fd)
: clientFD(fd)
, totalMessages(0)
, connected(true)
, logged(false)
, banned(false)
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
		nickname = other.nickname;
		servername = other.servername;
		hostname = other.hostname;
		buffer = other.buffer;
		logged = other.logged;
		banned = other.banned;
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

bool Client::isConnected() const { return connected; }
void Client::setConnected(bool connected) { this->connected = connected; }

int Client::getTotalMessages() const { return totalMessages; }
void Client::increaseTotalMessages() { totalMessages++; }

std::string Client::getBuffer() const { return buffer; }
void Client::setBuffer(std::string str) { buffer = str; }
void Client::resetBuffer() { buffer = ""; }

bool Client::isLogged() const { return logged; }
void Client::setLogged(bool logged) { this->logged = logged; }

bool Client::isBanned() const { return banned; }
void Client::setBanned(bool banned) { this->banned = banned; }

bool Client::isReadyToSend() const
{
	size_t pos;
	//if ((pos = buffer.find("\r\n")) != std::string::npos)
	//	std::cout << "\\r\\n Found. Ready to Send. Pos: " << pos << " Length: " << buffer.length() << std::endl;
	//else
	//	std::cout << "\\r\\n not found. Not ready to send" << std::endl;
	if ((pos = buffer.find("\n")) != std::string::npos)
		return true;
	else
		return false;
	//if ((pos = buffer.find("\r")) != std::string::npos)
	//	std::cout << "\\r Found. Ready to Send. Pos: " << pos << " Length: " << buffer.length() << std::endl;
	//else
	//	std::cout << "\\r not found. Not ready to send" << std::endl;
}
