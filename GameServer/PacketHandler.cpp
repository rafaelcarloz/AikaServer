#include "PacketHandler.h"
#include "Encryptation.h"

#include "ItemList.h"
#include "InitialCharacters.h"
#include "DateFunctions.h"
#include "WebHandler.h"
#include "ServerInstance.h"
#include "UseItemHandler.h"
#include "NPCHandler.h"
#include "TitleList.h"
#include "SkillHandler.h"


namespace json = boost::json;
using namespace packets;

const int LOG_PACKETS = 1;

bool PacketHandler::PacketControl(PPlayer player, char* buffer, int receivedBytes, int initialOffset) {
	if (receivedBytes == 0 || receivedBytes < sizeof(PacketHeader)) {
		Logger::Write(Error, "received a packet too small: %d", receivedBytes);
		return false;
	}

	WORD receivedHeaderSize = reinterpret_cast<WORD&>(buffer[0]);

	if (receivedBytes != receivedHeaderSize) {
		Logger::Write(Warnings, "received different sizes: 0x%x || pSize: 0x%x \n", receivedBytes, receivedHeaderSize);
	}

	if (initialOffset > 0) {
		memmove(&buffer[0], &buffer[initialOffset], receivedBytes);
	}

	if (Encryptation::Decrypt(buffer, receivedBytes) <= 0) {
		Logger::Write(Error, "packet decryption error bytes: %d", receivedBytes);
		return true;
	}

	auto& header = reinterpret_cast<PacketHeader&>(buffer[0]);

	switch (header.Code) {

	case PacketCode::PacketMovemment: {
		PacketHandler::OnMovementPacket(player, buffer);
		break;
	}
	case PacketCode::PacketAttackTarget: {
		SkillHandler::OnAttackTarget(player, buffer);
		break;
	}
	case PacketCode::PacketSkillUse: {
		SkillHandler::OnSkillUse(player, buffer);
		break;
	}

	case PacketCode::PacketLoginRequest:
		PacketHandler::OnLoginRequest(player, buffer);
		break;
	case PacketCode::PacketCharacterCreate:
		PacketHandler::OnCreateCharacter(player, buffer);
		break;

	case PacketCode::PacketCharacterDelete:
		PacketHandler::OnDeleteCharacter(player, buffer);
		break;
	case PacketCode::PacketCharacterDeletePermanent:
		PacketHandler::OnDeleteCharacterPermanent(player, buffer);
		break;

	case PacketCode::PacketNumericToken:
		PacketHandler::OnNumericTokenRequest(player, buffer);
		break;

	case PacketCode::PacketAHackStatusCheck:
		break;

	case PacketCode::PacketEnterIngame:
		player->characterController->EnterIngame();
		break;

	case PacketCode::PacketRevive:
		PacketHandler::OnRevivePlayerPacket(player, buffer);
		break;
	case PacketCode::PacketCharacterAction:
		PacketHandler::OnCharacterActionPacket(player, buffer);
		break;
	case PacketCode::PacketCharacterRotation:
		PacketHandler::OnRotationPacket(player, buffer);
		break;

	case PacketCode::PacketOpenNPC:
		PacketHandler::OnOpenNPCPacket(player, buffer);
		break;

	case PacketCode::PacketMoveItem:
		PacketHandler::OnMoveItemPacket(player, buffer);
		break;
	case PacketCode::PacketUseItem:
		PacketHandler::OnUseItemPacket(player, buffer);
		break;
	case PacketCode::PacketItemBar:
		PacketHandler::OnItemBarPacket(player, buffer);
		break;
	case PacketCode::PacketDeleteItem:
		PacketHandler::OnDeleteItemPacket(player, buffer);
		break;
	case PacketCode::PacketJoinItems:
		PacketHandler::OnGroupItemsPacket(player, buffer);
		break;
	case PacketCode::PacketSplitItem:
		PacketHandler::OnUngroupItemsPacket(player, buffer);
		break;

	case PacketCode::PacketCloseUsingNPC:
		PacketHandler::OnCloseNPC(player, buffer);
		break;

	case PacketCode::PacketActiveTitle:
		PacketHandler::OnUpdateActiveTitlePacket(player, buffer);
		break;
	

	default:
		Logger::Write(Logger::Format("On unknown packet => size: %x - opcode: %x", header.Size, header.Code), Packets);

		if (LOG_PACKETS == 1) {
			Logger::WritePackets(buffer, header.Size);
		}

		break;
	}

	return true;
}

bool PacketHandler::OnLoginRequest(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketLoginRequest&>(buffer[0]);

	std::string username(packet.Username);
	std::string token(packet.Token);

	player->playingStatus = WaitingLogin;

	WebHandler webHandler;

	json::value* response = new json::value();

	if (!webHandler.GetSessionDataByToken(username, token, response)) {
		Logger::Write(Logger::Format("account [%s] invalid token: %s", username.c_str(), token.c_str()), LOG_TYPE::Warnings);
		player->isActive = false;
		return false;
	}

	if (!player->account->FromJSON(*response)) {
		Logger::Write(Logger::Format("account [%s] parsing session data error", username.c_str()), LOG_TYPE::Error);
		player->isActive = false;
		return false;
	}

	player->SendCharacterList();

	return true;
}


bool PacketHandler::OnCreateCharacter(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketCharacterCreate&>(buffer[0]);

	ItemList* itemList = ItemList::GetInstance();

	if (packet.Face < 10 || packet.Face > 69) {
		player->SendClientMessage("face não reconhecida pelo servidor. CUIDADO!!");
		return true;
	}

	if (packet.Hair < 7700 || packet.Hair > 7779) {
		player->SendClientMessage("hair não reconhecida pelo servidor. CUIDADO!!");
		return true;
	}

	if (packet.SlotIndex > 2) {
		player->SendClientMessage("slot de personagem não reconhecida pelo servidor. CUIDADO!!");
		return true;
	}

	Character baseCharacter = InitialCharacters::GetInstance()->Get(itemList->Get(packet.Face).CharacterClass);

	baseCharacter.Equipaments[0].Index = packet.Face;
	baseCharacter.Equipaments[0].Apparence = packet.Face;
	baseCharacter.Equipaments[1].Index = packet.Hair;
	baseCharacter.Equipaments[1].Apparence = packet.Hair;

	WebHandler webHandler;
	json::value response = json::value();

#pragma region "Generate Character JSON"
	json::value characterData;

	try {
		json::object& _character = characterData.emplace_object();

		_character.emplace("AccountID", player->account->accountId);

		_character.emplace("CharacterName", packet.Name);
		_character.emplace("CharacterSlot", packet.SlotIndex);
		_character.emplace("CharacterClass", (int)baseCharacter.ClassInfo);
		_character.emplace("CharacterFirstLogin", 1);

		_character.emplace("CharacterLevel", baseCharacter.Level);
		_character.emplace("CurrentExperience", baseCharacter.ExpPoints);

		_character.emplace("CharacterGold", baseCharacter.Gold);

		switch (packet.Local) {
		case 0:
			_character.emplace("CharacterPositionX", 3450);
			_character.emplace("CharacterPositionY", 690);
			break;
		case 1:
			_character.emplace("CharacterPositionX", 3470);
			_character.emplace("CharacterPositionY", 935);
			break;
		default:
			break;
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnCreateCharacter_Character] error: %s", e.what());
	}

#pragma endregion
#pragma region "Send Character Data"

	if (!webHandler.PostCharacter(characterData, &response)) {
		player->SendClientMessage("erro ao criar personagem!");
		return true;
	}

	if (!json::value_to<bool>(response.at("success"))) {
		player->SendClientMessage(json::value_to<std::string>(response.at("message")));
		return true;
	}

	long characterId = json::value_to<int>(response.at("data").at("characterID"));

#pragma endregion
#pragma region "Generate Items JSON"

	json::value characterItemsData;

	try {
		json::array& _items = characterItemsData.emplace_array();

		for (int slot = 0; slot < 16; slot++) {
			PItem sourceItem = &baseCharacter.Equipaments[slot];

			if (sourceItem->Index == 0) {
				continue;
			}

			json::value item;
			json::object& _item = item.emplace_object();

			_item.emplace("CharacterItemID", 0);

			_item.emplace("ItemStatus", 1);
			_item.emplace("CharacterID", characterId);
			_item.emplace("InventoryType", 0);
			_item.emplace("InventorySlot", slot);

			_item.emplace("ItemListID", sourceItem->Index);
			_item.emplace("ItemAppearanceID", sourceItem->Apparence);
			_item.emplace("ItemIdentication", sourceItem->Identific);

			for (int i = 0; i < 3; i++) {
				_item.emplace(Logger::Format("ItemEffect%d_ID", i+1), sourceItem->Effects.Index[i]);
				_item.emplace(Logger::Format("ItemEffect%d_Value", i + 1), sourceItem->Effects.Value[i]);
			}

			_item.emplace("ItemDurabilityMin", sourceItem->MinDurability);
			_item.emplace("ItemDurabilityMax", sourceItem->MaxDurability);
			_item.emplace("ItemAmount", sourceItem->Amount); 
			_item.emplace("ItemExpireTime", sourceItem->Time);
			
			_items.emplace_back(_item);
		}

		for (int slot = 0; slot < 64; slot++) {
			PItem sourceItem = &baseCharacter.Inventory[slot];

			if (sourceItem->Index == 0) {
				continue;
			}

			json::value item;
			json::object& _item = item.emplace_object();

			_item.emplace("CharacterItemID", 0);

			_item.emplace("ItemStatus", 1);
			_item.emplace("CharacterID", characterId);
			_item.emplace("InventoryType", 1);
			_item.emplace("InventorySlot", slot);

			_item.emplace("ItemListID", sourceItem->Index);
			_item.emplace("ItemAppearanceID", sourceItem->Apparence);
			_item.emplace("ItemIdentication", sourceItem->Identific);

			for (int i = 0; i < 3; i++) {
				_item.emplace(Logger::Format("ItemEffect%d_ID", i + 1), sourceItem->Effects.Index[i]);
				_item.emplace(Logger::Format("ItemEffect%d_Value", i + 1), sourceItem->Effects.Value[i]);
			}

			_item.emplace("ItemDurabilityMin", sourceItem->MinDurability);
			_item.emplace("ItemDurabilityMax", sourceItem->MaxDurability);
			_item.emplace("ItemAmount", sourceItem->Amount);
			_item.emplace("ItemExpireTime", sourceItem->Time);

			_items.emplace_back(_item);
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnCreateCharacter_Items] error: %s", e.what());
	}

#pragma endregion
#pragma region "Send Items Data"

	if (!webHandler.PostCharacterItems(characterId, characterItemsData, &response)) {
		player->SendClientMessage("erro ao salvar items do personagem!");
		return true;
	}

#pragma endregion
#pragma region "Generate Skills JSON"

	json::value characterSkillsData;

	try {
		json::array& _skills = characterSkillsData.emplace_array();

		for (int slot = 0; slot < 59; slot++) {
			PCharacterSkill skillItem = &baseCharacter.SkillList[slot];

			json::value item;
			json::object& _item = item.emplace_object();

			_item.emplace("CharacterSkillID", skillItem->databaseID);
			_item.emplace("SkillStatus", (skillItem->skillID == 0)? 0 : 1);
			_item.emplace("CharacterID", characterId);
			_item.emplace("SkillType", (slot < 6) ? 0 : 1);
			_item.emplace("SkillSlot", slot);

			_item.emplace("SkillListID", skillItem->skillID);

			_skills.emplace_back(_item);
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnCreateCharacter_Skills] error: %s", e.what());
	}

#pragma endregion
#pragma region "Send Skills Data"

	if (!webHandler.PostCharacterSkills(characterId, characterSkillsData, &response)) {
		player->SendClientMessage("erro ao salvar skills do personagem!");
		return true;
	}

#pragma endregion
#pragma region "Generate HotBar JSON"

	json::value characterHotBarData;

	try {
		json::array& _items = characterHotBarData.emplace_array();

		for (int slot = 0; slot < 24; slot++) {
			PCharacterItemBar hotBarItem = &baseCharacter.ItemBar[slot];

			json::value item;
			json::object& _item = item.emplace_object();

			_item.emplace("CharacterHotBarID", hotBarItem->databaseID);
			_item.emplace("ItemStatus", (hotBarItem->itemID == 0) ? 0 : 1);
			_item.emplace("CharacterID", characterId);
			_item.emplace("ItemType", 0);
			_item.emplace("ItemSlot", slot);

			_item.emplace("ItemID", hotBarItem->itemID);

			_items.emplace_back(_item);
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnCreateCharacter_Skills] error: %s", e.what());
	}

#pragma endregion
#pragma region "Send HotBar Data"

	if (!webHandler.PostCharacterHotBar(characterId, characterHotBarData, &response)) {
		player->SendClientMessage("erro ao salvar hotbar do personagem!");
		return true;
	}

#pragma endregion

	player->SendCharacterList();

	return true;
}

bool PacketHandler::OnDeleteCharacter(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketCharacterDelete&>(buffer[0]);

	if (packet.AcountIndex != player->account->accountId) {
		player->SendClientMessage("incorrect accound ID. ???");
		return true;
	}

	if (packet.SlotIndex > 2) {
		player->SendClientMessage("invalid character slot. ???");
		return true;
	}

	try {
		int numericErrors = json::value_to<int>(player->_characterList[packet.SlotIndex].at("numericTokenError"));

		if (numericErrors >= 5) {
			player->SendClientMessage("character blocked, please contact support");
			return true;
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnDeleteCharacter] error: %s", e.what());
		return true;
	}

	if (!player->CheckNumericToken(packet.SlotIndex, packet.NumericToken)) {
		player->UpdateNumericToken(packet.SlotIndex, NULL, true);
		player->SendClientMessage("invalid numeric token");
		player->SendCharacterList();
		return true;
	}

	long characterId = 0;

	try {
		characterId = json::value_to<int>(player->_characterList[packet.SlotIndex].at("characterID"));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnDeleteCharacter] error: %s", e.what());
		return true;
	}

	json::value deletionData = json::value();

	json::object& data = deletionData.emplace_object();

	data.emplace("CharacterID", characterId);
	data.emplace("DeleteStatus", (packet.CancelDelete == 1 ? 0 : 1));
	data.emplace("DaleteTime", DateFunctions::DateToString((time_t)(time(0) + (3 * 24 * 60 * 60))));

	WebHandler webHandler;

	json::value response = json::value();

	if (!webHandler.PostCharacterDeleteStatus(characterId, deletionData, &response)) {
		player->SendClientMessage("error on saving character delete request");
		return true;
	}

	player->SendCharacterList();

	return true;
}

bool PacketHandler::OnDeleteCharacterPermanent(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketCharacterDeletePermanent&>(buffer[0]);

	if (packet.AcountIndex != player->account->accountId) {
		player->SendClientMessage("incorrect accound ID. ???");
		return true;
	}

	if (packet.SlotIndex > 2) {
		player->SendClientMessage("invalid character slot. ???");
		return true;
	}

	try {
		int numericErrors = json::value_to<int>(player->_characterList[packet.SlotIndex].at("numericTokenError"));

		if (numericErrors >= 5) {
			player->SendClientMessage("character blocked, please contact support");
			return true;
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnDeleteCharacterPermanent] error: %s", e.what());
		return true;
	}

	if (!player->CheckNumericToken(packet.SlotIndex, packet.NumericToken)) {
		player->UpdateNumericToken(packet.SlotIndex, NULL, true);
		player->SendClientMessage("invalid numeric token");
		player->SendCharacterList();
		return true;
	}

	long characterId = 0;

	try {
		characterId = json::value_to<int>(player->_characterList[packet.SlotIndex].at("characterID"));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnDeleteCharacterPermanent] error: %s", e.what());
		return true;
	}

	try {
		time_t deleteTime = DateFunctions::StringToDate(json::value_to<std::string>(player->_characterList[packet.SlotIndex].at("scheduledDeleteOn")));

		if (deleteTime > time(0)) {
			player->SendClientMessage("not possible to delete yet");
			return true;
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnDeleteCharacterPermanent] error: %s", e.what());
		return true;
	}

	json::value deletionData = json::value();

	json::object& data = deletionData.emplace_object();

	data.emplace("CharacterID", characterId);
	data.emplace("DeleteStatus", 2);
	data.emplace("DaleteTime", DateFunctions::DateToString((time_t)(time(0) + (3 * 24 * 60 * 60))));

	WebHandler webHandler;

	json::value response = json::value();

	if (!webHandler.PostCharacterDeleteStatus(characterId, deletionData, &response)) {
		player->SendClientMessage("error on saving character delete request");
		return true;
	}

	player->SendCharacterList();

	return true;
}


bool PacketHandler::OnNumericTokenRequest(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketNumericToken&>(buffer[0]);

	if (packet.SlotIndex > 2) {
		player->SendClientMessage("invalid character slot. ???");
		return true;
	}

	long characterId = 0;

	try {
		int numericErrors = json::value_to<int>(player->_characterList[packet.SlotIndex].at("numericTokenError"));
		characterId = json::value_to<int>(player->_characterList[packet.SlotIndex].at("characterID"));

		if (numericErrors >= 5) {
			player->SendClientMessage("character blocked, please contact support");
			return true;
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnNumericTokenRequest] error: %s", e.what());
		return true;
	}

	switch (packet.RequestChange) {

	case NumericRequestType::NumericRegister: {
		if (player->CharacterHasNumericToken(packet.SlotIndex)) {
			player->SendCharacterList();
			player->SendClientMessage("character already has a numeric token");
			return true;
		}

		player->UpdateNumericToken(packet.SlotIndex, packet.Numeric_1);

		player->characterController->Initialize(characterId, player);

		break;
	}

	case NumericRequestType::NumericCheck: {
		if (!player->CheckNumericToken(packet.SlotIndex, packet.Numeric_1)) {
			player->UpdateNumericToken(packet.SlotIndex, NULL, true);
			player->SendCharacterList();
			player->SendClientMessage("invalid numeric token");
			return true;
		}

		player->UpdateNumericToken(packet.SlotIndex, NULL);

		player->characterController->Initialize(characterId, player);

		break;
	}

	case NumericRequestType::NumericChange: {
		if (!player->CheckNumericToken(packet.SlotIndex, packet.Numeric_2)) {
			player->UpdateNumericToken(packet.SlotIndex, NULL, true);
			player->SendCharacterList();
			player->SendClientMessage("invalid numeric token");
			return true;
		}

		if (!player->UpdateNumericToken(packet.SlotIndex, packet.Numeric_1)) {
			player->SendCharacterList();
			return true;
		}
	
		player->characterController->Initialize(characterId, player);

		break;
	}

	default: {
		player->SendCharacterList();
		player->SendClientMessage("what are you doing??");
		break;
	}

	}

	return true;
}


bool PacketHandler::OnMovementPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketMovemment&>(buffer[0]);

	player->position = packet.Destination;

	player->SendMovemment(player->position, packet.MoveType, packet.Speed);

	return true;
}

bool PacketHandler::OnRotationPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketSignalData&>(buffer[0]);

	player->rotation = packet.Data;


	player->SendRotation();

	return true;
}

bool PacketHandler::OnCharacterActionPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketCharacterAction&>(buffer[0]);

	player->SendAction(packet.actionId);

	return true;
}



bool PacketHandler::OnMoveItemPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketMoveItem&>(buffer[0]);

	if (!player->inventoryController.MoveItem(packet.SrcType, packet.SrcSlot, packet.DestType, packet.DestSlot)) {
		return true;
	}

	if (packet.SrcType == SlotEquipment || packet.DestType == SlotEquipment) {
		player->statusController.Recalculate();
		player->characterController->RespawnCharacter();
	}

	return true;
}

bool PacketHandler::OnDeleteItemPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketDeleteItem&>(buffer[0]);

	return player->inventoryController.DeleteItem((SlotType)packet.SlotType, packet.Slot);
}

bool PacketHandler::OnGroupItemsPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketJoinItems&>(buffer[0]);

	if (packet.SrcSlot > 59 || packet.DestSlot > 59) {
		return false;
	}

	PItem sourceItem = &player->character.Inventory[packet.SrcSlot];
	PItem destItem = &player->character.Inventory[packet.DestSlot];

	if (!player->inventoryController.GroupItems(sourceItem, destItem)) {
		return false;
	}

	player->inventoryController.SendRefreshSlot(SlotInventory, packet.SrcSlot, *sourceItem);
	player->inventoryController.SendRefreshSlot(SlotInventory, packet.DestSlot, *destItem);
}

bool PacketHandler::OnUngroupItemsPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketSplitItem&>(buffer[0]);

	return player->inventoryController.UngroupItems((SlotType)packet.SlotType, (BYTE)packet.Slot, (WORD)packet.Amount);
}

bool PacketHandler::OnRevivePlayerPacket(PPlayer player, char* buffer) {
	if (!player->IsDead()) {
		return true;
	}

	ServerInstance* instance = ServerInstance::GetInstance();

	if (player->account->accountNation != instance->GetNationData()->nationId) {
		player->characterController->Teleport(2944, 1664);
	}
	else {
		player->characterController->SendToSavedPosition();
	}

	player->character.Status.Life.CurHP = (uint32_t)(player->statusController.volatileStatus.Life.MaxHP / 0x10);
	player->character.Status.Life.CurMP = (uint32_t)(player->statusController.volatileStatus.Life.MaxMP / 0x10);

	player->buffsController->Recalculate();
	player->statusController.Recalculate();

	return true;
}


bool PacketHandler::OnUseItemPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketUseItem&>(buffer[0]);

	if (player->IsDead()) {
		return false;
	}

	UseItemHandler handler = UseItemHandler();

	switch (packet.TypeSlot)
	{

	case SlotInventory:
		if (packet.Slot > 59) {
			return false;
		}
		
		handler.UseInventoryItem(player, packet.Slot, packet.Type1);
		break;

	default:
		break;
	}
	
	return true;
}


bool PacketHandler::OnOpenNPCPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketOpenNPC&>(buffer[0]);

	if (packet.Index == 0 && packet.Type1 == 8) {
		player->usingNPC = nullptr;
		player->usingNPCOption = 0;
		return true;
	}

	Logger::Write(Packets, "using npc: %d - %d", packet.Index, packet.Type1);

	EntityType entityType = EntityHandler::GetEntityType(packet.Index);

	switch (entityType)
	{
	case EntityPlayer:
		break;
	case EntityNPC:
		NPCHandler::OpenNPC(player, (PNpc)EntityHandler::GetEntity(packet.Index), packet);
		break;
	default:
		break;
	}

	return true;
}

bool PacketHandler::OnCloseNPC(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketSignalData&>(buffer[0]);

	if (player->usingNPC == nullptr) {
		return true;
	}

	player->usingNPC = nullptr;
	player->usingNPCOption = 0;

	return true;
}


bool PacketHandler::OnItemBarPacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketItemBar&>(buffer[0]);

	if (packet.destSlot >= 24) {
		player->SendClientMessage("item bar invalid slot!");
		return false;
	}

	if (packet.destType > 10) {
		player->SendClientMessage("item bar invalid type!");
		return false;
	}

	BYTE slot = (BYTE)packet.destSlot;
	WORD index = (WORD)packet.index;

	player->character.ItemBar[slot].itemID = index;
	player->character.ItemBar[slot].itemType = (WORD)packet.destType;

	player->characterController->SendItemBarSlot(packet.destSlot);

	return true;
}


bool PacketHandler::OnUpdateActiveTitlePacket(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketUpdateActiveTitle&>(buffer[0]);

	if (packet.TitleIndex == 0) {
		player->characterController->activeTitle = nullptr;

		player->characterController->SendActiveTitle();
		return true;
	}

	try {
		for (int i = 0; i < 96; i++) {
			if (player->character.Titles[i].title.Index != packet.TitleIndex) {
				continue;
			}

			player->characterController->activeTitle = &player->character.Titles[i].title;
			break;
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[PacketHandler::OnUpdateActiveTitlePacket] invalid title data -> titleId: %d", packet.TitleIndex);
		return false;
	}

	player->characterController->SendActiveTitle();
	return true;
}