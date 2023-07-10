/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:48:22 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/10 15:34:51 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PING.hpp"

Ping::Ping(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Ping::Ping(const Ping &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Ping::~Ping(){}
Ping &Ping::operator=(const Ping &other) { return *this; }

void Ping::exec()
{
	if (input.empty())
		return ;
	msg.reply(NULL, client, "0", SERVER, "PONG :%s %s", "IRCSERVER", input.c_str());
	server.logMessage(1, "PONG: " + input, "");
}
