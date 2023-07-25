/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAP.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:41:01 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/25 17:30:16 by andrferr         ###   ########.fr       */
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

void Cap::exec()
{
	parseInput();
	std::cout << "subcommand: " << subcommand << std::endl;
	std::cout << "code: " << code <<  std::endl;
	msg.reply(NULL, client, "0", SERVER, "CAP * LS :");
}
