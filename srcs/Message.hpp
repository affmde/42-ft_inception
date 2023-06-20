/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:56:05 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/20 12:59:20 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <exception>
#include <string>
//**Just to work home - DELETE LATER!!!!****
#include <stdexcept>
#include <string.h>
//*******************************************

#include "Client.hpp"
#include "Numerics.hpp"

class Message
{
public:
	struct SendFailed : public std::runtime_error {
		SendFailed(const std::string &msg) : runtime_error(msg) {}
	};
	Message();
	Message(std::string msg);
	Message(const Message& other);
	~Message();
	Message &operator=(const Message &other);

	void setMessage(std::string message);
	std::string getMessage(void) const ;
	void sendData(int client_fd) const;
	void RPL_Welcome(int client_fd, std::string nick) const;
	void reply(Client *sender, Client &receiver, std::string code, int header, std::string format, ...);
private:
	std::string message;

	std::string	toString(int number) const;
};

#endif
