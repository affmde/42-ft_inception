/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:32:38 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 12:53:16 by helneff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Notice.hpp"

Notice::Notice(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Notice::Notice(const Notice &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }

Notice::~Notice(){}
Notice &Notice::operator=(const Notice &other)
{
	message = other.message;
	for(std::vector<std::string>::const_iterator it = other.targets.begin(); it != other.targets.end(); ++it)
		targets.push_back(*it);
	return *this;
}

void Notice::parseInput()
{
	std::vector<std::string> info = split(input, " ");
	if (info.size() < 2)
		return;
	targets = split(info[0], ",");
	if (!info[1].empty() && info[1][0] == ':')
		info[1].erase(0, 1);
	for(int i = 1; i < info.size(); i++)
	{
		message += info[i];
		if (i - 1 < info.size())
			message += " ";
	}
}

void Notice::exec()
{
	size_t pos = input.find(" ");
	if (pos == std::string::npos)
		throw NeedMoreParamsException("Need more params");
	parseInput();
	if (message.empty())
		throw NeedMoreParamsException("Need more params");
	for(std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
	{
		if ((*it)[0] == '#') //target is chennel
		{
			Channel *c = server.searchChannel(*it);
			if (!c)
				throw NoSuchChannelException("No such channel " + *it);
			c->messageAllOthers(&client, "NOTICE %s :%s", (*it).c_str(), message.c_str());
		}
		else //target is a Client
		{
			Client *c = server.findClientByNick(*it);
			if (!c)
				throw InvalidNickException("Nick doesn't exist: " + *it);
			msg.reply(&client, *c, "0", CLIENT, "NOTICE %s :%s", (*it).c_str(), message.c_str());
		}
		server.logMessage(1, "NOTICE: " + message, client.getNickname());
	}
}
