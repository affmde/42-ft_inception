/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:40:52 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/25 16:24:57 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Parser.hpp"
#include "Channel.hpp"
#include "rpl_isupport.hpp"
#include "commands/NICK.hpp"
#include "commands/JOIN.hpp"
#include "commands/PART.hpp"
#include "commands/TOPIC.hpp"
#include "commands/KICK.hpp"
#include "commands/QUIT.hpp"
#include "commands/INVITE.hpp"
#include "commands/NOTICE.hpp"
#include "commands/PRIVMSG.hpp"
#include "commands/MODE.hpp"
#include "commands/PING.hpp"
#include "commands/MOTD.hpp"

Command::Command(std::string &input, Client &client, Server &server) :
input(input),
client(client),
server(server) {}
Command::Command(const Command &other) : input(other.input), client(other.client), server(other.server) { *this = other; }
Command::~Command() {}
Command &Command::operator=(const Command &other)
{
	if (this == &other) return *this;
	input = other.input;
	client = other.client;
	msg = other.msg;
	server = other.server;
	return (*this);
}

void Command::handleNick(std::vector<Client*> *clients)
{
	try{
		Nick n(server, client, input, *clients);
		n.exec();
	} catch(ACommand::InvalidNickException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch(ACommand::DuplicateNickException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	}
}

void Command::handleKick(std::vector<Client*> *clients)
{
	try {
		Kick k(server, client, input, *clients);
		k.exec();
	} catch (ACommand::NoSuchChannelException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::NeedMoreParamsException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::NoPrivilegesException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::NotOnChannelException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	}
}

void Command::handleInvite(std::vector<Client*> *clients)
{
	try {
		Invite i(server, client, input, *clients);
		i.exec();
	} catch (ACommand::NeedMoreParamsException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::NoSuchChannelException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::NotOnChannelException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::NoPrivilegesException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::UserOnChannelException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	}
}

void Command::handleTopic(std::vector<Client*> *clients)
{
	try {
		Join j(server, client, input, *clients);
		j.exec();
	} catch(ACommand::NeedMoreParamsException &e) {
		server.logMessage(2, "JOIN: Need more params", client.getNickname());
		msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, client.getNickname().c_str(), "JOIN");
	} catch(ACommand::NoSuchChannelException &e) {
		server.logMessage(2, "JOIN: No such channel", client.getNickname());
	} catch(ACommand::BadChannelKeyException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch(ACommand::InviteOnlyException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch(ACommand::ChannelFullException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	}
}

void Command::handleMode(std::vector<Client*> *clients)
{
	try {
		Mode m(server, client, input, *clients);
		m.exec();
	} catch (ACommand::NoSuchChannelException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::NoPrivilegesException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	}
}

void Command::handlePart(std::vector<Client*> *clients)
{
	try {
		Part p(server, client, input, *clients);
		p.exec();
	} catch (ACommand::NeedMoreParamsException &e) {
		server.logMessage(2, "PART: Need more params.", client.getNickname());
	}
}

void Command::handlePrivmsg(std::vector<Client*> *clients)
{
	try {
		Privmsg p(server, client, input, *clients);
		p.exec();
	} catch (ACommand::NoSuchChannelException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::InvalidNickException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch (ACommand::NeedMoreParamsException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	}
}

void Command::handleNotice(std::vector<Client*> *clients)
{
	try {
		Notice n(server, client, input, *clients);
		n.exec();
	} catch(ACommand::NeedMoreParamsException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch(ACommand::NoSuchChannelException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch(ACommand::InvalidNickException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	}
}

void Command::handleJoin(std::vector<Client*> *clients)
{
	try {
		Join j(server, client, input, *clients);
		j.exec();
	} catch(ACommand::NeedMoreParamsException &e) {
		server.logMessage(2, "JOIN: Need more params", client.getNickname());
		msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, client.getNickname().c_str(), "JOIN");
	} catch(ACommand::NoSuchChannelException &e) {
		server.logMessage(2, "JOIN: No such channel", client.getNickname());
	} catch(ACommand::BadChannelKeyException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch(ACommand::InviteOnlyException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	} catch(ACommand::ChannelFullException &e) {
		server.logMessage(2, e.what(), client.getNickname());
	}
}

void Command::checkCommands(std::vector<Client*> *clients)
{
	if (input[input.length() - 1] == '\n')
		input.erase(input.length() - 1, 1);
	if (input[input.length() - 1] == '\r')
		input.erase(input.length() - 1, 1);
	size_t pos;
	pos = input.find(" ");
	std::string command = input.substr(0, pos);
	int commandId = getCommandId(command);
	input.erase(0, pos + 1);;
	switch (commandId)
	{
		case PASS:
			throw AlreadyRegisteredException("Already registered");
			break;
		case NICK:
			handleNick(clients);
			break;
		case USER:
			throw AlreadyRegisteredException("Already registered");
			break;
		case KICK:
			handleKick(clients);
			break;
		case INVITE:
			handleInvite(clients);
			break;
		case TOPIC:
			handleTopic(clients);
			break;
		case MODE:
			handleMode(clients);
			break;
		case QUIT:
		{
			Quit q(server, client, input, *clients);
			q.exec();
			break;
		}
		case JOIN:
			handleJoin(clients);
			break;
		case PART:
			handlePart(clients);
			break;
		case PRIVMSG:
			handlePrivmsg(clients);
			break;
		case NOTICE:
			handleNotice(clients);
			break;
		case PING:
		{
			Ping p(server, client, input, *clients);
			p.exec();
			break;
		}
		case MOTD:
		{
			Motd m(server, client, input, *clients);
			m.exec();
			break;
		}
		case CAP:
			break;
		default:
			break;
	}
}

int Command::getCommandId(std::string &input) const
{
	if (input == "PASS")
		return PASS;
	else if (input == "NICK")
		return NICK;
	else if (input == "USER")
		return USER;
	else if (input == "KICK")
		return KICK;
	else if (input == "INVITE")
		return INVITE;
	else if (input == "TOPIC")
		return TOPIC;
	else if (input == "MODE")
		return MODE;
	else if (input == "QUIT")
		return QUIT;
	else if (input == "JOIN")
		return JOIN;
	else if (input == "PART")
		return PART;
	else if (input == "PRIVMSG")
		return PRIVMSG;
	else if (input == "NOTICE")
		return NOTICE;
	else if (input == "PING")
		return PING;
	else if (input == "motd")
		return MOTD;
	else if (input == "CAP")
		return CAP;
	return (-1);
}
