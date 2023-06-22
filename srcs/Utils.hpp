/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 11:51:12 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/22 11:55:29 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <sstream>
#include <string>

template <typename T>
std::string toString(T nbr)
{
	std::stringstream ss;
	ss << nbr;
	std::string str;
	ss.str(str);
	return (str);
}


#endif
