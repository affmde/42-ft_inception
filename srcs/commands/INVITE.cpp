/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:26:44 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 17:27:45 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "INVITE.hpp"
#include "../Message.hpp"

Invite::Invite(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Invite::Invite(const Invite &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Invite::~Invite(){}
Invite &Invite::operator=(const Invite &other) { return *this; }

void Invite::execINVITE()
{
	if (input.empty())
		throw NeedMoreParamsException("Need more params");
	size_t pos = input.find(" ");
	if (pos == std::string::npos)
		throw NeedMoreParamsException("Need more params");
	std::string targetClient = input.substr(0, pos);
	input.erase(0, pos + 1);
	std::string targetChannel = input;
	if (targetClient.empty() || targetChannel.empty())
		throw NeedMoreParamsException("Need more params");
	Channel *c = server.searchChannel(targetChannel);
	if (!c)
	{
		Message msg;
		msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), targetChannel.c_str());
		throw NoSuchChannelException("No such channel: " + targetChannel);
	}
	if (!c->isClientInChannel(client.getNickname()))
	{
		Message msg;
		msg.reply(NULL, client, ERR_NOTONCHANNEL_CODE, SERVER, ERR_NOTONCHANNEL, client.getNickname().c_str(), targetChannel.c_str());
		throw NotOnChannelException("Not on channel " + targetChannel);
	}
	if (c->getModesInvite() && !c->isOper(client.getNickname()))
	{
		Message msg;
		msg.reply(NULL, client, ERR_CHANOPRIVSNEEDED_CODE, SERVER, ERR_CHANOPRIVSNEEDED, client.getNickname().c_str(), targetChannel.c_str());
		throw NoPrivilegesException("No privileges on channel " + targetChannel);
	}
	if (c->isClientInChannel(targetClient))
	{
		Message msg;
		msg.reply(NULL, client, ERR_USERONCHANNEL_CODE, SERVER, ERR_USERONCHANNEL, client.getNickname().c_str(), targetClient.c_str(), targetChannel.c_str());
		throw UserOnChannelException(targetClient + " already on channel " + targetChannel);
	}
	Client *clientToSend = server.findClientByNick(targetClient);
	if (!clientToSend)
		return;
	Message msg;
	msg.reply(NULL, client, RPL_INVITING_CODE, SERVER, RPL_INVITING, client.getNickname().c_str(), targetClient.c_str(), targetChannel.c_str());
	msg.reply(&client, *clientToSend, "0", CLIENT, "INVITE %s :%s", targetClient.c_str(), targetChannel.c_str());
	server.logMessage(1, "Invite " + targetClient + " to channel " + targetChannel, client.getNickname());
	clientToSend->addChannelInvite(targetChannel);
	c->addInvitedClient(clientToSend);
}
