/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:25:58 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/08 22:29:03 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITE_HPP
# define INVITE_HPP

#include "ACommand.hpp"

class Invite : public ACommand
{
public:
	Invite(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Invite(const Invite &other);
	~Invite();
	Invite &operator=(const Invite &other);

	void exec();

};

#endif
