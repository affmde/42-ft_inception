/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:56:05 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/19 17:29:36 by andrferr         ###   ########.fr       */
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
	Message();
	Message(std::string msg);
	Message(const Message& other);
	~Message();
	Message &operator=(const Message &other);

	void setMessage(std::string message);
	std::string getMessage(void);
	void sendData(int client_fd);
private:
	std::string message;
};

#endif
