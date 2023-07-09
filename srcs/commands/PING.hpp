/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:47:30 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/08 22:30:36 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_HPP
# define PING_HPP

#include "ACommand.hpp"

class Ping : public ACommand
{
public:
	Ping(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Ping(const Ping &other);
	~Ping();
	Ping &operator=(const Ping &other);

	void exec();
};

#endif
