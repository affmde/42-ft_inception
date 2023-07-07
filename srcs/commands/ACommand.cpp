/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:28:11 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 17:06:40 by andrferr         ###   ########.fr       */
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

std::vector<std::string> ACommand::split(std::string str, std::string del)
{
	std::vector<std::string> vec;
	size_t pos;
	std::string tmp;
	while ((pos = str.find(del)) != std::string::npos)
	{
		tmp = str.substr(0, pos);
		vec.push_back(tmp);
		str.erase(0, pos + 1);
	}
	if (!str.empty())
		vec.push_back(str);
	return (vec);
}

