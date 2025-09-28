#include "NPCHandler.h"
#include "Player.h"
#include "Quests.h"

using namespace packets;

bool NPCHandler::OpenNPC(PPlayer player, PNpc npc, PacketOpenNPC& packet) {
	if (player == nullptr || npc == nullptr) {
		return false;
	}

	printf("%d -> %d \n", npc->index, packet.Index);

	switch ((NpcUsingOption)packet.Type1)
	{

	case NpcOptionOpen: 
		NPCHandler::DisplayOptions(player, npc);
		break;			  
	/*case NpcOptionTalk:
		break;*/
	case NpcOptionQuest:
		NPCHandler::DisplayQuests(player, npc, packet.Type2);
		break;

	/*
	case NpcOptionEnterCastle:
		break;*/
	case NpcOptionStore:
		NPCHandler::DisplayShop(player, npc);
		break;
	case NpcOptionSkills:
		player->characterController->SendCharacterSkills(npc->index);
		break;
		/*case NpcOptionStorage:
		break;*/
	case NpcOptionClose: 
		NPCHandler::CloseNPC(player);
		break;
	case NpcOptionSignCastle:
		break;
	case NpcOptionGuildCreate:
		player->usingNPC = nullptr;
		player->usingNPCOption = 0;

		player->SendSignalData(PacketCode::PacketCreateGuild, 0);
		break;
	/*case NpcOptionGuildStorage:
		break;
			
	case NpcOptionPranStation:
		break; */

	case NpcOptionAnvil:
		NPCHandler::DisplayDialog(player, NpcDialogAnvil);
		break;

	case NpcOptionReinforce:
		NPCHandler::DisplayDialog(player, NpcDialogReinforce);
		break;

	case NpcOptionEnchant:
		NPCHandler::DisplayDialog(player, NpcDialogEnchant);
		break;

	case NpcOptionTransformApp:
		NPCHandler::DisplayDialog(player, NpcDialogTransformApp);
		break;

	case NpcOptionLeveling:
		NPCHandler::DisplayDialog(player, NpcDialogLeveling);
		break;

	case NpcOptionTimat:
		NPCHandler::DisplayDialog(player, NpcDialogTimat);
		break;

	case NpcOptionMenu:
		NPCHandler::DisplayOptions(player, npc);
		break;
	case NpcOptionTakeQuest:
		NPCHandler::AcceptQuest(player, npc, packet.Type2);
		break;

	/*case NpcOptionDeliveryQuest:
		break;
	case NpcOptionSaveLocation:
		break;
	case NpcOptionDungeon:
		break;*/

	case NpcOptionRepair:
		NPCHandler::DisplayDialog(player, NpcDialogRepair);
		break;

	case NpcOptionRepairAll:
		NPCHandler::DisplayDialog(player, NpcDialogRepairAll);
		break;
			
	case NpcOptionRecycle:
		NPCHandler::DisplayDialog(player, NpcDialogRecycle);
		break;

	//case NpcOptionInitialBuff:
		//break;

	case NpcOptionEnchantMount:
		NPCHandler::DisplayDialog(player, NpcDialogEnchantMount);
		break;

	case NpcOptionAuction:
		NPCHandler::DisplayDialog(player, NpcDialogAuction);
		break;

	case NpcOptionEnchantPran:
		NPCHandler::DisplayDialog(player, NpcDialogEnchantPran);
		break;

	default:
		Logger::Write(Packets, "unmaped npc option: %d", packet.Type1);

		NPCHandler::CloseNPC(player);
		break;
	}

	if (packet.Type1 > 0 && packet.Type1 != 8) {
		player->usingNPCOption = packet.Type1;
	}

	return true;
}

bool NPCHandler::DisplayOptions(PPlayer player, PNpc npc) {
	if (player->IsDead()) {
		return false;
	}

	if (player->usingNPC != nullptr && player->usingNPC->index != npc->index) {
		player->SendSignal(PacketCode::PacketCloseNPC);
		player->usingNPC = nullptr;
		return true;
	}

	player->SendSignal(PacketCode::PacketUnknow110);
	player->SendSignalData(PacketCode::PacketNPCTalk, npc->index);

	for (auto& option : npc->options) {
		NPCHandler::SendOption(player, npc, (NpcUsingOption)option.first);
	}

	NPCHandler::SendCloseOption(player, npc);

	player->usingNPC = npc;
	player->usingNPCOption = 0;

	return true;
}

#pragma region "quests"

bool NPCHandler::DisplayQuests(PPlayer player, PNpc npc, uint16_t questId) {
	player->SendSignal(PacketCode::PacketUnknow110);

	if (questId > 0) {
		QuestData questData = QuestList::Get(questId);

		player->SendSignalData(PacketCode::PacketAnimationTalk, questData.DialogStart);
		NPCHandler::SendCustomOption(player, npc, NpcOptionTakeQuest, "Aceitar", questData.Index);
	}
	else {
		for (int i = 0; i < npc->quests->size(); i++) {
			if (!player->questsController->AvailableContains(npc->quests->at(i))) {
				continue;
			}

			QuestData questData = QuestList::Get(npc->quests->at(i));

			NPCHandler::SendCustomOption(player, npc, NpcOptionQuest, questData.Title, questData.Index);
		}
	}

	NPCHandler::SendCloseOption(player, npc);

	return true;
}


bool NPCHandler::AcceptQuest(PPlayer player, PNpc npc, uint16_t questId) {
	if (!npc->HasQuest(questId)) {
		player->SendClientMessage("NPC does not have this quest");
		return true;
	}

	if (!player->questsController->AvailableContains(questId)) {
		player->SendClientMessage("This quest is not available for you yet");
		return true;
	}

	if (player->questsController->activeQuests.size() >= 16) {
		player->SendClientMessage("You cannot accept any more quests");
		return true;
	}

	NPCHandler::CloseNPC(player);

	return player->questsController->AcceptQuest(questId);
}

#pragma endregion




bool NPCHandler::CloseNPC(PPlayer player) {
	player->usingNPC = nullptr;
	player->usingNPCOption = 0;

	player->SendSignal(PacketCode::PacketCloseNPC);

	return true;
}


bool NPCHandler::SendOption(PPlayer player, PNpc npc, NpcUsingOption option, DWORD color) {
	PacketShowNPCOption packet;

	ZeroMemory(&packet, sizeof(PacketShowNPCOption));

	packet.Header.Size = sizeof(PacketShowNPCOption);
	packet.Header.Index = DEFAULT_SERVER_CLIENT_ID;
	packet.Header.Code = PacketCode::PacketShowNPCOption;

	packet.Option = option;

	if (option == NpcOptionClose) {
		strcpy_s(packet.Text, "Fechar");
	}
	else {
		strcpy_s(packet.Text, npc->options[option].Name.c_str());
	}
	
	if (option == NpcOptionMenu) {
		strcpy_s(packet.Text, "Menu");
		packet.Show = 0xFF7FC1F4;
	}
	else {
		packet.Show = (color == 0xFFFFFFFF) ? npc->options[option].Color : color;
	}

	player->SendPacket(&packet, packet.Header.Size);

	return true;
}

bool NPCHandler::SendCustomOption(PPlayer player, PNpc npc, NpcUsingOption option, const char* title, uint16_t data, DWORD color) {
	PacketShowNPCOption packet;

	ZeroMemory(&packet, sizeof(PacketShowNPCOption));

	packet.Header.Size = sizeof(PacketShowNPCOption);
	packet.Header.Index = DEFAULT_SERVER_CLIENT_ID;
	packet.Header.Code = PacketCode::PacketShowNPCOption;

	packet.Option = option;
	packet.Data = data;

	strcpy_s(packet.Text, title);

	packet.Show = color;

	player->SendPacket(&packet, packet.Header.Size);

	return true;
}

bool NPCHandler::SendCloseOption(PPlayer player, PNpc npc) {
	return NPCHandler::SendOption(player, npc, NpcOptionClose, 0xFFEB5A5A);
}

bool NPCHandler::DisplayDialog(PPlayer player, NpcDialog dialog) {
	player->SendSignal(PacketCode::PacketCloseNPC);
	player->SendSignalData(PacketCode::PacketNpcDialog, dialog);

	return true;
}



bool NPCHandler::DisplayShop(PPlayer player, PNpc npc) {
	PacketNPCShop packet;

	ZeroMemory(&packet, sizeof(PacketNPCShop));

	packet.header.Size = sizeof(PacketNPCShop);
	packet.header.Index = player->index;
	packet.header.Code = PacketCode::PacketNPCShop;

	packet.index = npc->index;
	packet.shopType = NpcShopItems;

	for (int i = 0; i < 40; i++) {
		packet.items[i] = npc->character.Inventory[i].Index;
	}

	player->SendSignal(PacketCode::PacketCloseNPC);

	return player->SendPacket(&packet, packet.header.Size);
}