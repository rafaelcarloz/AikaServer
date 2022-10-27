#pragma once

#ifndef _ITEM_DATA_H_
#define _ITEM_DATA_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#pragma pack(push,1)

#pragma region "Items Data"



enum ItemRarityType : BYTE {
	ITEM_RARITY_NORMAL,
	ITEM_RARITY_RARE_SUPERIOR,
	ITEM_RARITY_RARE = 3,
	ITEM_RARITY_SUPERIOR = 5
};


typedef struct ItemEffect {
	BYTE Index[3];
	BYTE Value[3];
}*PItemEffect;


typedef struct Item {
	WORD Index;
	WORD Apparence;
	DWORD Identific;
	ItemEffect Effects;
	BYTE MinDurability;
	BYTE MaxDurability;
	WORD Amount;
	WORD Time;
public:
	BYTE GetReinforce();
	bool CanAgroup();
} *PItem;


typedef struct ItemCash {
	WORD Index;
	WORD Apparence;
	DWORD Identific;

public:
	Item ToItem();
}*PItemCash;

#pragma endregion
#pragma region "Inventory Data"

enum SlotType : WORD {
	SlotEquipment,
	SlotInventory,
	SlotStorage,
	SlotPremium = 10
};

enum ITEM_GROUP_RESPONSE : BYTE {
	GROUP_NOT_ALLOWED,
	GROUP_AMOUNT_LIMIT,
	GROUP_ALLOWED
};

#pragma endregion

#endif