/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:48:56 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/14 16:25:47 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>

#include "PART.hpp"

Part::Part(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Part::Part(const Part &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Part::~Part(){}
Part &Part::operator=(const Part &other) { return *this; }

void Part::exec()
{
	std::vector<std::string> channels;
	size_t pos = input.find(" ");
	std::string channels_list = input.substr(0, pos);
	input.erase(0, pos +1);
	std::map<std::string, std::string> list;
	std::string tmp;
	while ((pos = channels_list.find(",")) != std::string::npos)
	{
		tmp = channels_list.substr(0, pos);
		channels.push_back(tmp);
		channels_list.erase(0, pos + 1);
	}
	if (!channels_list.empty())
		channels.push_back(channels_list);
	if (channels.size() < 1)
		throw NeedMoreParamsException("Need more params");
	std::string reasons;
	if (!input.empty() && input[0] == ':')
		input.erase(0, 1);
	pos = input.find(" ");
	if (pos == std::string::npos)
		reasons = "";
	else
		reasons = input;
	for(std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel *c = server.searchChannel(*it);
		if (!c)
		{
			msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), (*it).c_str());
			continue;
		}
		if (!c->isClientInChannel(client.getNickname()))
		{
			msg.reply(NULL, client, ERR_NOTONCHANNEL_CODE, SERVER, ERR_NOTONCHANNEL, client.getNickname().c_str(), (*it).c_str());
			continue;
		}
		c->eraseClient(client.getNickname(), reasons, 0);
		client.removeChannel(c->getName());
	}
}
