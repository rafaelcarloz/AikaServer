#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <time.h>

#include "Logger.h"


void Logger::Write(std::string LogMsg, LOG_TYPE LogType) {
	char LogString[2048];

	sprintf_s(LogString, "%s", LogMsg.c_str());

	Logger::Write(LogString, LogType);
}

void Logger::Write(char* LogMsg, LOG_TYPE LogType) {
	char error_msg[2048] = "\n";

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (LogType)
	{

	case Status:
		sprintf_s(error_msg, "[%s] [!] %s \n", Logger::CurrentDateTime().c_str(), LogMsg);
		break;
	case ConnectionsTrafic:
		sprintf_s(error_msg, "[%s] [>>] %s \n", Logger::CurrentDateTime().c_str(), LogMsg);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case Packets:
		sprintf_s(error_msg, "[%s] [->] %s \n", Logger::CurrentDateTime().c_str(), LogMsg);
		break;
	case Warnings:
		sprintf_s(error_msg, "[%s] [X] %s \n", Logger::CurrentDateTime().c_str(), LogMsg);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case Error:
		sprintf_s(error_msg, "[%s] %s \n", Logger::CurrentDateTime().c_str(), LogMsg);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	default:
		break;
	}

	printf(error_msg);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Logger::Write(LOG_TYPE LogType, const char* fmtstring, ...) {
	char buffer[2048];

	va_list arglist;
	va_start(arglist, fmtstring);
	vsprintf_s(buffer, fmtstring, arglist);
	va_end(arglist);

	Logger::Write(buffer, LogType);
}

void Logger::WritePackets(char* packet, int packetSize) {
	std::string packetString;

	for (int i = 0; i < packetSize; i++) {
		packetString += Logger::Format("%02x ", (BYTE)packet[i]);
	}

	Logger::Write(Packets, packetString.c_str());
}


std::string  Logger::Format(const char* fmtstring, ...) {
	char buffer[2048];

	va_list arglist;
	va_start(arglist, fmtstring);
	vsprintf_s(buffer, fmtstring, arglist);
	va_end(arglist);

	return std::string(buffer);
}


std::string Logger::CurrentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	int result = localtime_s(&tstruct, &now);
	
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}