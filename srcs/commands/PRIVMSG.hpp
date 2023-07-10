/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:35:48 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/10 16:48:31 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

#include "ACommand.hpp"

class Privmsg : public ACommand
{
public:
	Privmsg(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Privmsg(const Privmsg &other);
	~Privmsg();
	Privmsg &operator=(const Privmsg &other);

	void exec();

private:
	std::vector<std::string> targets;
	std::string message;

	void parseInput();

};

#endif
