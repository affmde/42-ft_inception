/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:02:34 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/10 17:01:20 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KICK.hpp"
#include "../rpl_isupport.hpp"

Kick::Kick(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Kick::Kick(const Kick &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Kick::~Kick(){}
Kick &Kick::operator=(const Kick &other)
{
	target = other.target;
	reason = other.reason;
	usersToKick.clear();
	usersToKick.insert(other.usersToKick.begin(), other.usersToKick.end());
	return *this;
}

void Kick::parseInput()
{
	size_t pos = input.find(" ");
	target = input.substr(0, pos);
	input.erase(0, pos + 1);
	pos = input.find(" ");
	std::string usersString = input.substr(0, pos);
	input.erase(0 , pos + 1);
	std::vector<std::string> users = split(usersString, ",");
	if (input[0] == ':')
		input.erase(0, 1);
	reason = input;
	for (int i = 0; i < users.size(); i++)
	{
		if (!reason.empty())
		{
			if (reason.size() > KICKLEN)
				reason = reason.substr(0, KICKLEN);
			usersToKick.insert(std::pair<std::string, std::string>(users[i], reason));
		}
		else
			usersToKick.insert(std::pair<std::string, std::string>(users[i], "You were kicked from the channel " + target));
	}
}

void Kick::exec()
{
	if (input.empty())
	{
		msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, client.getNickname().c_str(), "KICK");
		throw NeedMoreParamsException ("KICK: Need more params");
	}
	parseInput();
	Channel *c = server.searchChannel(target);
	if (!c)
	{
		msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), target.c_str());
		throw NoSuchChannelException("No such channel " + target);
	}
	if (!c->isOper(client.getNickname()))
	{
		msg.reply(NULL, client, ERR_CHANOPRIVSNEEDED_CODE, SERVER, ERR_CHANOPRIVSNEEDED, client.getNickname().c_str(), target.c_str());
		throw NoPrivilegesException("No privileges: Not oper.");
	}
	if (!c->isClientInChannel(client.getNickname()))
	{
		msg.reply(NULL, client, ERR_NOTONCHANNEL_CODE, SERVER, ERR_NOTONCHANNEL, client.getNickname().c_str(), target.c_str());
		throw NotOnChannelException("Not on channel");
	}
	for(std::map<std::string, std::string>::iterator it = usersToKick.begin(); it != usersToKick.end(); ++it)
	{
		std::vector<Client*>::iterator kickUser = c->findClientByNick(it->first);
		if (c->isEnd(kickUser) || !c->isClientInChannel((*kickUser)->getNickname()))
		{
			msg.reply(NULL, client, ERR_USERNOTINCHANNEL_CODE, SERVER, ERR_USERNOTINCHANNEL, client.getNickname().c_str(), client.getNickname().c_str(), target.c_str());
			server.logMessage(2, "User not in channel", client.getNickname());
			continue;
		}
		msg.reply(&client, **kickUser, "0", CLIENT, "KICK %s %s :%s", c->getName().c_str(), (*kickUser)->getNickname().c_str(), it->second.c_str());
		c->messageAll(&client, "KICK %s %s :%s", c->getName().c_str(), (*kickUser)->getNickname().c_str(), it->second.c_str());
		c->eraseClient((*kickUser)->getNickname(), it->second, 1);
		client.removeChannel(c->getName());
	}
}
