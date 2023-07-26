/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Motd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 12:05:46 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 12:53:13 by helneff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOTD_HPP
# define MOTD_HPP

#include "ACommand.hpp"

class Motd : public ACommand
{
public:
	Motd(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Motd(const Motd &other);
	~Motd();
	Motd &operator=(const Motd &other);

	void exec();

};

#endif
