/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:27:58 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/18 10:36:24 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_HPP
# define Client_HPP

#include <iostream>

class	Client
{
	public:
		Client(void);
		Client(const Client&);
		Client	&operator=(const Client&);
		~Client(void);

		//Member Functions
		int			getClientFd(void);
		int			getTotalMessages(void);
		std::string	getUsername(void);
		std::string	getNickname(void);
		bool		isConnected(void);
		std::string	getBuffer(void);
		bool		isLogged(void);
		bool		isBanned(void);

		void		setClientFd(int);
		void		increaseTotalMessages(void);
		void		setUsername(std::string);
		void		setNickname(std::string);
		void		setConnected(bool);
		void		setBuffer(std::string);
		void		resetBuffer(void);
		bool		readyToSend(void);
		void		setLogged(bool);
		void		setBanned(bool);

	private:
		int			clientFd;
		int			totalMessages;
		std::string	nickname;
		std::string	username;
		bool		connected;
		bool		logged;
		bool		banned;
		std::string	buffer;
};

#endif