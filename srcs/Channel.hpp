/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 19:25:23 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/09 10:16:01 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
//**Just to work home - DELETE LATER!!!!****
#include <stdexcept>

#include "Client.hpp"
#include "Time.hpp"

class Server;

struct Modes
{
	bool invite;
	bool topic;
	bool limitRequired;
	int limit;
	bool passRequired;
	int op;
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

	void setModesLimitRequired(bool req);
	bool getModesLimitRequired() const;

	void setModesPassRequired(bool req);
	bool getModesPassRequired() const;

	void setModesOp(int op);
	int getModesOp() const;

	std::string getCreationTimestampAsString() const;

	int totalClients() const;
	std::vector<Client*>::iterator findClientByNick(std::string nick);
	void eraseClient(std::string nick, std::string reason, int code);
	void messageAll(Client *sender, std::string format, ...);
	void messageAllOthers(Client * client, std::string format, ...);
	void addOper(Client *client);
	std::vector<Client*>::iterator removeOper(std::string nick);
	bool isOper(std::string nick);
	bool isClientInChannel(std::string nick);
	bool isClientBanned(std::string nick);
	std::string getChannelModes() const;
	void addInvitedClient(Client *clientToAdd);
	std::vector<Client*>::iterator removeInvitedClient(std::string nick);
	bool isClientInvited(std::string nick);
	std::vector<Client*> getInvitedClients() const;
	bool isEnd(std::vector<Client*>::iterator &it);
private:
	std::string name;
	std::vector<Client*> clients;
	std::vector<Client*> operators;
	std::vector<Client*> bannedList;
	std::vector<Client*> invitedClients;
	std::string topic;
	std::string pass;
	Server &server;
	Modes modes;
	Time creationTime;
};

#include "Server.hpp"

#endif
