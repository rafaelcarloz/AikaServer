#pragma once

#include "ClientConnection.h"
#include "..\GameServer\Packets.h"

class PacketHandler {
private:
	bool LoginHandler(ClientConnection client, char* buffer);
public:
	bool PacketControl(ClientConnection client, char* buffer, int receivedBytes, int initialOffset);
};
