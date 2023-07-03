/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 19:25:23 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/03 12:02:52 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>

#include "Client.hpp"

class Server;

struct Modes
{
	bool invite;
	bool topic;
	int limit;
};

class Channel
{
public:
	struct AlreadyUserException : public std::runtime_error {
		AlreadyUserException(const std::string &msg) : runtime_error(msg) {}
	};
	
	Channel(Server &server);
	Channel(const Channel &other);
	~Channel();
	Channel &operator=(const Channel &other);

	std::string getName() const;
	void setName(std::string name);

	std::string getTopic() const;
	void setTopic(std::string topic);

	std::string getPass() const;
	void setPass(std::string pass);

	std::vector<Client*> getClients() const;
	std::vector<Client*> getOperators() const;

	void addUser(Client *client);
	std::string getListClientsNicknames();

	void setModesInvite(bool invite);
	bool getModesInvite() const;

	void setModesTopic(bool topic);
	bool getModesTopic() const;

	void setModesLimit(int limit);
	int getModesLimit() const;

	int totalClients() const;
	std::vector<Client*>::iterator findClientByNick(std::string nick);
	void eraseClient(std::string nick, std::string reason, int code);
	void messageAll(Client *sender, std::string format, ...);
	void messageAllOthers(Client * client, std::string format, ...);
	void addOper(Client *client);
	bool isOper(std::string nick);
	bool isClientInChannel(std::string nick);
	bool isClientBanned(std::string nick);
private:
	std::string name;
	std::vector<Client*> clients;
	std::vector<Client*> operators;
	std::vector<Client*> bannedList;
	//CHECK BEST WAY TO ADD THE MODES!!!!!
	std::string topic;
	std::string pass;
	Server &server;
	Modes modes;
};

#include "Server.hpp"

#endif