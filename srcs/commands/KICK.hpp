/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:01:57 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 17:02:29 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
# define KICK_HPP

#include "ACommand.hpp"

class Kick : public ACommand
{
public:
	Kick(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Kick(const Kick &other);
	~Kick();
	Kick &operator=(const Kick &other);

	void execKICK();

};

#endif
