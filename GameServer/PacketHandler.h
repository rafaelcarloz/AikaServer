#pragma once

#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_

#include "Player.h"

class PacketHandler {
private:
	static bool RecvLoginRequest(PPlayer player, char* buffer);

	static bool RecvCreateCharacter(PPlayer player, char* buffer);
	static bool RecvDeleteCharacter(PPlayer player, char* buffer);
	static bool RecvDeleteCharacterPermanent(PPlayer player, char* buffer);

	static bool RecvNumericTokenRequest(PPlayer player, char* buffer);

	static bool RecvMovementPacket(PPlayer player, char* buffer);
	static bool RecvRotationPacket(PPlayer player, char* buffer); 
	static bool RecvCharacterActionPacket(PPlayer player, char* buffer);

	static bool RecvMoveItemPacket(PPlayer player, char* buffer);
	static bool RecvDeleteItemPacket(PPlayer player, char* buffer);
	static bool RecvGroupItemsPacket(PPlayer player, char* buffer);
	static bool RecvUngroupItemsPacket(PPlayer player, char* buffer);
	static bool RecvUseItemPacket(PPlayer player, char* buffer);

	static bool RecvRevivePlayerPacket(PPlayer player, char* buffer);
public:
	static bool PacketControl(PPlayer player, char* buffer, int receivedBytes, int initialOffset);
};

#endif
