/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 12:40:05 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/16 13:04:29 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class Client
{
	public:
		Client(void);
		Client(const Client &);
		Client	&operator=(const Client&);
		~Client(void);

		int	getClientFd(void);
	private:
		int	clientFd;
		int	totalMessages;

};
