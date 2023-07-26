/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:31:48 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 12:53:16 by helneff          ###   ########.fr       */
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
