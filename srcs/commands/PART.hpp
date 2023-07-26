/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:48:15 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 12:53:18 by helneff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PART_HPP
# define PART_HPP

#include "ACommand.hpp"

class Part : public ACommand
{
public:
	Part(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Part(const Part &other);
	~Part();
	Part &operator=(const Part &other);

	void exec();

};

#endif
