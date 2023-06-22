/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:38:03 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/22 15:49:36 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <string>

#define PASS 0
#define NICK 1
#define USER 2
#define KICK 3
#define INVITE 4
#define TOPIC 5
#define MODE 6
#define OPER 7
#define QUIT 8
#define JOIN 9
#define PART 10
#define PRIVMSG 11
#define NOTICE 12

class Command
{
public:
	Command();
	Command(const Command &other);
	~Command();
	Command &operator=(const Command &other);

	void checkCommands(std::string &input);
	int getCommandId(std::string &input);
	
private:
	
};

#endif