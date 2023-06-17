/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 07:41:59 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/17 10:41:10 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include <iostream>
#include <sstream>
#include <string>

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
		std::string	parsePass(void);
		std::string	parseNick(void);
		std::string	getInput(void);
		void		setInput(std::string);

	private:
		std::string	input;
		std::string	pass;
		std::string	nick;
};

#endif