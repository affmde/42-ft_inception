/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 15:27:58 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/19 16:25:11 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

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
	void setRealname(std::string username);

	std::string getHostname() const;
	void setHostname(std::string username);

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

	bool isLogged() const;
	void setLogged(bool logged);

	bool isBanned() const;
	void setBanned(bool banned);

	bool isReadyToSend() const;

private:
	int clientFD;
	int totalMessages;
	std::string nickname;
	std::string realname;
	std::string servername;
	std::string hostname;
	bool connected;
	bool logged;
	bool banned;
	std::string buffer;
};

#endif
