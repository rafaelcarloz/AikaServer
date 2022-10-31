#include "InventoryController.h"
#include "Logger.h"
#include "WebHandler.h"
#include "Player.h"
#include "ItemList.h"

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
				Logger::Write(Error, "[InventoryController::InventoryToJSON] generation json items error [Equipments:%d]", i);
				continue;
			}

			_items.push_back(item);
		}

		for (int i = 0; i < 64; i++) {
			PItem sourceItem = &this->_character->Inventory[i];

			json::value item = this->ItemToJSON(*sourceItem, i, 1, this->_character->InventoryDB[i]);

			if (item.at("ItemListID").is_null()) {
				Logger::Write(Error, "[InventoryController::InventoryToJSON] generation json items error [Inventory:%d]", i);
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

BYTE InventoryController::GetInventoryMaxSlots(SlotType slotType) {
	BYTE maxSlots = 0;

	switch (slotType)
	{

	case SlotInventory:
		if (this->_player->character.Inventory[60].Index != 0) { maxSlots += 15; }
		if (this->_player->character.Inventory[61].Index != 0) { maxSlots += 15; }
		if (this->_player->character.Inventory[62].Index != 0) { maxSlots += 15; }
		if (this->_player->character.Inventory[63].Index != 0) { maxSlots += 15; }
		break;
	case SlotStorage:
		
		break;

	default:
		break;
	}

	return maxSlots;
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

		if (sourceType == SlotEquipment) {
			if (destItem->Index > 0) {
				if (!destItem->CanEquip() || destItem->GetEquipSlot() != sourceItem->GetEquipSlot()) {
					return false;
				}
			}
		}
		break;

	case SlotEquipment:
		if (destSlot < 1 || destSlot > 15) {
			return false;
		}

		destItem = &this->_character->Equipaments[destSlot];

		if (sourceItem->Index > 0) {
			if (!sourceItem->CanEquip() || sourceItem->GetEquipSlot() != destSlot) {
				return false;
			}
		}

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
	if (source->Index == 0 || destination->Index == 0) {
		return false;
	}

	if (source->Index != destination->Index) {
		return false;
	}

	if (!source->CanAgroup()) {
		return false;
	}

	int maxAmount = ItemList::GetInstance()->Get(source->Index).MaxAmount;

	if (maxAmount == 0) {
		return false;
	}

	if (maxAmount < (source->Amount + destination->Amount)) {
		int Remains = (source->Amount + destination->Amount) - maxAmount;

		destination->Amount = maxAmount;
		source->Amount = Remains;
	}
	else {
		destination->Amount += source->Amount;
		ZeroMemory(source, sizeof(Item));
	}

	return true;
}

bool InventoryController::UngroupItems(PItem source, PItem destination, WORD amount) {
	if (source->Amount < 2 || source->Amount <= amount) {
		return false;
	}

	*destination = *source;

	source->Amount -= amount;
	destination->Amount = amount;

	return true;
}

bool InventoryController::UngroupItems(SlotType slotType, BYTE sourceSlot, WORD amount) {
	auto destinationSlot = this->GetEmptySlot(slotType);

	if (destinationSlot == INVALID_SLOT) {
		this->_player->SendClientMessage("Inventario cheio.");
		return true;
	}

	PItem destinationItem;
	PItem sourceItem;

	switch (slotType)
	{

	case SlotInventory:
		sourceItem = &_player->character.Inventory[sourceSlot];
		destinationItem = &_player->character.Inventory[destinationSlot];
		break;
	case SlotStorage:
		return true;
		break;

	default:
		return true;
		break;
	}

	if (!this->UngroupItems(sourceItem, destinationItem, amount)) {
		return false;
	}

	this->SendRefreshSlot(slotType, sourceSlot, *sourceItem);
	this->SendRefreshSlot(slotType, destinationSlot, *destinationItem);
}

bool InventoryController::DeleteItem(SlotType sourceType, BYTE sourceSlot, bool notify) {
	PItem sourceItem;

	switch (sourceType) {

	case SlotEquipment:
		if (sourceSlot < 2 && sourceSlot > 15) {
			return false;
		}

		sourceItem = &this->_character->Equipaments[sourceSlot];
		break;
	case SlotInventory:
		if (sourceSlot > 59) {
			return false;
		}

		sourceItem = &this->_character->Inventory[sourceSlot];
		break;
	default:
		return true;
		break;
	}

	if (sourceItem == nullptr) {
		return true;
	}

	if (sourceItem->Index == 0) {
		return true;
	}

	if (notify) {
		_player->SendClientMessage(Logger::Format("o item [%s] foi de base.",
			ItemList::GetInstance()->Get(sourceItem->Index).Name));
	}
	

	ZeroMemory(sourceItem, sizeof(Item));
	this->SendRefreshSlot(sourceType, sourceSlot, *sourceItem, false);

	return true;
}


bool InventoryController::SendRefreshSlot(SlotType slotType, BYTE slot, Item item, bool notify) {
	PacketRefreshItem packet;

	ZeroMemory(&packet, sizeof(PacketRefreshItem));

	packet.Header.Size = sizeof(PacketRefreshItem);
	packet.Header.Index = _player->index;
	packet.Header.Code = PacketCode::PacketRefreshItem;

	packet.SlotType = slotType;
	packet.Slot = slot;

	packet.Notice = notify;

	packet.Item = item;

	return _player->SendPacket(&packet, packet.Header.Size);
}


BYTE InventoryController::GetEmptySlot(SlotType slotType) {
	BYTE maxSlots = this->GetInventoryMaxSlots(slotType);

	for (BYTE i = 0; i < maxSlots; i++) {
		switch (slotType) {

		case SlotInventory:
			if (this->_player->character.Inventory[i].Index == 0) {
				return i;
			}
			break;

		case SlotStorage:
			return INVALID_SLOT;
			break;
		}
	}

	return INVALID_SLOT;
}

BYTE InventoryController::GetUsedSlotsCount(SlotType slotType) {
	int usedCount = 0;
	BYTE maxSlots = this->GetInventoryMaxSlots(slotType);

	switch (slotType) {

	case SlotInventory:
		for (BYTE i = 0; i < maxSlots; i++) {
			if (this->_player->character.Inventory[i].Index > 0) {
				usedCount++;
			}
		}

		break;
	case SlotStorage:
		return INVALID_SLOT;
		break;
	}

	return usedCount;
}

BYTE InventoryController::GetAvailableSlotsCount(SlotType slotType) {
	return this->GetInventoryMaxSlots(slotType) - this->GetUsedSlotsCount(slotType);
}


void InventoryController::DecreaseAmount(BYTE slot, WORD amount, SlotType slotType, PItem source) {
	if (source == nullptr) {
		switch (slotType)
		{
		case SlotInventory:
			source = &this->_player->character.Inventory[slot];
			break;
		case SlotStorage:
			return;
			break;
		case SlotPremium:
			return;
			break;
		default:
			return;
			break;
		}
	}

	if (source->Index == 0) {
		Logger::Write(Warnings, 
			"[InventoryController::DecreaseAmount] trying to decrease amount on a null item {slot=%d, amount=%d}", slot, amount);
		return;
	}

	source->Amount -= amount;

	if (source->Amount == 0) {
		this->DeleteItem(slotType, slot, false);
	}
	else {
		this->SendRefreshSlot(slotType, slot, *source);
	}
}

ItemAgroupStatus InventoryController::CanAgroupAmount(const Item item, WORD amount) {
	ItemFromList itemData = ItemList::GetInstance()->Get(item.Index);

	if (!itemData.CanAgroup) {
		return ItemUnagrupable;
	}

	if ((item.Amount + amount) > itemData.MaxAmount) {
		return ItemQuantityExceded;
	}

	return ItemAgrupable;
}


BYTE InventoryController::GetSlotById(const Item item, SlotType slotType, BYTE startSlot) {
	switch (slotType)
	{
	case SlotEquipment:
		for (BYTE i = 0; i < 16; i++) {
			if (_player->character.Equipaments[i].Index == item.Index) {
				return i;
			}
		}
		break;
	case SlotInventory:
		for (BYTE i = 0; i < 64; i++) {
			if (_player->character.Inventory[i].Index == item.Index) {
				return i;
			}
		}
		break;
	case SlotStorage:
		return INVALID_SLOT;
		break;
	case SlotPremium:
		return INVALID_SLOT;
		break;
	default:
		return INVALID_SLOT;
		break;
	}

	return INVALID_SLOT;
}


BYTE InventoryController::PutItem(Item item, BYTE startSlot, bool notify) {
	BYTE slot = 0;
	BYTE result = INVALID_SLOT;

	ItemFromList itemData = ItemList::Get(item.Index);

	if (itemData.Expires && !itemData.CanSealed) {
		item.SetSealed(false);
	}

	if (itemData.CanSealed) {
		item.SetSealed(true);
	}

	BYTE existingSlot = this->GetSlotById(item, SlotInventory, startSlot);

	if (existingSlot == INVALID_SLOT) {
		slot = this->GetEmptySlot();

		if (slot == INVALID_SLOT) {
			_player->SendClientMessage("inventory full!");
			return result;
		}

		_player->character.Inventory[slot] = item;
		this->SendRefreshSlot(SlotInventory, slot, item, notify);
		return slot;
	}

	int maxAmount;
	int remainingAmount;
	PItem existingItem;

	switch (this->CanAgroupAmount(item, item.Amount))
	{

	case ItemUnagrupable:
		slot = this->GetEmptySlot();

		if (slot == INVALID_SLOT) {
			_player->SendClientMessage("inventory full!");
			return result;
		}

		_player->character.Inventory[slot] = item;
		this->SendRefreshSlot(SlotInventory, slot, item, notify);
		break;

	case ItemQuantityExceded:
		existingItem = &_player->character.Inventory[existingSlot];

		maxAmount = itemData.MaxAmount;
		remainingAmount = maxAmount  -existingItem->Amount;

		if (remainingAmount > 0) {
			existingItem->Amount = maxAmount;
			this->SendRefreshSlot(SlotInventory, existingSlot, *existingItem);

			item.Amount = remainingAmount;
			result = this->PutItem(item, existingSlot+1, notify);
		}
		else {
			result = this->PutItem(item, existingSlot+1, notify);
		}

		return result;
		break;

	case ItemAgrupable:
		existingItem = &_player->character.Inventory[existingSlot];

		existingItem->Amount += item.Amount;

		this->SendRefreshSlot(SlotInventory, existingSlot, *existingItem, notify);

		result = existingSlot;
		break;

	default:
		return result;
		break;
	}

	if (result == INVALID_SLOT && slot != INVALID_SLOT) {
		result = slot;
	}

	return result;
}

BYTE InventoryController::PutItem(WORD itemId, WORD amount) {
	Item item = Item();

	ZeroMemory(&item, sizeof(Item));

	item.Index = itemId;
	item.Apparence = itemId;
	item.Amount = amount;
	item.MaxDurability = ItemList::GetInstance()->Get(itemId).Durability;
	item.MinDurability = item.MaxDurability;

	return this->PutItem(item, 0, true);
}