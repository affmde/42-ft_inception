/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MOTD.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 12:07:36 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/10 13:13:35 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> ///DELETE THIS!!!

#include "MOTD.hpp"
#include "../Message.hpp"


Motd::Motd(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Motd::Motd(const Motd &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Motd::~Motd(){}
Motd &Motd::operator=(const Motd &other) { return *this; }

void Motd::exec()
{
	if (input.empty())
		return ;
	Message msg;
	msg.reply(NULL, client, RPL_MOTDSTART_CODE, SERVER, RPL_MOTDSTART, client.getNickname().c_str(), "IRCSERV");
	std::vector<std::string> messageOfTheDay;
	messageOfTheDay.push_back("*******************************************************************************");
	messageOfTheDay.push_back(COLOUR_CYAN + std::string("Welcome to IRCSERV. This web server is made for ft_irc project from 42school HIVE, Helsinki") + COLOUR_END);
	messageOfTheDay.push_back("        _________");
	messageOfTheDay.push_back("       / ======= \\");
	messageOfTheDay.push_back("      / __________\\");
	messageOfTheDay.push_back("     | ___________ |");
	messageOfTheDay.push_back("     | | -       | |");
	messageOfTheDay.push_back("     | |         | |");
	messageOfTheDay.push_back("     | |_________| |");
	messageOfTheDay.push_back("     | |_________| |______________________");
	messageOfTheDay.push_back("      \\___________/   Andre | Hans        )");
	messageOfTheDay.push_back("     / ''''''''''' \\                     /");
	messageOfTheDay.push_back("    / ::::::::::::: \\                =D-'");
	messageOfTheDay.push_back("   (_________________)");
	messageOfTheDay.push_back("                      ");
	messageOfTheDay.push_back("Moderators:");
	messageOfTheDay.push_back(" \033[0;33m->\033[0m Andre");
	messageOfTheDay.push_back(" \033[0;33m->\033[0m Hans");
	messageOfTheDay.push_back("Behave well and enjoy our server!");
	messageOfTheDay.push_back("*******************************************************************************");
	for(std::vector<std::string>::iterator it = messageOfTheDay.begin(); it != messageOfTheDay.end(); ++it)
		msg.reply(NULL, client, RPL_MOTD_CODE, SERVER, RPL_MOTD, client.getNickname().c_str(), (*it).c_str());
	msg.reply(NULL, client, RPL_ENDOFMOTD_CODE, SERVER, RPL_ENDOFMOTD, client.getNickname().c_str());

}
