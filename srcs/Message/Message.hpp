/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:56:05 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/17 09:10:26 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

class	Message
{
	public:
		Message(std::string);
		Message(const Message&);
		Message	&operator=(const Message&);
		~Message(void);

		void	sendData(int client_fd);

		struct SendFailed : public std::runtime_error
		{
			SendFailed(const std::string &msg) : runtime_error(msg) {}
		};
	private:
		std::string	message;
};

#endif