/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:25:54 by helneff           #+#    #+#             */
/*   Updated: 2023/06/15 13:15:55 by andrferr         ###   ########.fr       */
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

	void	poll_client_events();
	void	emit(unsigned char*, int, int);
	int		sendAllData(int, unsigned char*, int*);

private:
	static const int listen_timeout = 10;
	int sockfd;
	std::vector<pollfd> pollfds;
};

#endif
