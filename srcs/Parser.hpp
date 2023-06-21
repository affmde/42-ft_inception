/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 07:41:59 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/21 08:28:47 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
//Just to work home - DELETE LATER!!!!!
#include <stdexcept>

class Parser
{
public:
	struct WrongInputException : public std::runtime_error {
		WrongInputException(const std::string &msg) : runtime_error(msg) {}
	};
	struct NoPassException : public std::runtime_error {
		NoPassException(const std::string &msg) : runtime_error(msg) {}
	};
	struct WrongPassException : public std::runtime_error {
		WrongPassException(const std::string &msg) : runtime_error(msg) {}
	};
	struct NoNickException : public std::runtime_error {
		NoNickException(const std::string &msg) : runtime_error(msg) {}
	};
	struct InvalidNickException : public std::runtime_error {
		InvalidNickException(const std::string &msg) : runtime_error(msg) {}
	};

	Parser();
	Parser(const Parser &other);
	~Parser();
	Parser &operator=(const Parser &other);

	//Member Functions
	void parsePass(std::string input, std::string password);
	void parseNick(std::string inout, std::string &nick);
	std::string getInput() const;
	void setInput(std::string);
	std::vector<std::string> parseInput();

private:
	std::string input;
	std::string pass;
	std::string nick;
};

#endif
