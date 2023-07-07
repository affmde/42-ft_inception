/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:40:52 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 17:38:50 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>

#include "Command.hpp"
#include "Parser.hpp"
#include "Message.hpp"
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
	input.erase(0, pos + 1);;
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
				Nick n(server, client, input, *clients);
				n.execNICK();
			} catch(ACommand::InvalidNickException &e) {
				std::cerr << client.getNickname() << " cant update NICK." << std::endl;
			} catch(ACommand::DuplicateNickException &e) {
				std::cerr << client.getNickname() << " cant update NICK because already exists." << std::endl;
			}
			break;
		}
		case USER:
			throw AlreadyRegisteredException("Already registered");
			break;
		case KICK:
		{
			try {
				Kick k(server, client, input, *clients);
				k.execKICK();
			} catch (ACommand::NoSuchChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (ACommand::NeedMoreParamsException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (ACommand::NoPrivilegesException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (ACommand::NotOnChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			}
			break;
		}
		case INVITE:
		{
			try {
				Invite i(server, client, input, *clients);
				i.execINVITE();
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
			break;
		case TOPIC:
		{
			try {
				Topic t(server, client, input, *clients);
				t.execTOPIC();
				server.logMessage(1, "Topic changed", client.getNickname());
			} catch (ACommand::NeedMoreParamsException &e) {
				Message msg;
				msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, client.getNickname().c_str(), "TOPIC");
				server.logMessage(2, "TOPIC: need more params", client.getNickname());
			} catch (ACommand::NoSuchChannelException &e) {
				server.logMessage(2, "TOPIC: no such channel", client.getNickname());
			} catch (ACommand::NotOnChannelException &e) {
				server.logMessage(2, "TOPIC: not on channel", client.getNickname());
			} catch (ACommand::NoPrivilegesException &e) {
				server.logMessage(2, "TOPIC: no priviledges", client.getNickname());
			}
			break;
		}
		case MODE:
		{
			try {
				execMODE(input);
			} catch (NoSuchChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (NoPrivilegesException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			}
			break;
		}
		case QUIT:
		{
			Quit q(server, client, input, *clients);
			q.execQUIT();
			break;
		}
		case JOIN:
			try {
				Join j(server, client, input, *clients);
				j.execJOIN();
			} catch(ACommand::NeedMoreParamsException &e) {
				server.logMessage(2, "JOIN: Need more params", client.getNickname());
				Message msg;
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
			break;
		case PART:
		{
			try {
				Part p(server, client, input, *clients);
				p.execPART();
			} catch (ACommand::NeedMoreParamsException &e) {
				server.logMessage(2, "PART: Need more params.", client.getNickname());
			}
			break;
		}
		case PRIVMSG:
		{
			try {
				Privmsg p(server, client, input, *clients);
				p.execPRIVMSG();
			} catch (ACommand::NoSuchChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (ACommand::InvalidNickException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (ACommand::NeedMoreParamsException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			}
			break;
		}
		case NOTICE:
		{
			try {
				Notice n(server, client, input, *clients);
				n.execNOTICE();
			} catch(ACommand::NeedMoreParamsException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch(ACommand::NoSuchChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch(ACommand::InvalidNickException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			}
			break;
		}
		case PING:
		{
			execPING(input);
			break;
		}
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
	return (-1);
}

void Command::execPING(std::string &input)
{
	if (input.empty())
		return ;
	Message msg;
	msg.reply(NULL, client, "0", SERVER, "PONG :%s %s", "IRCSERVER", input.c_str());
	server.logMessage(1, "PONG: " + input, "");
}

bool isStrToNumberValid(std::string num)
{
	char *end;
	long val = std::strtol(num.c_str(), &end, 10);
	if (end == num || *end != '\0' || errno == ERANGE)
		return false;
	return true;
}

void Command::execMODE(std::string &input)
{
	if (input.empty())
		return ;
	size_t pos = input.find(" ");
	std::string modesString;
	std::string target;
	if (pos == std::string::npos)
	{
		target = input;
		modesString = "";
	}
	else
	{
		target = input.substr(0, pos);
		input.erase(0, pos + 1);
		modesString = input;
	}
	if (target[0] == '#') // TARGET IS A CHANNEL!
	{
		Channel *c = server.searchChannel(target);
		if (!c)
		{
			Message msg;
			msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), target.c_str());
			throw NoSuchChannelException("No such channel");
		}
		std::string modes; //GET THE MODES STRING HERE!
		if (modesString.empty())
		{
			Message msg;
			modes = modes = c->getChannelModes();
			msg.reply(NULL, client, RPL_CHANNELMODEIS_CODE, SERVER, RPL_CHANNELMODEIS, client.getNickname().c_str(), target.c_str(), modes.c_str());
			msg.reply(NULL, client, RPL_CREATIONTIME_CODE, SERVER, RPL_CREATIONTIME, client.getNickname().c_str(), target.c_str(), c->getCreationTimestampAsString().c_str());
			server.logMessage(1, "Channel modes", client.getNickname());
		}
		else
		{
			if (!c->isOper(client.getNickname()))
			{
				Message msg;
				msg.reply(NULL, client, ERR_CHANOPRIVSNEEDED_CODE, SERVER, ERR_CHANOPRIVSNEEDED, client.getNickname().c_str(), target.c_str());
				throw NoPrivilegesException("No privileges on channel " + target);
			}
			//modesString.erase(0, 1);
			pos = modesString.find(" ");
			std::string modeList;
			std::string modeArgs;
			std::vector<std::string> argsVector;
			if (pos == std::string::npos)
			{
				modeList = modesString;
				modeArgs = "";
			}
			else
			{
				modeList = modesString.substr(0, pos);
				modesString.erase(0, pos + 1);
				modeArgs = modesString;
				argsVector = split(modeArgs, " ");
			}
			int i = 0;
			int j = 0;
			std::string mode;
			std::string args;
			bool currentIsPlus;
			if (modeList[0] == '-')
			{
				mode += "-";
				currentIsPlus = false;
			}
			else
			{
				mode += "+";
				currentIsPlus = true;
			}
			while (modeList[i])
			{
				if (modeList[i] == '+')
				{
					pos = true;
					if (!currentIsPlus)
						mode += "+";
					currentIsPlus = true;
				}
				else if (modeList[i] == '-')
				{
					pos = false;
					if (currentIsPlus)
						mode += "-";
					currentIsPlus = false;
				}
				else if (modeList[i] == 'i')
				{
					if (pos)
						c->setModesInvite(true);
					else
						c->setModesInvite(false);
					mode += "i";
				}
				else if(modeList[i] == 't')
				{
					if (pos)
						c->setModesTopic(true);
					else
						c->setModesTopic(false);
					mode += "t";
				}
				else if (modeList[i] == 'l')
				{
					if (pos)
					{
						if (j < argsVector.size())
						{
							if (!isStrToNumberValid(argsVector[j]))
							{
								i++;
								j++;
								continue;
							}
							c->setModesLimitRequired(true);
							c->setModesLimit(std::atoi(argsVector[j].c_str()));
							args += argsVector[j] + " ";
							j++;
						}
					}
					else
					{
						c->setModesLimitRequired(false);
					}
					mode += "l";
				}
				else if (modeList[i] == 'k')
				{
					if (pos)
					{
						if (j < argsVector.size())
						{
							c->setModesPassRequired(true);
							c->setPass(argsVector[j]);
							args += argsVector[j] + " ";
							j++;
						}
					}
					else
					{
						c->setModesPassRequired(false);
					}
					mode += "k";
				}
				else if (modeList[i] == 'o')
				{
					Client *clientToAdd = *c->findClientByNick(argsVector[j]);
					if (pos)
					{
						if (c->isClientInChannel(argsVector[j]) && !c->isClientBanned(argsVector[j]) && !c->isOper(argsVector[j]))
						{
							if (clientToAdd)
							{
								c->addOper(clientToAdd);
								args += client.getNickname() + " ";
							}
							mode += "o";
						}
					}
					else
					{
						if (clientToAdd)
						{
							c->removeOper(clientToAdd->getNickname());
							mode += "o";
						}
					}
					j++;
				}
				i++;
			}
			modes = mode + " " + args;
			c->messageAll(&client, "MODE %s :%s", target.c_str(), modes.c_str());
		}
	}
}

std::vector<std::string> Command::split(std::string str, std::string del)
{
	std::vector<std::string> vec;
	size_t pos;
	std::string tmp;
	while ((pos = str.find(del)) != std::string::npos)
	{
		tmp = str.substr(0, pos);
		vec.push_back(tmp);
		str.erase(0, pos + 1);
	}
	if (!str.empty())
		vec.push_back(str);
	return (vec);
}
