/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 19:25:23 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/28 17:41:06 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>

#include "Client.hpp"

class Channel
{
public:
	struct AlreadyUserException : public std::runtime_error {
		AlreadyUserException(const std::string &msg) : runtime_error(msg) {}
	};
	
	Channel();
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
	std::string getListClientsNicknames() const;

	std::vector<Client*>::iterator findClientByNick(std::string nick);
	void eraseClient(std::string nick); ///IMPLEMENT THIS NEXT!!!!! NEED TO ERSE CLIENT FROM THE CHANNELS WHEN THEY DISCONNET
	void messageAll(Client *sender, std::string message);
private:
	std::string name;
	std::vector<Client*> clients;
	std::vector<Client*> operators;
	//CHECK BEST WAY TO ADD THE MODES!!!!!
	std::string topic;
	std::string pass;
};


#endif