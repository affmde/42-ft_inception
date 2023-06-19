/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 07:43:52 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/19 09:05:54 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(void)
{
	this->input = "";
	this->pass = "";
	this->nick = "";
}
Parser::Parser(const Parser &other){*this = other;}
Parser	&Parser::operator=(const Parser &other)
{
	this->input = other.input;
	this->pass = other.pass;
	this->nick = other.nick;
	return (*this);
}
Parser::~Parser(void){}


//Exceptions
const char	*Parser::WrongInputException::what() const throw()
{
	return ("Wrong input.");
}

const char	*Parser::NoPassException::what() const throw()
{
	return ("No pass.");
}

const char	*Parser::NoNickException::what() const throw()
{
	return ("Bad nick.");
}

//Member Functions

std::string	Parser::getInput(void)
{
	return (this->input);
}

void		Parser::setInput(std::string str)
{
	this->input = str;
}

std::vector<std::string>	Parser::parseInput(void)
{
	if (this->input.empty())
		throw(WrongInputException());
	std::vector<std::string>	args;
	size_t						pos;
	std::string					arg;
	if ((this->input.find("\n")) == std::string::npos)
		args.push_back(this->input);
	else
		while ((pos = this->input.find("\n")) != std::string::npos)
		{
			arg = this->input.substr(0, pos + 1);
			this->input.erase(0, pos + 1);
			args.push_back(arg);
		}
	return (args);
}

void	Parser::parsePass(std::string input)
{
	if (input.empty())
		throw(WrongInputException());
	if (input.length() - std::string("Pass ").length() < 1)
		throw(NoPassException());
	std::string	pass = input.substr(5, input.length() - 5);
}

std::string	Parser::parseNick(std::string input)
{
	if (input.empty())
		throw(WrongInputException());
	if (input.length() - std::string("NICK ").length() < 1)
		throw(NoNickException());
	std::string	nick = input.substr(5, input.length() - 5);
	return nick;
}

