/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:31:48 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/10 17:05:23 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTICE_HPP
# define NOTICE_HPP

#include "ACommand.hpp"

class Notice : public ACommand
{
public:
	Notice(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Notice(const Notice &other);
	~Notice();
	Notice &operator=(const Notice &other);

	void exec();

private:
	std::vector<std::string> targets;
	std::string message;

	void parseInput();

};

#endif
