/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:47:30 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 12:53:19 by helneff          ###   ########.fr       */
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
