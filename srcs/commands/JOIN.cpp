/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:40:42 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/13 17:38:37 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JOIN.hpp"
#include "../Parser.hpp"

Join::Join(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Join::Join(const Join &other) :
ACommand(other.server, other.client, other.input, other.clientsList){ *this = other; }

Join::~Join(){}
Join &Join::operator=(const Join &other)
{
	channels.clear();
	for(std::vector<std::string>::const_iterator it = other.channels.begin(); it != other.channels.end(); ++it)
		channels.push_back(*it);
	keys.clear();
	for(std::vector<std::string>::const_iterator it = other.keys.begin(); it != other.keys.end(); ++it)
		keys.push_back(*it);
	return *this;
}

void Join::parseInput()
{
	size_t pos;
	std::string list;

	pos = input.find(" ");
	list = input.substr(0, pos);
	input.erase(0, pos + 1);
	channels = split(list, ",");
	keys = split(input, ",");
}

void Join::exec()
{
	parseInput();
	if (channels.size() < 1)
		throw NeedMoreParamsException("Need more params");
	for (int i = 0; i < channels.size(); i++)
	{
		Parser parser;
		if (parser.parseChannelName(channels[i]) == -1)
		{
			server.logMessage(2, "Bad channel name", client.getNickname());

			msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), channels[i].c_str());
			throw NoSuchChannelException("No such channel");
		}
		Channel *channel = server.searchChannel(channels[i]);
		if (!channel)
		{
			try{
				channel = server.createChannel(channels[i], "", keys[i], client);
				channel->addOper(&client);
			} catch (Server::ChannelLenException &e) {
				server.logMessage(2, e.what(), client.getNickname());
				msg.reply(NULL, client, "0", SERVER, "ERROR Channel name is too long");
				continue;
			}
		}
		else
		{
			if (channel->getModesInvite() && !channel->isClientInvited(client.getNickname()))
			{
				msg.reply(NULL, client, ERR_INVITEONLYCHAN_CODE, SERVER, ERR_INVITEONLYCHAN, client.getNickname().c_str(), channels[i].c_str());
				throw InviteOnlyException("Invite only channel");
			}
			if (channel->getModesLimitRequired() && channel->totalClients() >= channel->getModesLimit())
			{
				msg.reply(NULL, client, ERR_CHANNELISFULL_CODE, SERVER, ERR_CHANNELISFULL, client.getNickname().c_str(), channels[i].c_str());
				throw ChannelFullException("Channel " + channels[i] + " is full");
			}
			if (channel->getModesPassRequired() && keys[i] != channel->getPass())
			{
				msg.reply(NULL, client, ERR_BADCHANNELKEY_CODE, SERVER, ERR_BADCHANNELKEY, client.getNickname().c_str(), channel->getName().c_str());
				throw BadChannelKeyException("Invalid key to join channel " + channel->getName());
			}
		}
		try {
			channel->addUser(&client);
		} catch (Channel::AlreadyUserException &e) {
			break ;
		}
		channel->messageAll(&client, "%s %s", "JOIN", channel->getName().c_str());
		if (channel->getTopic().empty())
			msg.reply(NULL, client, RPL_NOTOPIC_CODE, SERVER, RPL_NOTOPIC, client.getNickname().c_str(), channel->getName().c_str());
		else
			msg.reply(NULL, client, RPL_TOPIC_CODE, SERVER, RPL_TOPIC, client.getNickname().c_str(), channel->getName().c_str(), channel->getTopic().c_str());
		msg.reply(NULL, client, RPL_NAMREPLY_CODE, SERVER, RPL_NAMREPLY, client.getNickname().c_str(), "=", channel->getName().c_str(), channel->getListClientsNicknames().c_str());
		msg.reply(NULL, client, RPL_ENDOFNAMES_CODE, SERVER, RPL_ENDOFNAMES, client.getNickname().c_str(), channel->getName().c_str());
		server.logMessage(1, "joined channel " + channel->getName(), client.getNickname());
		if (channel->isClientInvited(client.getNickname()))
		{
			channel->removeInvitedClient(client.getNickname());
			client.removeInvite(channel->getName());
		}
	}
}
