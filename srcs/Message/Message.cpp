/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:57:50 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/17 09:09:08 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(std::string msg)
{
	this->message = msg;
}
Message::Message(const Message & other){*this = other;}
Message	&Message::operator=(const Message &other)
{
	this->message = other.message;
	return (*this);
}
Message::~Message(void){}


//Member Functions
void	Message::sendData(int clientFd)
{
	int	bytes_left = this->message.length();
	while (bytes_left > 0)
	{
		int bytes_sent = send(clientFd, this->message.c_str(), bytes_left, 0);
		if (bytes_sent == -1)
			throw SendFailed(std::string("Error: send: ") + strerror(errno));
		bytes_left -= bytes_sent;
	}
}