/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:25:54 by helneff           #+#    #+#             */
/*   Updated: 2023/06/15 13:29:29 by andrferr         ###   ########.fr       */
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
	struct RecvFailed : public std::runtime_error
	{
		RecvFailed(const std::string &msg) : runtime_error(msg) {}
	};

	Server(const char *port);
	~Server();

	void	poll_client_events();
private:
	static const int listen_timeout = 10;
	static const int msg_buf_size = 1024;
	int sockfd;
	unsigned char msg_buf[msg_buf_size];
	std::vector<pollfd> pollfds;

	void	register_new_user();
	void	handle_client_msg(int *client_fd);
	void	emit(unsigned char *msg, int sender);
	int		sendAllData(int socket, unsigned char *msg, int *len);
};

#endif
