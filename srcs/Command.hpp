/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:38:03 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/25 16:23:14 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <stdexcept>
#include <string>
#include <vector>

#include "Client.hpp"
#include "Message.hpp"

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
#define PING 13
#define MOTD 14
#define CAP 15

class Server;

class Command
{
public:
	struct AlreadyRegisteredException : public std::runtime_error {
		AlreadyRegisteredException(const std::string &msg) : runtime_error(msg) {}
	};

	Command(std::string &input, Client &client, Server &server);
	Command(const Command &other);
	~Command();
	Command &operator=(const Command &other);

	void checkCommands(std::vector<Client*> *clients);

private:
	std::string &input;
	Client &client;
	Server &server;
	Message msg;

	void handleNick(std::vector<Client*> *clients);
	void handleKick(std::vector<Client*> *clients);
	void handleInvite(std::vector<Client*> *clients);
	void handleTopic(std::vector<Client*> *clients);
	void handleMode(std::vector<Client*> *clients);
	void handlePart(std::vector<Client*> *clients);
	void handlePrivmsg(std::vector<Client*> *clients);
	void handleNotice(std::vector<Client*> *clients);
	void handleJoin(std::vector<Client*> *clients);
	int getCommandId(std::string &input) const;
};

#include "Server.hpp"

#endif
