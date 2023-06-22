/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 11:51:12 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/22 21:55:12 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <sstream>
#include <string>

template <typename T>
std::string toString(T nbr)
{
	std::ostringstream ss;
	std::string ret;
	ss << nbr;
	ret = ss.str();
	return (ret);
}

#endif
