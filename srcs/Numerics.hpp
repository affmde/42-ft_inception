/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 10:41:53 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/22 20:34:40 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
# define NUMERICS_HPP

#define SERVER 0
#define CLIENT 1

#define RPL_WELCOME_CODE "001"
#define RPL_YOURHOST_CODE "002"
#define RPL_CREATED_CODE "003"
#define RPL_MYINFO_CODE "004"
#define RPL_ISUPPORT_CODE "005"
#define ERR_NONICKNAMEGIVEN_CODE "431"
#define ERR_NICKNAMEINUSE_CODE "432"
#define ERR_ERRONEUSNICKNAME_CODE "433"
#define ERR_NEEDMOREPARAMS_CODE "461"
#define ERR_ALREADYREGISTERED_CODE "462"
#define ERR_PASSWDMISMATCH_CODE "464"

#define RPL_WELCOME "%s :Welcome to the IRCSERV Network, %s"			//001
#define RPL_YOURHOST "%s :Your host is IRCSERV, running version 1.0.0"	//002
#define RPL_CREATED "%s :This server was created %s"					//003
#define RPL_MYINFO "%s %s %s"											//004
#define RPL_ISUPPORT "%s %s :are supported by this server"				//005
#define ERR_NONICKNAMEGIVEN "%s :No nickname given"						//431
#define ERR_NICKNAMEINUSE "%s %s :Nickname is already in use"			//432
#define ERR_ERRONEUSNICKNAME "%s %s :Erroneus nickname"					//433
#define ERR_NEEDMOREPARAMS "%s %s :Not enough parameters"				//461
#define ERR_ALREADYREGISTERED "%s :You may not reregister"				//462
#define ERR_PASSWDMISMATCH "%s :Password incorrect"						//464

#endif
