/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:41:17 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 17:46:08 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "MODE.hpp"
#include "../rpl_isupport.hpp"
#include "../Message.hpp"

Mode::Mode(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Mode::Mode(const Mode &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Mode::~Mode(){}
Mode &Mode::operator=(const Mode &other) { return *this; }

bool Mode::isStrToNumberValid(std::string num)
{
	char *end;
	long val = std::strtol(num.c_str(), &end, 10);
	if (end == num || *end != '\0' || errno == ERANGE)
		return false;
	return true;
}

void Mode::execMODE()
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
