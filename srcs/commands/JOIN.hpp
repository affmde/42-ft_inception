/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:38:23 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/13 17:30:56 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_HPP
# define JOIN_HPP

#include "ACommand.hpp"

class Join : public ACommand
{
public:
	Join(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Join(const Join &other);
	~Join();
	Join &operator=(const Join &other);

	void exec();

private:
	std::vector<std::string> channels;
	std::vector<std::string> keys;

	void parseInput();
};

#endif
