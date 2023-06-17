/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 07:43:52 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/17 08:06:42 by andrferr         ###   ########.fr       */
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


//Member Functions

std::string	Parser::getInput(void)
{
	return (this->input);
}

void		Parser::setInput(std::string str)
{
	this->input = str;
}

