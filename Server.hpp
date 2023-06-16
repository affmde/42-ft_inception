/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:25:54 by helneff           #+#    #+#             */
/*   Updated: 2023/06/16 09:24:25 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <exception>
#include <poll.h>

class Server
{
public:
	struct InitFailed : public std::runtime_error
	{
		InitFailed(const std::string &msg) : runtime_error(msg) {}
	};
	struct RecvFailed : public std::runtime_error
	{
		RecvFailed(const std::string &msg) : runtime_error(msg) {}
	};
	struct SendFailed : public std::runtime_error
	{
		SendFailed(const std::string &msg) : runtime_error(msg) {}
	};

	Server(const char *port);
	~Server();

	void	pollClientEvents();
private:
	static const int listenTimeout = 10;
	static const int bufferSize = 1024;
	int sockfd;
	char buffer[bufferSize];
	std::vector<pollfd> pollfds;

	void	registerNewUser();
	void	handleClientMessage(int *client_fd);
	void	sendAllData(int client_fd, const char *msg);
	void	emit(int client_fd, const char *msg);
};

#endif
