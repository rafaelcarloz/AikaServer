#pragma once

#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_

#include "Player.h"

class PacketHandler {
private:
	static bool OnLoginRequest(PPlayer player, char* buffer);

	static bool OnCreateCharacter(PPlayer player, char* buffer);
	static bool OnDeleteCharacter(PPlayer player, char* buffer);
	static bool OnDeleteCharacterPermanent(PPlayer player, char* buffer);

	static bool OnNumericTokenRequest(PPlayer player, char* buffer);

	static bool OnMovementPacket(PPlayer player, char* buffer);
	static bool OnRotationPacket(PPlayer player, char* buffer);
	static bool OnCharacterActionPacket(PPlayer player, char* buffer);

	static bool OnMoveItemPacket(PPlayer player, char* buffer);
	static bool OnDeleteItemPacket(PPlayer player, char* buffer);
	static bool OnGroupItemsPacket(PPlayer player, char* buffer);
	static bool OnUngroupItemsPacket(PPlayer player, char* buffer);
	static bool OnUseItemPacket(PPlayer player, char* buffer);

	static bool OnOpenNPCPacket(PPlayer player, char* buffer);
	static bool OnCloseNPC(PPlayer player, char* buffer);

	static bool OnRevivePlayerPacket(PPlayer player, char* buffer);

	static bool OnItemBarPacket(PPlayer player, char* buffer);

	static bool OnUpdateActiveTitlePacket(PPlayer player, char* buffer);
public:
	static bool PacketControl(PPlayer player, char* buffer, int receivedBytes, int initialOffset);
};

#endif
