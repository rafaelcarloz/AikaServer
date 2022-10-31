#pragma once

#ifndef _NPC_HANDLER_H_
#define _NPC_HANDLER_H_

#include "Player.fwd.h"
#include "NPC.h"
#include "Packets.h"

class NPCHandler {
private:
	static bool DisplayOptions(PPlayer player, PNpc npc);
	static bool SendOption(PPlayer player, PNpc npc, NpcUsingOption option, DWORD color = 0xFFFFFFFF);
	static bool SendCloseOption(PPlayer player, PNpc npc);

	static bool CloseNPC(PPlayer player);
	static bool DisplayDialog(PPlayer player, NpcDialog dialog);

	static bool DisplayShop(PPlayer player, PNpc npc);
public:
	static bool OpenNPC(PPlayer player, PNpc npc, packets::PacketOpenNPC& packet);
};

#endif