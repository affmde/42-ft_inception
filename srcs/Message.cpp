/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:57:50 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/19 17:30:23 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

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
void Message::sendData(int clientFD)
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

std::string Message::getMessage(void) { return message; }
