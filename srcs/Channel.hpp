/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 19:25:23 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/30 16:58:12 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>

#include "Client.hpp"

class Server;

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

	std::vector<Client*>::iterator findClientByNick(std::string nick);
	void eraseClient(std::string nick, std::string reason);
	void messageAll(Client *sender, std::string format, ...);
	void messageAllOthers(Client * client, std::string format, ...);
	void addOper(Client *client);
	bool isOper(std::string nick);
	bool isClientInChannel(std::string nick);
	int totalClients() const;
private:
	std::string name;
	std::vector<Client*> clients;
	std::vector<Client*> operators;
	//CHECK BEST WAY TO ADD THE MODES!!!!!
	std::string topic;
	std::string pass;
	Server &server;
};

#include "Server.hpp"

#endif