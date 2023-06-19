/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 07:41:59 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/18 10:49:00 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class	Parser
{
	public:
		Parser(void);
		Parser(const Parser&);
		Parser	&operator=(const Parser&);
		~Parser(void);

		//
		class WrongInputException : std::exception{
			public:
				const char	*what()const throw();
		};
		class NoPassException : std::exception{
			public:
				const char	*what()const throw();
		};
		class NoNickException : std::exception{
			public:
				const char	*what()const throw();
		};

		//Member Functions
		void						parsePass(std::string);
		std::string					parseNick(std::string);
		std::string					getInput(void);
		void						setInput(std::string);
		std::vector<std::string>	parseInput(void);

	private:
		std::string	input;
		std::string	pass;
		std::string	nick;
};

#endif