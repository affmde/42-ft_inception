/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:21:27 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 12:53:14 by helneff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

#include "Nick.hpp"
#include "../rpl_isupport.hpp"

Nick::Nick(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Nick::Nick(const Nick &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Nick::~Nick(){}
Nick &Nick::operator=(const Nick &other) { return *this; }

bool Nick::isNickValid()
{
	if (std::isdigit(input[0]) || input[0] == '#' || input[0] == ' ' || input[0] == ':')
	{
		msg.reply(NULL, client, ERR_ERRONEUSNICKNAME_CODE, SERVER, ERR_ERRONEUSNICKNAME, client.getNickname().c_str(), input.c_str());
		return false;
	}
	return true;
}

void Nick::informChannels()
{
	std::vector<std::string> currentChannels = client.getCurrentChannels();
	for(std::vector<std::string>::iterator it = currentChannels.begin(); it != currentChannels.end(); ++it)
	{
		Channel *chan = server.searchChannel(*it);
		if (!chan)
			continue;
		chan->messageAllOthers(&client, "NICK :%s", input.c_str());
	}
}

void Nick::exec()
{
	if (input.empty())
		throw NeedMoreParamsException("Need more params.");
	std::string clientNick = server.toLowercase(input);
	if (server.isDuplicate(input))
	{
		msg.reply(NULL, client, ERR_NICKNAMEINUSE_CODE, SERVER, ERR_NICKNAMEINUSE, client.getNickname().c_str(), input.c_str());
		throw DuplicateNickException("Duplicate nick");
	}
	if (!isNickValid())
		throw InvalidNickException("Invalid Nick");
	if (input.size() > NICKLEN)
		input = input.substr(0, NICKLEN);
	msg.reply(&client, client, "0", CLIENT, "NICK %s", input.c_str());
	informChannels();
	client.setNickname(input);
}


