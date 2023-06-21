/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 07:43:52 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/21 12:08:23 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> //DELETE AFTER DEBUGGING!!!!!!!!!!!!!!!!

#include "Parser.hpp"

Parser::Parser() {}

Parser::Parser(const Parser &other) { *this = other; }

Parser::~Parser() {}

Parser &Parser::operator=(const Parser &other)
{
	if (this != &other)
	{
		input = other.input;
		pass = other.pass;
		nick = other.nick;
	}
	return *this;
}

//Member Functions

std::string Parser::getInput() const { return input; }

void Parser::setInput(std::string str) { input = str; }

std::vector<std::string> Parser::parseInput()
{
	if (input.empty())
		throw WrongInputException("Wrong input");
	std::vector<std::string>	args;
	size_t pos;
	std::string arg;
	if (input.find("\n") == std::string::npos)
		args.push_back(input);
	else
	{
		while ((pos = input.find("\n")) != std::string::npos)
		{
			arg = input.substr(0, pos + 1);
			input.erase(0, pos + 1);
			args.push_back(arg);
		}
	}
	return args;
}

void Parser::parsePass(std::string input, std::string pass)
{
	if (input[input.length() - 1] == '\n')
		input.erase(input.length() - 1, 1);
	if (input[input.length() - 1] == '\r')
		input.erase(input.length() - 1, 1);
	if (input.empty() || input.length() - std::string("Pass ").length() < 1)
		throw NoPassException("No pass");
	std::string password = input.substr(5, input.length() - 5);
	if (password[0] == ':')
		password.erase(0, 1);
	if (password.compare(pass) != 0)
		throw WrongPassException("Wrong pass");
}

void Parser::parseNick(std::string input, std::string &nick)
{
	if (input.empty())
		throw WrongInputException("Wrong input");
	if (input.length() - std::string("NICK ").length() < 1)
		throw NoNickException("No nick");
	nick = input.substr(5, input.length() - 5);
	if (nick[nick.length() - 1] == '\n')
		nick.erase(nick.length() - 1, 1);
	if (nick[nick.length() - 1] == '\r')
		nick.erase(nick.length() - 1, 1);
	if (std::isdigit(nick[0]) || nick[0] == '#' || nick[0] == ' ' || nick[0] == ':')
		throw InvalidNickException("Invalid Nick");
}

void Parser::parseUser(std::string input, Client &client)
{
	if (input[input.length() - 1] == '\n')
		input.erase(input.length() - 1, 1);
	if (input[input.length() - 1] == '\r')
		input.erase(input.length() - 1, 1);
	std::vector<std::string> args;
	size_t pos;
	std::string arg;
	while ((pos = input.find(" ")) != std::string::npos)
	{
		arg = input.substr(0, pos);
		args.push_back(arg);
		input.erase(0, pos + 1);
	}
	if (!input.length() < 1)
		args.push_back(input);
	std::string username = args[1];
	if (username.empty())
		throw EmptyUsernameException("Empty username");
	if (username.length() > maxClientUsernameLength) // REMEMBER TO ADD THIS ON THE USERLEN PROPERTY ON PL_SUPPORT (005)
		username = username.substr(0, maxClientUsernameLength);
	std::cout << "username: " << username << std::endl;

}


