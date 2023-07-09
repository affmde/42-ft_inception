/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:21:26 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/08 22:30:53 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "QUIT.hpp"
#include "../Message.hpp"

Quit::Quit(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Quit::Quit(const Quit &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Quit::~Quit(){}
Quit &Quit::operator=(const Quit &other) { return *this; }

void Quit::exec()
{
	if (input.empty()) return;
	if (input[0] == ':')
		input.erase(0, 1);
	Message msg;
	msg.reply(NULL, client, "0", SERVER, "ERROR: %s", input.c_str());
	for(std::vector<Channel*>::iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it)
	{
		if ((*it)->isClientInChannel(client.getNickname()))
		{
			(*it)->messageAllOthers(&client,"QUIT :Quit: %s", input.c_str());
			client.removeChannel((*it)->getName());
		}
	}
	//TODO: HANDLE QUIT ON DISCONNECTION WITHOUT QUIT COMMAND (EX: CNTL_C). MAYBE SHOULD USE PING FOR THIS?
	client.setConnected(false);
	client.setActiveStatus(NOT_CONNECTED);
	server.logMessage(1, "disconnected (" + input + ")", client.getNickname());
}
