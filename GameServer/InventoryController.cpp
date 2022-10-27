#include "InventoryController.h"
#include "Logger.h"
#include "WebHandler.h"
#include "Player.h"

namespace json = boost::json;

using namespace packets;

bool InventoryController::Initialize(PCharacter character, PPlayer player) {
	this->_character = character;
	this->_player = player;

	if (!this->GetEquipmentsData()) {
		return false;
	}

	if (!this->GetInventoryData()) {
		return false;
	}

	return true;
}

bool InventoryController::GetEquipmentsData() {
	WebHandler webHandler;

	try {
		json::value response = json::value();

		if (!webHandler.GetCharacterItems(this->_character->CharacterId, 0, &response)) {
			return false;
		}

		for (auto& item : response.as_array()) {
			int itemSlot = json::value_to<int>(item.at("inventorySlot"));

			PItem _item = &this->_character->Equipaments[itemSlot];

			*_item = this->ItemFromJSON(item);

			this->_character->EquipamentsDB[itemSlot] = json::value_to<int>(item.at("characterItemID"));

			if (_item->Index == 0) {
				continue;
			}
		}

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[InventoryController::GetEquipmentsData] error: %s", e.what());
		return false;
	}

	return false;
}

bool InventoryController::GetInventoryData() {
	WebHandler webHandler;

	try {
		json::value response = json::value();

		if (!webHandler.GetCharacterItems(this->_character->CharacterId, 1, &response)) {
			return false;
		}

		for (auto& item : response.as_array()) {
			int itemSlot = json::value_to<int>(item.at("inventorySlot"));

			PItem _item = &this->_character->Inventory[itemSlot];

			*_item = this->ItemFromJSON(item);

			this->_character->InventoryDB[itemSlot] = json::value_to<int>(item.at("characterItemID"));

			if (_item->Index == 0) {
				continue;
			}
		}

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[InventoryController::GetInventoryData] error: %s", e.what());
		return false;
	}

	return false;
}


boost::json::value InventoryController::InventoryToJSON() {
	json::value characterItemsData;

	try {
		json::array& _items = characterItemsData.emplace_array();

		for (int i = 0; i < 16; i++) {
			PItem sourceItem = &this->_character->Equipaments[i];

			json::value item = this->ItemToJSON(*sourceItem, i, 0, this->_character->EquipamentsDB[i]);

			if (item.at("ItemListID").is_null()) {
				Logger::Write(Error, "[InventoryController::InventoryToJSON] generatin item json error [Equipments:%d]", i);
				continue;
			}

			_items.push_back(item);
		}

		for (int i = 0; i < 64; i++) {
			PItem sourceItem = &this->_character->Inventory[i];

			json::value item = this->ItemToJSON(*sourceItem, i, 1, this->_character->InventoryDB[i]);

			if (item.at("ItemListID").is_null()) {
				Logger::Write(Error, "[InventoryController::InventoryToJSON] generatin item json error [Inventory:%d]", i);
				continue;
			}

			_items.push_back(item);
		}

		return characterItemsData;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[InventoryController::InventoryToJSON] error: %s", e.what());
	}

	return json::value();
}



Item InventoryController::ItemFromJSON(boost::json::value item) {
	Item _item = Item();

	try {
		_item.Index = json::value_to<int>(item.at("itemListID"));
		_item.Apparence = json::value_to<int>(item.at("itemAppearanceID"));
		_item.Identific = json::value_to<int>(item.at("itemIdentication"));

		for (int i = 0; i < 3; i++) {
			_item.Effects.Index[i] = json::value_to<int>(item.at(Logger::Format("itemEffect%d_ID", i + 1)));
			_item.Effects.Value[i] = json::value_to<int>(item.at(Logger::Format("itemEffect%d_Value", i + 1)));
		}

		_item.MinDurability = (BYTE)json::value_to<int>(item.at("itemDurabilityMin"));
		_item.MaxDurability = (BYTE)json::value_to<int>(item.at("itemDurabilityMax"));

		_item.Amount = json::value_to<int>(item.at("itemAmount"));
		_item.Time = json::value_to<int>(item.at("itemExpireTime"));

		return _item;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[InventoryController::ItemFromJSON] error: %s", e.what());
		return Item();
	}

	return Item();
}

boost::json::value InventoryController::ItemToJSON(Item item, BYTE slot, BYTE slotType, uint64_t databaseId) {
	json::value itemData = json::value();

	try {
		json::object& _item = itemData.emplace_object();

		_item.emplace("CharacterItemID", databaseId);

		_item.emplace("ItemStatus", 1);
		_item.emplace("CharacterID", this->_character->CharacterId);
		_item.emplace("InventoryType", slotType);
		_item.emplace("InventorySlot", slot);

		_item.emplace("ItemListID", item.Index);
		_item.emplace("ItemAppearanceID", item.Apparence);
		_item.emplace("ItemIdentication", item.Identific);

		for (int i = 0; i < 3; i++) {
			_item.emplace(Logger::Format("ItemEffect%d_ID", i + 1), item.Effects.Index[i]);
			_item.emplace(Logger::Format("ItemEffect%d_Value", i + 1), item.Effects.Value[i]);
		}

		_item.emplace("ItemDurabilityMin", item.MinDurability);
		_item.emplace("ItemDurabilityMax", item.MaxDurability);
		_item.emplace("ItemAmount", item.Amount);
		_item.emplace("ItemExpireTime", item.Time);

		return itemData;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[InventoryController::ItemToJSON] error: %s", e.what());
	}

	return json::value();
}


PItem InventoryController::GetInventoryBagFromSlot(BYTE slot) {
	BYTE bagSlot = 60 + (BYTE)(slot / 15);

	if (bagSlot > 63) {
		bagSlot = 63;
	}

	return &this->_character->Inventory[bagSlot];
}


bool InventoryController::MoveItem(SlotType sourceType, BYTE sourceSlot, SlotType destType, BYTE destSlot) {
	if (sourceType == SlotEquipment && destType != SlotInventory) {
		return false;
	}

	if (destType == SlotEquipment && sourceType != SlotInventory) {
		return false;
	}

	PItem sourceBag;
	PItem sourceItem;

	switch (sourceType)
	{

	case SlotInventory:
		if (sourceSlot > 60) {
			return false;
		}

		sourceBag = this->GetInventoryBagFromSlot(sourceSlot);

		if (sourceBag->Index == 0) {
			return false;
		}

		sourceItem = &this->_character->Inventory[sourceSlot];
		break;

	case SlotEquipment:
		if (sourceSlot < 1 || sourceSlot > 15) {
			return false;
		}

		sourceItem = &this->_character->Equipaments[sourceSlot];
		break;

	default:
		return false;
		break;
	}

	PItem destBag;
	PItem destItem;

	switch (destType)
	{

	case SlotInventory:
		if (destSlot > 60) {
			return false;
		}

		destBag = this->GetInventoryBagFromSlot(destSlot);

		if (destBag->Index == 0) {
			return false;
		}

		destItem = &this->_character->Inventory[destSlot];
		break;

	case SlotEquipment:
		if (sourceSlot < 1 || sourceSlot > 15) {
			return false;
		}

		destItem = &this->_character->Equipaments[destSlot];
		break;

	default:
		return false;
		break;
	}

	if (!this->SwapItems(sourceItem, destItem)) {
		return false;
	}

	this->SendRefreshSlot(destType, destSlot, *destItem);
	this->SendRefreshSlot(sourceType, sourceSlot, *sourceItem);

	return true;
}

bool InventoryController::SwapItems(PItem source, PItem destination) {
	if (source->Index == destination->Index && source->CanAgroup()) {
		return this->GroupItems(source, destination);
	}

	Item temp = *source;

	ZeroMemory(source, sizeof(Item));

	*source = *destination;
	*destination = temp;
	
	return true;
}

bool InventoryController::GroupItems(PItem source, PItem destination) {
	return true;
}


bool InventoryController::SendRefreshSlot(SlotType slotType, BYTE slot, Item item, bool notify) {
	PacketRefreshItem packet;

	ZeroMemory(&packet, sizeof(PacketRefreshItem));

	packet.Header.Size = sizeof(PacketRefreshItem);
	packet.Header.Index = _player->index;
	packet.Header.Code = 0xF0E;

	packet.SlotType = slotType;
	packet.Slot = slot;

	packet.Notice = notify;

	packet.Item = item;

	return _player->SendPacket(&packet, packet.Header.Size);
}