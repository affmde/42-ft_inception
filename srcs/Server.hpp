/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:25:54 by helneff           #+#    #+#             */
/*   Updated: 2023/06/19 16:41:51 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <exception>
#include <poll.h>

#include "Client.hpp"

class Server
{
public:
	struct InitException : public std::runtime_error {
		InitException(const std::string &msg) : runtime_error(msg) {}
	};
	struct RecvException : public std::runtime_error {
		RecvException(const std::string &msg) : runtime_error(msg) {}
	};
	struct UserRegistrationException : public std::runtime_error {
		UserRegistrationException(const std::string &msg) : runtime_error(msg) {}
	};

	Server(const char *port, std::string pass);
	~Server();

	void pollClientEvents();

private:
	static const int listenTimeout = 10;
	static const int bufferSize = 1024;
	char buffer[bufferSize];
	int sockfd;
	std::string pass;
	std::vector<pollfd> pollfds;
	std::vector<Client> clients;

	void registerNewUser();
	void eraseDisconnectedUsers();
	void handleClientMessage(Client &client);
	void emit(int client_fd, std::string msg);
	std::vector<pollfd>::iterator findPollfdByFD(int fd);
	std::vector<Client>::iterator findClientByFD(int fd);
	std::vector<Client>::iterator eraseUserByFD(int fd);
};

#endif
