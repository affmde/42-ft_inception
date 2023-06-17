/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 07:41:59 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/17 08:02:57 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include <iostream>

class	Parser
{
	public:
		Parser(void);
		Parser(const Parser&);
		Parser	&operator=(const Parser&);
		~Parser(void);

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