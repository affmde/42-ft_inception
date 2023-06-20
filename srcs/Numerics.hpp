/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 10:41:53 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/20 13:47:06 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
# define NUMERICS_HPP

#define SERVER 0
#define CLIENT 1

#define RPL_WELCOME_CODE "001"
#define ERR_NONICKNAMEGIVEN_CODE "431"
#define ERR_NICKNAMEINUSE_CODE "432"
#define ERR_ERRONEUSNICKNAME_CODE "433"

#define RPL_WELCOME "Welcome to the IRCSERV Network, %s"
#define ERR_NICKNAMEINUSE "%s :Nickname is already in use"
#define ERR_ERRONEUSNICKNAME "%s :Erroneus nickname"
#define ERR_NONICKNAMEGIVEN "No nickname given"

#endif