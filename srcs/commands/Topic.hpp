/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:55:00 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 12:53:24 by helneff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_HPP
# define TOPIC_HPP

#include "ACommand.hpp"

class Topic : public ACommand
{
public:
	Topic(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Topic(const Topic &other);
	~Topic();
	Topic &operator=(const Topic &other);

	void exec();

};

#endif
