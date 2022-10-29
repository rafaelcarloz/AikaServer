#pragma once

#ifndef _INVENTORY_CONTROLLER_H_
#define _INVENTORY_CONTROLLER_H_

#include <stdio.h>

#include <boost/json.hpp>

#include "PlayerData.h"
#include "Player.fwd.h"

enum : BYTE {
	Bag1 = 60,
	Bag2,
	Bag3,
	BagPremium
};


class InventoryController {
private:
	PCharacter _character;
	PPlayer _player;

	bool GetEquipmentsData();
	bool GetInventoryData();

	Item ItemFromJSON(boost::json::value item);
	boost::json::value ItemToJSON(Item item, BYTE slot, BYTE slotType, uint64_t databaseId = 0);

	PItem GetInventoryBagFromSlot(BYTE slot);
	BYTE GetInventoryMaxSlots(SlotType slotType = SlotInventory);
public:
	bool Initialize(PCharacter _character, PPlayer _player);
	boost::json::value InventoryToJSON();

	bool MoveItem(SlotType sourceType, BYTE sourceSlot, SlotType destType, BYTE destSlot);
	bool SwapItems(PItem source, PItem destination);

	bool GroupItems(PItem source, PItem destination);
	bool UngroupItems(PItem source, PItem destination, WORD amount);
	bool UngroupItems(SlotType slotType, BYTE sourceSlot, WORD amount);

	bool DeleteItem(SlotType sourceType, BYTE sourceSlot, bool notify = true);

	bool SendRefreshSlot(SlotType slotType, BYTE slot, Item item, bool notify = false);

	BYTE GetEmptySlot(SlotType slotType = SlotInventory);
	BYTE GetUsedSlotsCount(SlotType slotType = SlotInventory);
	BYTE GetAvailableSlotsCount(SlotType slotType = SlotInventory);

	void DecreaseAmount(BYTE slot, WORD amount, SlotType slotType = SlotInventory, PItem source = nullptr);


	ItemAgroupStatus CanAgroupAmount(const Item item, WORD amount);

	BYTE GetSlotById(const Item item, SlotType slotType, BYTE startSlot = 0);

	BYTE PutItem(Item item, BYTE startSlot = 0, bool notify = false);
	BYTE PutItem(WORD itemId, WORD amount = 1);
};

#endif
