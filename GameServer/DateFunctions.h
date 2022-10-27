#pragma once

#ifndef _DATE_FUNCTIONS_H_
#define _DATE_FUNCTIONS_H_

#include <stdio.h>
#include <string>

class DateFunctions {
public:
	static time_t StringToDate(std::string dateTime);
	static std::string DateToString(time_t dateTime);
};

#endif