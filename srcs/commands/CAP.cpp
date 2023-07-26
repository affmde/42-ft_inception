/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAP.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:41:01 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 10:57:40 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> //DELETE THIS!!!! JUST FOR DEBUGGING!!!

#include "CAP.hpp"

Cap::Cap(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList) :
ACommand(server, client, input, clientsList){}

Cap::Cap(const Cap &other) :
ACommand(other.server, other.client, other.input, other.clientsList) { *this = other; }
Cap::~Cap(){}
Cap &Cap::operator=(const Cap &other) { return *this; }

void Cap::parseInput()
{
	size_t pos  = input.find(" ");
	subcommand = input.substr(0, pos);
	input.erase(0, pos + 1);
	code = std::atoi(input.c_str());
}

bool Cap::isValidCAP()
{
	if (subcommand == "LS" || subcommand == "LIST" || subcommand == "REQ"\
	 || subcommand == "ACK" || subcommand == "NAK" || subcommand == "END"\
	 || subcommand == "NEW (302)" || subcommand == "DEL (302)")
		return true;
	return false;
}

void Cap::exec()
{
	if (input[input.length() - 1] == '\n')
		input.erase(input.length() - 1, 1);
	if (input[input.length() - 1] == '\r')
		input.erase(input.length() - 1, 1);
	parseInput();
	if (!isValidCAP())
	{
		msg.reply(NULL, client, ERR_BADCAP_CODE, SERVER, ERR_BADCAP, subcommand.c_str());
		throw BadCapException("Invalid CAP command: " + subcommand);
	}
	msg.reply(NULL, client, "0", SERVER, "CAP * LS :");
}
