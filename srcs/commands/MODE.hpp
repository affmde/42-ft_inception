/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:40:40 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/07 17:44:00 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
# define MODE_HPP

#include "ACommand.hpp"

class Mode : public ACommand
{
public:
	Mode(Server &server, Client &client, std::string &input, std::vector<Client*> &clientsList);
	Mode(const Mode &other);
	~Mode();
	Mode &operator=(const Mode &other);

	void execMODE();

private:
	bool isStrToNumberValid(std::string num);

};

#endif
