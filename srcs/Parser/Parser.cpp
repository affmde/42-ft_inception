/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 07:43:52 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/17 11:22:48 by andrferr         ###   ########.fr       */
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

std::string	Parser::parsePass(void)
{
	if (this->input.empty())
		throw(WrongInputException());
	if (this->input.length() - std::string("Pass ").length() < 1)
		throw(NoPassException());
	std::string	tmp;
	std::string	input = this->input;
	size_t		pos;
	while ((pos = this->input.find("\n")) != std::string::npos)
	{
		tmp = input.substr(0, pos);
		input.erase(0, pos + 1);
		if (tmp.find("PASS ") != std::string::npos)
			break;
	}
	std::string	pass = tmp.substr(5, tmp.length() - 5);
	std::cout << "Pass: " << pass << std::endl;
	return pass;
}

std::string	Parser::parseNick(void)
{
	if (this->input.empty())
		throw(WrongInputException());
	if (this->input.length() - std::string("NICK ").length() < 1)
		throw(NoNickException());
	std::string tmp;
	size_t pos;
	while ((pos = this->input.find("\n")) != std::string::npos)
	{
		tmp = this->input.substr(0, pos);
		this->input.erase(0, pos + 1);
		if (tmp.find("NICK ") != std::string::npos)
			break;
	}
	std::string	nick = tmp.substr(5, tmp.length() - 5);
	std::cout << "NICK: " << nick << std::endl;
	return nick;
}

