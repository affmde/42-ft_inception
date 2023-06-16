/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:27:58 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/16 15:37:36 by andrferr         ###   ########.fr       */
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
		int	getClientFd(void);
		int	getClientTotalMessages(void);

	private:
		int			clientFd;
		int			totalMessages;
		std::string	nickname;
		std::string	userName;
		bool		connected;
};

#endif