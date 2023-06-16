/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 12:42:53 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/16 13:04:15 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void){}
Client::Client(const Client &other){*this = other;}
Client	&Client::operator=(const Client &other)
{
	this->clientFd = other.clientFd;
	return (*this);
}

Client::~Client(void){}

//Member Functions

int	Client::getClientFd(void)
{
	return (this->clientFd);
}
