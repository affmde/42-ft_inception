/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:40:52 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 16:52:13 by andrferr         ###   ########.fr       */
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
				execKICK(input);
			} catch (NoSuchChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (NeedMoreParamsException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (NoPrivilegesException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (NotOnChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			}
			break;
		}
		case INVITE:
		{
			try {
				execINVITE(input);
			} catch (NeedMoreParamsException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (NoSuchChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (NotOnChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (NoPrivilegesException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (UserOnChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			}
		}
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
			execQUIT(input);
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
			} catch (NeedMoreParamsException &e) {
				server.logMessage(2, "PART: Need more params.", client.getNickname());
			}
			break;
		}
		case PRIVMSG:
		{
			try {
				execPRIVMSG(input);
			} catch (NoSuchChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (InvalidNickException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch (NeedMoreParamsException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			}
			break;
		}
		case NOTICE:
		{
			try {
				execNOTICE(input);
			} catch(NeedMoreParamsException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch(NoSuchChannelException &e) {
				server.logMessage(2, e.what(), client.getNickname());
			} catch(InvalidNickException &e) {
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
	if (input.size() > TOPICLEN)
		input = input.substr(0, TOPICLEN);
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
			client.removeChannel((*it)->getName());
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
	msg.reply(NULL, client, "0", SERVER, "PONG :%s %s", "IRCSERVER", input.c_str());
	server.logMessage(1, "PONG: " + input, "");
}

void Command::execKICK(std::string &input)
{
	if (input.empty())
	{
		Message msg;
		msg.reply(NULL, client, ERR_NEEDMOREPARAMS_CODE, SERVER, ERR_NEEDMOREPARAMS, client.getNickname().c_str(), "KICK");
		throw NeedMoreParamsException ("KICK: Need more params");
	}
	size_t pos = input.find(" ");
	std::string channelName = input.substr(0, pos);
	input.erase(0, pos + 1);
	pos = input.find(" ");
	std::string usersString = input.substr(0, pos);
	input.erase(0 , pos + 1);
	std::vector<std::string> users = split(usersString, ",");
	if (input[0] == ':')
		input.erase(0, 1);
	std::vector<std::string> comments = split(input, ",");
	Channel *c = server.searchChannel(channelName);
	if (!c)
	{
		Message msg;
		msg.reply(NULL, client, ERR_NOSUCHCHANNEL_CODE, SERVER, ERR_NOSUCHCHANNEL, client.getNickname().c_str(), channelName.c_str());
		throw NoSuchChannelException("No such channel " + channelName);
	}
	if (!c->isOper(client.getNickname()))
	{
		Message msg;
		msg.reply(NULL, client, ERR_CHANOPRIVSNEEDED_CODE, SERVER, ERR_CHANOPRIVSNEEDED, client.getNickname().c_str(), channelName.c_str());
		throw NoPrivilegesException("No privileges: Not oper.");
	}
	if (!c->isClientInChannel(client.getNickname()))
	{
		Message msg;
		msg.reply(NULL, client, ERR_NOTONCHANNEL_CODE, SERVER, ERR_NOTONCHANNEL, client.getNickname().c_str(), channelName.c_str());
		throw NotOnChannelException("Not on channel");
	}
	std::map<std::string, std::string> usersToKick;

	for (int i = 0; i < users.size(); i++)
	{
		if (i < comments.size())
		{
			std::string reason;
			if (comments[i].size() > KICKLEN)
				reason = comments[i].substr(0, KICKLEN);
			else
				reason = comments[i];
			usersToKick.insert(std::pair<std::string, std::string>(users[i], reason));
		}
		else
			usersToKick.insert(std::pair<std::string, std::string>(users[i], "You were kicked from the channel " + channelName));
	}
	for(std::map<std::string, std::string>::iterator it = usersToKick.begin(); it != usersToKick.end(); ++it)
	{
		if (!c->isClientInChannel(it->first))
		{
			Message msg;
			msg.reply(NULL, client, ERR_USERNOTINCHANNEL_CODE, SERVER, ERR_USERNOTINCHANNEL, client.getNickname().c_str(), client.getNickname().c_str(), channelName.c_str());
			server.logMessage(2, "User not in channel", client.getNickname());
			continue;
		}
		Client *kickUser = *c->findClientByNick(it->first);
		Message msg;
		msg.reply(&client, *kickUser, "0", CLIENT, "KICK %s %s :%s", c->getName().c_str(), it->first.c_str(), it->second.c_str());
		c->messageAll(&client, "KICK %s %s :%s", c->getName().c_str(), it->first.c_str(), it->second.c_str());
		c->eraseClient(it->first, it->second, 1);
		client.removeChannel(c->getName());
	}
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

void Command::execINVITE(std::string &input)
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

void Command::execPRIVMSG(std::string &input)
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

void Command::execNOTICE(std::string &input)
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
			Message msg;
			msg.reply(&client, *c, "0", CLIENT, "NOTICE %s :%s", (*it).c_str(), message.c_str());
		}
		server.logMessage(1, "NOTICE: " + message, client.getNickname());
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
