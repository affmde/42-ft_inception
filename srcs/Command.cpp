/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:40:52 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/03 09:28:52 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>

#include "Command.hpp"
#include "Parser.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

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
	return (*this);
}

void Command::checkCommands(std::vector<Client*> *clients)
{
	size_t pos;
	pos = input.find(" ");
	std::string command = input.substr(0, pos);
	int commandId = getCommandId(command);
	input.erase(0, pos + 1);
	if (input[input.length() - 1] == '\n')
		input.erase(input.length() - 1, 1);
	if (input[input.length() - 1] == '\r')
		input.erase(input.length() - 1, 1);
	switch (commandId)
	{
		case PASS:
			throw AlreadyRegisteredException("Already registered");
			break;
		case NICK:
		{
			try{
				execNICK(input, *clients);
			} catch(InvalidNickException &e) {
				std::cerr << client.getNickname() << " cant update NICK." << std::endl;
			} catch(DuplicateNickException &e) {
				std::cerr << client.getNickname() << " cant update NICK because already exists." << std::endl;
			}
			break;
		}
		case USER:
			throw AlreadyRegisteredException("Already registered");
			break;
		case KICK:
		{
			execKICK(input);
			break;
		}
		case INVITE:
			break;
		case TOPIC:
		{
			try {
				execTOPIC(input);
				server.logMessage(1, "Topic changed", client.getNickname());
			} catch (NeedMoreParamsException &e) {
				Message msg;
				msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, client.getNickname().c_str(), "TOPIC");
				server.logMessage(2, "TOPIC: need more params", client.getNickname());
			} catch (NoSuchChannelException &e) {
				server.logMessage(2, "TOPIC: no such channel", client.getNickname());
			} catch (NotOnChannelException &e) {
				server.logMessage(2, "TOPIC: not on channel", client.getNickname());
			} catch (NoPrivilegesException &e) {
				server.logMessage(2, "TOPIC: no priviledges", client.getNickname());
			}
			break;
		}
		case MODE:
			break;
		case OPER:
			break;
		case QUIT:
		{
			execQUIT(input);
			break;
		}
		case JOIN:
			try {
				execJOIN(input);
			} catch(NeedMoreParamsException &e) {
				server.logMessage(2, "JOIN: Need more params", client.getNickname());
				Message msg;
				msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, client.getNickname().c_str(), "JOIN");
			} catch(NoSuchChannelException &e) {
				server.logMessage(2, "JOIN: No such channel", client.getNickname());
			} catch(BadChannelKeyException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			}
			break;
		case PART:
		{
			try {
				execPART(input);
			} catch (NeedMoreParamsException &e) {
				server.logMessage(2, "PART: Need more params.", client.getNickname());
			}
			break;
		}
		case PRIVMSG:
		{
			execPRIVMSG(input);
			break;
		}
		case NOTICE:
			break;
		case PING:
		{
			execPING(input);
			break;
		}
			
		case PONG:
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
	else if (input == "OPER")
		return OPER;
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
	else if (input == "PONG")
		return PONG;
	return (-1);
}

void Command::execNICK(std::string &input, std::vector<Client*> &clients)
{
	std::string clientNick = input;
	for (int i = 0; i < clientNick.length(); i++)
		clientNick[i] = std::tolower(clientNick[i]);
	std::string nickToCompare;
	for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
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
	client.setNickname(input);
}

void Command::execJOIN(std::string &input)
{
	std::vector<std::string> channels, keys;
	size_t pos;
	std::string arg, list;
	
	pos = input.find(" ");
	list = input.substr(0, pos);
	input.erase(0, pos + 1);
	while ((pos = list.find(",")) != std::string::npos)
	{
		arg = list.substr(0, pos);
		channels.push_back(arg);
		list.erase(0, pos + 1);
	}
	if (!list.empty())
		channels.push_back(list);
	while ((pos = input.find(",")) != std::string::npos)
	{
		arg = input.substr(0, pos);
		keys.push_back(arg);
		input.erase(0, pos + 1);
	}
	if (!input.empty())
		keys.push_back(input);
	if (channels.size() < 1)
		throw NeedMoreParamsException("Need more params");
	for (int i = 0; i < channels.size(); i++)
	{
		Parser parser;
		if (parser.parseChannelName(channels[i]) == -1)
		{
			server.logMessage(2, "Bad channel name", client.getNickname());
			Message msg;
			msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), channels[i].c_str());
			throw NoSuchChannelException("No such channel");
		}
		Channel *channel = server.searchChannel(channels[i]);
		if (!channel)
		{
			channel = server.createChannel(channels[i], "", keys[i], client);
			channel->addOper(&client);
		}
		else
		{
			if (keys[i] != channel->getPass())
			{
				Message msg;
				msg.reply(NULL, client, ERR_BADCHANNELKEY_CODE, SERVER, ERR_BADCHANNELKEY, client.getNickname().c_str(), channel->getName().c_str());
				throw BadChannelKeyException("Invalid key to join channel " + channel->getName());
			}
		}
		try {
			channel->addUser(&client);
		} catch (Channel::AlreadyUserException &e) {
			break ;
		}
		Message msg;
		channel->messageAll(&client, "%s %s", "JOIN", channel->getName().c_str());
		if (channel->getTopic().empty())
			msg.reply(NULL, client, RPL_NOTOPIC_CODE, SERVER, RPL_NOTOPIC, client.getNickname().c_str(), channel->getName().c_str());
		else
			msg.reply(NULL, client, RPL_TOPIC_CODE, SERVER, RPL_TOPIC, client.getNickname().c_str(), channel->getName().c_str(), channel->getTopic().c_str());
		msg.reply(NULL, client, RPL_NAMREPLY_CODE, SERVER, RPL_NAMREPLY, client.getNickname().c_str(), "=", channel->getName().c_str(), channel->getListClientsNicknames().c_str());
		msg.reply(NULL, client, RPL_ENDOFNAMES_CODE, SERVER, RPL_ENDOFNAMES, client.getNickname().c_str(), channel->getName().c_str());
		server.logMessage(1, "joined channel " + channel->getName(), client.getNickname());
	}
}

void Command::execPART(std::string &input)
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
	std::vector<std::string> reasons;
	if (!input.empty() && input[0] == ':')
		input.erase(0, 1);
	while ((pos = input.find(",")) != std::string::npos)
	{
		tmp = input.substr(0, pos);
		reasons.push_back(tmp);
		input.erase(0, pos + 1);
	}
	if (!input.empty())
		reasons.push_back(input);
	for(int i = 0; i < channels.size(); i++)
	{
		if(i < reasons.size())
			list.insert({channels[i], reasons[i]});
		else
			list.insert({channels[i], ""});
	}
	for(std::map<std::string, std::string>::iterator it = list.begin(); it != list.end(); ++it)
	{
		//HANDLE PART OF EVERY CHANNEL IN THE LIST!!!
		Message msg;
		Channel *c = server.searchChannel(it->first);
		if (!c)
		{
			Message msg;
			msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), (*it).first.c_str());
			continue;
		}
		if (!c->isClientInChannel(client.getNickname()))
		{
			Message msg;
			msg.reply(NULL, client, ERR_NOTONCHANNEL_CODE, SERVER, ERR_NOTONCHANNEL, client.getNickname().c_str(), (*it).first.c_str());
			continue;
		}
		c->eraseClient(client.getNickname(), it->second);
	}
}

void Command::execPRIVMSG(std::string &input)
{
	std::vector<std::string> targets;
	size_t pos;
	std::string tmp;
	pos = input.find(" ");
	if (pos == std::string::npos)
		return ;
	std::string targets_list = input.substr(0, pos);
	input.erase(0, pos + 1);
	if (input[0] == ':')
		input.erase(0, 1);
	while ((pos = targets_list.find(",")) != std::string::npos)
	{
		tmp = targets_list.substr(0, pos);
		targets.push_back(tmp);
		targets_list.erase(0, pos + 1);
	}
	if (!targets_list.empty())
		targets.push_back(targets_list);
	for(std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
	{
		Channel *c = server.searchChannel(*it);
		if (!c) continue;
		c->messageAllOthers(&client, "PRIVMSG %s :%s", c->getName().c_str(), input.c_str());
		server.logMessage(1, "PRIVMSG " + c->getName() + ": " + input, client.getNickname());
	}
}

void Command::execTOPIC(std::string &input)
{
	size_t pos;
	pos = input.find(" ");
	std::string target = input.substr(0, pos);
	if (target.empty())
		throw NeedMoreParamsException("Need more params");
	input.erase(0, pos + 1);
	Channel *c = server.searchChannel(target);
	if (!c)
	{
		Message msg;
		msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), target.c_str());
		throw NoSuchChannelException("No such channel");
	}
	if (!c->isClientInChannel(client.getNickname()))
	{
		Message msg;
		msg.reply(NULL, client, ERR_NOTONCHANNEL, SERVER, ERR_NOTONCHANNEL, client.getNickname().c_str(), c->getName().c_str());
		throw NotOnChannelException("Not on channel");
	}
	if (!c->isOper(client.getNickname()) && c->getModesTopic())
	{
		Message msg;
		msg.reply(NULL, client, ERR_CHANOPRIVSNEEDED_CODE, SERVER, ERR_CHANOPRIVSNEEDED, client.getNickname().c_str(), c->getName().c_str());
		throw NoPrivilegesException("No privileges");
	}
	if (input[0] == ':' && input.length() > 1)
	{
		input.erase(0, 1);
		c->setTopic(input);
		c->messageAll(&client, "TOPIC %s :%s", c->getName().c_str(), c->getTopic().c_str());
	}
	else if (input[0] == ':' && input.length() <= 1)
	{
		c->setTopic("");
		c->messageAll(&client, "TOPIC %s :%s", c->getName().c_str(), c->getTopic().c_str());
	}
	else
	{
		Message msg;
		msg.reply(NULL, client, RPL_TOPIC_CODE, SERVER, RPL_TOPIC, client.getNickname().c_str(), c->getName().c_str(), c->getTopic().c_str());
	}
}

void Command::execQUIT(std::string &input)
{
	if (input.empty()) return;
	if (input[0] == ':')
		input.erase(0, 1);
	Message msg;
	msg.reply(NULL, client, "0", SERVER, "ERROR: %s", input.c_str());
	for(std::vector<Channel*>::iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it)
	{
		if ((*it)->isClientInChannel(client.getNickname()))
		{
			(*it)->messageAllOthers(&client,"QUIT :Quit: %s", input.c_str());
		}
	}
	//TODO: HANDLE QUIT ON DISCONNECTION WITHOUT QUIT COMMAND (EX: CNTL_C). MAYBE SHOULD USE PING FOR THIS?
	client.setConnected(false);
	client.setActiveStatus(NOT_CONNECTED);
	server.logMessage(1, "disconnected (" + input + ")", client.getNickname());
}

void Command::execPING(std::string &input)
{
	if (input.empty())
		return ;
	Message msg;
	msg.reply(NULL, client, "0", SERVER, "PONG :%s %s", "IRCSERVER", input);
	server.logMessage(1, "PONG: " + input, "");
}

void Command::execKICK(std::string &input)
{
	std::cout << "KICK input: " << input << std::endl;
	size_t pos = input.find(" ");
	std::string channelName = input.substr(0, pos);
	input.erase(0, pos + 1);
	pos = input.find(" ");
	std::string usersString = input.substr(0, pos);
	input.erase(0 , pos + 1);
	std::vector<std::string> users = split(usersString, ",");
	for(std::vector<std::string>::iterator it = users.begin(); it != users.end(); ++it)
		std::cout << "User: " << *it << std::endl;
	if (input[0] == ':')
		input.erase(0, 1);
	std::vector<std::string> comments = split(input, ",");
	for(std::vector<std::string>::iterator it = comments.begin(); it != comments.end(); ++it)
		std::cout << "Comment: " << *it << std::endl;
	//std::vector<std::string> comments = split()
}