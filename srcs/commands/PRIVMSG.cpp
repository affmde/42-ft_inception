/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:36:33 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 17:37:36 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PRIVMSG.hpp"
#include "../Message.hpp"

Privmsg::Privmsg(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Privmsg::Privmsg(const Privmsg &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Privmsg::~Privmsg(){}
Privmsg &Privmsg::operator=(const Privmsg &other) { return *this; }

void Privmsg::execPRIVMSG()
{
	size_t pos = input.find(" ");
	if (pos == std::string::npos)
		throw NeedMoreParamsException("Need more params");
	std::vector<std::string> info = split(input, " ");
	if (info.size() < 2)
		return;
	std::vector<std::string> targets = split(info[0], ",");
	if (!info[1].empty() && info[1][0] == ':')
		info[1].erase(0, 1);
	std::string message;
	for(int i = 1; i < info.size(); i++)
	{
		message += info[i];
		if (i - 1 < info.size())
			message += " ";
	}
	for(std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
	{
		if ((*it)[0] == '#')//target is a channel
		{
			if (client.getActiveStatus() == BANNED)
				continue;
			Channel *c = server.searchChannel(*it);
			if (!c)
			{
				Message msg;
				msg.reply(NULL, client, ERR_CANNOTSENDTOCHAN_CODE, SERVER, ERR_CANNOTSENDTOCHAN, client.getNickname().c_str(), (*it).c_str());
				throw NoSuchChannelException("No such channel " + *it);
			}
			c->messageAllOthers(&client, "PRIVMSG %s :%s", (*it).c_str(), message.c_str());
		}
		else//target is a Client
		{
			Client *c = server.findClientByNick(*it);
			if (!c)
			{
				Message msg;
				msg.reply(NULL, client, ERR_NOSUCHNICK_CODE, SERVER, ERR_NOSUCHNICK, client.getNickname().c_str(), (*it).c_str());
				throw InvalidNickException("Nick doesn't exist: " + *it);
			}
			Message msg;
			msg.reply(&client, *c, "0", CLIENT, "PRIVMSG %s :%s", (*it).c_str(), message.c_str());
		}
		server.logMessage(1, "PRIVMSG " + *it + ": " + message, client.getNickname());
	}
}
