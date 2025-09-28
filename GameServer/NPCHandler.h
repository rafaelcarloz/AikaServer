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
	static bool SendCustomOption(PPlayer player, PNpc npc, NpcUsingOption option, const char* title, uint16_t data = 0, DWORD color = 0xFFFFFFFF);
	static bool SendCloseOption(PPlayer player, PNpc npc);

	//quests
	static bool DisplayQuests(PPlayer player, PNpc npc, uint16_t questId = 0);
	static bool AcceptQuest(PPlayer player, PNpc npc, uint16_t questId);

	static bool CloseNPC(PPlayer player);
	static bool DisplayDialog(PPlayer player, NpcDialog dialog);

	static bool DisplayShop(PPlayer player, PNpc npc);
public:
	static bool OpenNPC(PPlayer player, PNpc npc, packets::PacketOpenNPC& packet);
};

#endif