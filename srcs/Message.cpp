/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:57:50 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/20 20:33:51 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <sstream>
#include <iostream> //DELETE AFTER DUBBIGING IS OVER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "Message.hpp"

Message::Message(){}

Message::Message(std::string msg)
{
	message = msg;
}

Message::~Message() {}

Message::Message(const Message &other) { *this = other; }

Message &Message::operator=(const Message &other)
{
	if (this != &other)
		message = other.message;
	return (*this);
}

//Member Functions
void Message::sendData(int clientFD) const
{
	int bytes_left = message.length();
	while (bytes_left > 0)
	{
		int bytes_sent = send(clientFD, message.c_str(), bytes_left, 0);
		if (bytes_sent == -1)
			throw SendFailed(std::string("Error: send: ") + strerror(errno));
		bytes_left -= bytes_sent;
	}
}

void	Message::setMessage(std::string message)
{
	this->message = message;
}

std::string Message::getMessage(void) const { return message; }


void Message::RPL_Welcome (int client_fd, std::string nick) const
{
	Message msg;
	std::string message = nick + ": " + "Welcome to the Internet Relay Network, " + nick;
	msg.setMessage(message);
	msg.sendData(client_fd);
}

void Message::reply(Client *sender, Client &receiver, std::string code, int header, std::string format, ...)
{
	std::string message;
	std::string head;

	switch (header)
	{
		case SERVER:
		head = ":127.0.0.1 " + code + " ";
		break;
		case CLIENT:
		head = sender->getNickname() + "!" + sender->getNickname() + "@" + sender->getHostname() + " " + code + " " + receiver.getNickname() + " :";
	}
	va_list args;
	va_start(args, format);
	while (format.find("%s") != std::string::npos)
		format.replace(format.find("%s"), 2, va_arg(args, char*));
	va_end(args);
	message = head + format + "\r\n";
	setMessage(message);
	sendData(receiver.getClientFD());
}

