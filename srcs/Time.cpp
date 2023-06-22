/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrferr <andrferr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 10:18:58 by andrferr          #+#    #+#             */
/*   Updated: 2023/06/22 12:07:59 by andrferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>

#include "Time.hpp"
#include "Utils.hpp"

Time::Time() { getDate(date); }
Time::Time(const Time &other) { *this = other; }
Time::~Time() {}
Time &Time::operator=(const Time &other)
{
	if (this == & other) return *this;
	date.year = other.getYear();
	date.month = other.getMonth();
	date.day = other.getDay();
	date.hour = other.getHour();
	date.min = other.getMin();
	date.sec = other.getSec();
	date.weekDay = other.getWeekday();
	return (*this);
}

int Time::getYear() const { return date.year; }
int Time::getMonth() const { return date.month; }
int Time::getDay() const { return date.day; }
int Time::getHour() const { return date.hour; }
int Time::getMin() const { return date.min; }
int Time::getSec() const { return date.sec; }
std::string Time::getWeekday() const { return date.weekDay; }

void Time::getDate(Date &date)
{
	std::time_t time = std::time(0);
	std::tm *now = std::localtime(&time);

	date.year = now->tm_year + 1900;
	date.month = now->tm_mon + 1;
	date.day = now->tm_mday;
	date.hour = now->tm_hour;
	date.min = now->tm_min;
	date.sec = now->tm_sec;
	switch (now->tm_wday)
	{
		case 0:
			date.weekDay = "Sunday";
			break;
		case 1:
			date.weekDay = "Monday";
			break;
		case 2:
			date.weekDay = "Tuesday";
			break;
		case 3:
			date.weekDay = "Wednesday";
			break;
		case 4:
			date.weekDay = "Thursday";
			break;
		case 5:
			date.weekDay = "Friday";
			break;
		case 6:
			date.weekDay = "Sunday";
			break;
		default:
			date.weekDay = "Error: couldn't detect day of the week";
	}
}

std::string Time::getDateAsString() const
{
	std::string year = toString(date.year);
	std::string month = toString(date.month);
	std::string day = toString(date.day);
	std::string hour = toString(date.hour);
	std::string min = toString(date.min);
	std::string sec = toString(date.sec);
	std::string date = year + "-" + month + "-" + day + " " + hour + ":" + min + ":" + sec + " " + getWeekday();
	return (date);
}
