/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:48:15 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 16:48:45 by andrferr         ###   ########.fr       */
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

	void execPART();

};

#endif
