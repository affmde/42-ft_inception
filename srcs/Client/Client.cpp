/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:27:34 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/16 15:34:00 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


Client::Client(void){}
Client::Client(const Client &other){*this = other;}
Client	&Client::operator=(const Client &other)
{
	this->clientFd = other.clientFd;
	this->totalMessages = other.totalMessages;
	return (*this);
}
Client::~Client(void){}


//Member Functions

int	Client::getClientFd(void)
{
	return (this->clientFd);
}

int	Client::getClientTotalMessages(void)
{
	return (this->totalMessages);
}
