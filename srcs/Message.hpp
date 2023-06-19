/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:56:05 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/19 16:23:30 by helneff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <exception>
#include <string>

class Message
{
public:
	struct SendFailed : public std::runtime_error {
		SendFailed(const std::string &msg) : runtime_error(msg) {}
	};

	Message(std::string);
	Message(const Message& other);
	~Message();
	Message &operator=(const Message &other);

	void sendData(int client_fd);

private:
	std::string message;
};

#endif