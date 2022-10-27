#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <thread>

#include "..\GameServer\Logger.h"
#include "..\GameServer\Encryptation.h"

#pragma comment(lib, "Ws2_32.lib")

class ClientConnection {
private:
	SOCKET _socket;
	sockaddr _addr;

	int _packetCount;

	void GetClientAddress();

	bool ReceiveNetworkData();
public:
	bool isActive = false;

	std::string remoteAddress{};

	void Start(const SOCKET socket, const sockaddr addr);
	void Destroy();

	void HandleNetworkRecv();
	bool SendPacket(LPVOID packet, WORD size);
};