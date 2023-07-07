/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:21:27 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 16:17:39 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

#include "NICK.hpp"
#include "../Message.hpp"
#include "../rpl_isupport.hpp"

Nick::Nick(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Nick::Nick(const Nick &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Nick::~Nick(){}
Nick &Nick::operator=(const Nick &other) { return *this; }


void Nick::execNICK()
{
	if (input.empty())
		throw NeedMoreParamsException("Need more params.");
	if (input[0] == ':')
		input.erase(0, 1);
	std::string clientNick = input;
	for (int i = 0; i < clientNick.length(); i++)
		clientNick[i] = std::tolower(clientNick[i]);
	std::string nickToCompare;
	for(std::vector<Client*>::iterator it = clientsList.begin(); it != clientsList.end(); ++it)
	{
		nickToCompare = (*(*it)).getNickname();
		for (int i = 0; i < nickToCompare.length(); i++)
			nickToCompare[i] = std::tolower(nickToCompare[i]);
		if (nickToCompare == clientNick)
		{
			Message msg;
			msg.reply(NULL, client, ERR_NICKNAMEINUSE_CODE, SERVER, ERR_NICKNAMEINUSE, client.getNickname().c_str(), input.c_str());
			throw DuplicateNickException("Duplicate nick");
		}
	}
	if (std::isdigit(input[0]) || input[0] == '#' || input[0] == ' ' || input[0] == ':')
	{
		Message msg;
		msg.reply(NULL, client, ERR_ERRONEUSNICKNAME_CODE, SERVER, ERR_ERRONEUSNICKNAME, client.getNickname().c_str(), input.c_str());
		throw InvalidNickException("Invalid Nick");
	}
	if (input.size() > NICKLEN)
		input = input.substr(0, NICKLEN);
	Message msg;
	msg.reply(&client, client, "0", CLIENT, "NICK %s", input.c_str());
	std::vector<std::string> currentChannels = client.getCurrentChannels();
	for(std::vector<std::string>::iterator it = currentChannels.begin(); it != currentChannels.end(); ++it)
	{
		Channel *chan = server.searchChannel(*it);
		if (!chan)
			continue;
		chan->messageAllOthers(&client, "NICK %s", input.c_str());
	}
	client.setNickname(input);
}


