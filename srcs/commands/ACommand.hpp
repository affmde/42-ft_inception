/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:24:05 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 15:44:00 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

#include <string>

#include "../Server.hpp"
#include "../Client.hpp"

class ACommand
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
	struct UserOnChannelException : public std::runtime_error {
		UserOnChannelException(const std::string &msg) : runtime_error(msg) {}
	};

	ACommand(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	ACommand(const ACommand &other);
	~ACommand();
	ACommand &operator=(const ACommand &other);

protected:
	Server &server;
	Client &client;
	std::string &input;
	std::vector<Client*> &clientsList;

};

#endif
