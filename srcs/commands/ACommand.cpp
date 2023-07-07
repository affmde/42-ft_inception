/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:28:11 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 16:15:41 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

ACommand::ACommand(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
server(server),
client(client),
input(input),
clientsList(clientsList)
{}

ACommand::ACommand(const ACommand &other) :
server(other.server),
client(other.client),
input(other.input),
clientsList(other.clientsList)
{}

ACommand::~ACommand(){}

ACommand &ACommand::operator=(const ACommand &other)
{
	if (this == &other) return (*this);
	server = other.server;
	client = other.client;
	input = other.input;
	clientsList = other.clientsList;
	return (*this);
}

