/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:25:54 by helneff           #+#    #+#             */
/*   Updated: 2023/06/15 12:48:18 by helneff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
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

	Server(const char *port);
	~Server();

	void poll_client_events();

private:
	static const int listen_timeout = 10;
	int sockfd;
	std::vector<pollfd> pollfds;
};

#endif
