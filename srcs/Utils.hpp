/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 11:51:12 by andrferr          #+#    #+#             */
/*   Updated: 2023/07/03 09:13:44 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <sstream>
#include <string>
#include <vector>

template <typename T>
std::string toString(T nbr)
{
	std::ostringstream ss;
	std::string ret;
	ss << nbr;
	ret = ss.str();
	return (ret);
}

std::vector<std::string> split(std::string str, std::string del)
{
	std::vector<std::string> vec;
	size_t pos;
	std::string tmp;
	while ((pos = str.find(del)) != std::string::npos)
	{
		tmp = str.substr(0, pos);
		vec.push_back(tmp);
		str.erase(0, pos + 1);
	}
	if (!str.empty())
		vec.push_back(str);
	return (vec);
}

#endif
