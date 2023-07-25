/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAP.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:39:37 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/25 17:55:32 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAP_HPP
# define CAP_HPP

#include "ACommand.hpp"

class Cap : public ACommand
{
public:
	Cap(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Cap(const Cap &other);
	~Cap();
	Cap &operator=(const Cap &other);

	void exec();
	void parseInput();

private:
	bool isValidCAP();

	int code;
	std::string subcommand;
};

#endif
