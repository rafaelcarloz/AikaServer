#pragma once

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <string>

enum LOG_TYPE {
	None = 0,
	Status,
	ConnectionsTrafic,
	Packets,
	Warnings,
	Error
};

class Logger {

public:
	static void Write(std::string LogMsg, LOG_TYPE LogType);
	static void Write(char* LogMsg, LOG_TYPE LogType);
	static void Write(LOG_TYPE LogType, const char* fmtstring, ...);

	static void WritePackets(char* packet, int packetSize);

	static std::string Format(const char* fmtstring, ...);

	static std::string CurrentDateTime();
};


#endif