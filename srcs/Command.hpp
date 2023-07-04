/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:38:03 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/04 13:30:25 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <stdexcept>
#include <string>
#include <vector>

#include "Client.hpp"

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
#define PONG 14

class Server;

class Command
{
public:
	struct AlreadyRegisteredException : public std::runtime_error {
		AlreadyRegisteredException(const std::string &msg) : runtime_error(msg) {}
	};
	struct InvalidNickException : public std::runtime_error {
		InvalidNickException(const std::string &msg) : runtime_error(msg) {}
	};
	struct DuplicateNickException : public std::runtime_error {
		DuplicateNickException(const std::string &msg) : runtime_error(msg) {}
	};
	struct NeedMoreParamsException : public std::runtime_error {
		NeedMoreParamsException(const std::string &msg) : runtime_error(msg) {}
	};
	struct NoSuchChannelException : public std::runtime_error {
		NoSuchChannelException(const std::string &msg) : runtime_error(msg) {}
	};
	struct NotOnChannelException : public std::runtime_error {
		NotOnChannelException(const std::string &msg) : runtime_error(msg) {}
	};
	struct NoPrivilegesException : public std::runtime_error {
		NoPrivilegesException(const std::string &msg) : runtime_error(msg) {}
	};
	struct BadChannelKeyException : public std::runtime_error {
		BadChannelKeyException(const std::string &msg) : runtime_error(msg) {}
	};
	struct InviteOnlyException : public std::runtime_error {
		InviteOnlyException(const std::string &msg) : runtime_error(msg) {}
	};
	struct ChannelFullException : public std::runtime_error {
		ChannelFullException(const std::string &msg) : runtime_error(msg) {}
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
	
	int getCommandId(std::string &input) const;
	std::vector<std::string> split(std::string str, std::string del);
	void execNICK(std::string &input, std::vector<Client*> &clients);
	void execJOIN(std::string &input);
	void execPART(std::string &input);
	void execPRIVMSG(std::string &input);
	void execTOPIC(std::string &input);
	void execQUIT(std::string &input);
	void execPING(std::string &input);
	void execKICK(std::string &input);
	void execOper(std::string &input);
	void execMODE(std::string &input);
};

#include "Server.hpp"

#endif