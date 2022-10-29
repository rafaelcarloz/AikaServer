#pragma once

#ifndef _ITEM_DATA_H_
#define _ITEM_DATA_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#pragma pack(push,1)

#pragma region "Items Data"


const BYTE INVALID_SLOT = 255;

const WORD HOUR_TO_SEC_MULTIPLYER = 3600;


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
	bool CanAgroup();
	bool CanEquip();

	BYTE GetReinforce();
	BYTE GetEquipSlot();

	time_t GetExpireDate();
	void SetExpireDate(time_t date);

	bool IsSealed();
	void SetSealed(bool seal);

	DWORD GetEquipmentSellPrice();
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

enum ItemUseType : WORD {
	ItemUseFace,
	ItemUseHair,
	ItemUseHead,
	ItemUseArmor,
	ItemUseGloves,
	ItemUseShoes,

	ItemUsePranFood = 26,
	ItemUseCityScroll = 202,
	ItemUseLocationScroll = 204,
	ItemUseRecipe,
	ItemUsePortalScroll = 208,

	ItemUseBagInventory = 217,
	ItemUseBagStorage,
	ItemUseBagPran,

	ItemUseStorageOpen = 226,
	ItemUsePranDigest = 230,

	ItemUseGoldCoin = 234,
	ItemUseCashCoin = 239,

	ItemUseRegisterNation = 300,

	ItemUseAddExpPerc = 404,

	ItemUseHealthPotion = 700,
	ItemUseManaPotion,
	ItemUseBuffPotion,

	ItemUseAddLevel = 704,
	ItemUseRandomChest,

	ItemUseOpenChest = 714,
	ItemUseAddBuff,

	ItemUseRichGoldCoin = 717,

	ItemUseHpMpPotion = 800
};

enum ItemAgroupStatus: BYTE {
	ItemUnagrupable,
	ItemQuantityExceded,
	ItemAgrupable
};

#pragma endregion

#endif