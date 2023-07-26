/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helneff <helneff@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:40:40 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/26 12:53:12 by helneff          ###   ########.fr       */
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

	void exec();

private:
	bool isStrToNumberValid(std::string num);

};

#endif
