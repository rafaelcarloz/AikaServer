#include "NPCHandler.h"
#include "Player.h"

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
		break;
	case NpcOptionQuest:
		break;

	case NpcOptionEnterCastle:
		break;*/
	case NpcOptionStore:
		NPCHandler::DisplayShop(player, npc);
		break;
	/*case NpcOptionSkills:
		break;
	case NpcOptionStorage:
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
	/*case NpcOptionTakeQuest:
		break;

	case NpcOptionDeliveryQuest:
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

	if (player->usingNPC != nullptr) {
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
	packet.Header.Index = 0x7535;
	packet.Header.Code = PacketCode::PacketShowNPCOption;

	packet.Option = option;

	if (option == NpcOptionClose) {
		strcpy_s(packet.Text, "Fechar");
	}
	else {
		strcpy_s(packet.Text, npc->options[option].Name.c_str());
	}
	
	if (option == NpcOptionMenu) {
		packet.Show = 0xFF7FC1F4;
	}
	else {
		packet.Show = (color == 0xFFFFFFFF) ? npc->options[option].Color : color;
	}

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