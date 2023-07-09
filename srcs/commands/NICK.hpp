/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:19:29 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/09 15:15:23 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
# define NICK_HPP

#include "ACommand.hpp"

class Nick : public ACommand
{
public:
	Nick(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Nick(const Nick &other);
	~Nick();
	Nick &operator=(const Nick &other);

	void exec();
	
private:
	bool isNickValid();
	void informChannels();

};

#endif
