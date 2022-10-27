#include "DateFunctions.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

time_t DateFunctions::StringToDate(string dateTime) {
	if (dateTime.empty()) {
		return time(0);
	}

	static const std::string dateTimeFormat{ "%Y-%m-%dT%H:%M:%SZ" };

	std::istringstream ss{ dateTime };

	std::tm dt;

	ss >> std::get_time(&dt, dateTimeFormat.c_str());

	return std::mktime(&dt);
}

std::string DateFunctions::DateToString(time_t dateTime) {
	std::tm tm;

	localtime_s(&tm, &dateTime);

	std::stringstream wss;

	wss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
	return wss.str();
}