#pragma once

#ifndef _SERVER_SOCKET_H_
#define _SERVER_SOCKET_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <stdio.h>
#include <string>

#include "Logger.h"

#pragma comment(lib, "Ws2_32.lib")

class ServerSocket {
private:
	WSADATA _wsaData{};

	std::string _address{};
	WORD _port{};

	SOCKET _socket{};

public:
	ServerSocket() {}
	ServerSocket(std::string Address, WORD port);

	bool StartSocket();
	void CloseSocket();

	bool AcceptConnection(SOCKET *clientSocket, sockaddr *clientAddr);
};

#endif