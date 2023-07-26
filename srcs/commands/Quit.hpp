/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:20:48 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 12:53:22 by helneff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUIT_HPP
# define QUIT_HPP

#include "ACommand.hpp"

class Quit : public ACommand
{
public:
	Quit(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Quit(const Quit &other);
	~Quit();
	Quit &operator=(const Quit &other);

	void exec();

};

#endif
