/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:02:34 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/09 08:53:53 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <iostream> //DELETE!!!!!

#include "KICK.hpp"
#include "../rpl_isupport.hpp"
#include "../Message.hpp"

Kick::Kick(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Kick::Kick(const Kick &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Kick::~Kick(){}
Kick &Kick::operator=(const Kick &other) { return *this; }

void Kick::exec()
{
	if (input.empty())
	{
		Message msg;
		msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, client.getNickname().c_str(), "KICK");
		throw NeedMoreParamsException ("KICK: Need more params");
	}
	size_t pos = input.find(" ");
	std::string channelName = input.substr(0, pos);
	input.erase(0, pos + 1);
	pos = input.find(" ");
	std::string usersString = input.substr(0, pos);
	input.erase(0 , pos + 1);
	std::vector<std::string> users = split(usersString, ",");
	if (input[0] == ':')
		input.erase(0, 1);
	//std::vector<std::string> comments = split(input, ",");
	std::string comment = input;
	Channel *c = server.searchChannel(channelName);
	if (!c)
	{
		Message msg;
		msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), channelName.c_str());
		throw NoSuchChannelException("No such channel " + channelName);
	}
	if (!c->isOper(client.getNickname()))
	{
		Message msg;
		msg.reply(NULL, client, ERR_CHANOPRIVSNEEDED_CODE, SERVER, ERR_CHANOPRIVSNEEDED, client.getNickname().c_str(), channelName.c_str());
		throw NoPrivilegesException("No privileges: Not oper.");
	}
	if (!c->isClientInChannel(client.getNickname()))
	{
		Message msg;
		msg.reply(NULL, client, ERR_NOTONCHANNEL_CODE, SERVER, ERR_NOTONCHANNEL, client.getNickname().c_str(), channelName.c_str());
		throw NotOnChannelException("Not on channel");
	}
	std::map<std::string, std::string> usersToKick;

	for (int i = 0; i < users.size(); i++)
	{
		if (!comment.empty())
		{
			if (comment.size() > KICKLEN)
				comment = comment.substr(0, KICKLEN);
			usersToKick.insert(std::pair<std::string, std::string>(users[i], comment));
		}
		else
			usersToKick.insert(std::pair<std::string, std::string>(users[i], "You were kicked from the channel " + channelName));
	}
	for(std::map<std::string, std::string>::iterator it = usersToKick.begin(); it != usersToKick.end(); ++it)
	{
		if (!c->isClientInChannel(it->first))
		{
			Message msg;
			msg.reply(NULL, client, ERR_USERNOTINCHANNEL_CODE, SERVER, ERR_USERNOTINCHANNEL, client.getNickname().c_str(), client.getNickname().c_str(), channelName.c_str());
			server.logMessage(2, "User not in channel", client.getNickname());
			continue;
		}
		Client *kickUser = *c->findClientByNick(it->first);
		Message msg;
		msg.reply(&client, *kickUser, "0", CLIENT, "KICK %s %s :%s", c->getName().c_str(), it->first.c_str(), it->second.c_str());
		c->messageAll(&client, "KICK %s %s :%s", c->getName().c_str(), it->first.c_str(), it->second.c_str());
		c->eraseClient(it->first, it->second, 1);
		client.removeChannel(c->getName());
	}
}
