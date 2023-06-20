/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:27:58 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/20 21:22:21 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#define CONNECTED 0
#define PASSED 1
#define REGISTERED 2
#define BANNED 3

class Client
{
public:
	Client();
	Client(int fd);
	Client(const Client &other);
	~Client();
	Client &operator=(const Client &other);

	int getClientFD() const;
	void setClientFD(int fd);

	std::string getRealname() const;
	void setRealname(std::string realName);

	std::string getHostname() const;
	void setHostname(std::string hostName);

	std::string getServername() const;
	void setServername(std::string username);

	std::string getNickname() const;
	void setNickname(std::string nickname);

	bool isConnected() const;
	void setConnected(bool connected);

	int getTotalMessages() const;
	void increaseTotalMessages();

	std::string getBuffer() const;
	void setBuffer(std::string str);
	void resetBuffer();

	int getActiveStatus() const;
	void setActiveStatus(int status);
	

	bool isReadyToSend() const;

private:
	int clientFD;
	int totalMessages;
	std::string nickname;
	std::string realname;
	std::string servername;
	std::string hostname;
	bool connected;
	int activeStatus;
	std::string buffer;
};

#endif
