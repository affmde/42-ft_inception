/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 10:18:51 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/22 11:57:53 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIME_HPP
# define TIME_HPP

#include <string>

struct Date
{
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
	std::string weekDay;
};

class Time
{
public:
	Time();
	Time(const Time &other);
	~Time();
	Time &operator=(const Time &other);

	void getDate(Date &date);
	int getYear() const;
	int getMonth() const;
	int getDay() const;
	int getHour() const;
	int getMin() const;
	int getSec() const;
	std::string getWeekday() const;
	std::string getDateAsString(void) const;
private:
	Date date;
};

#endif
