#pragma once

#ifndef _ITEM_LIST_H_
#define _ITEM_LIST_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdio.h>
#include <map>

#include "ItemData.h"

#pragma region "ItemList Data"

enum ItemTradeType : BYTE
{
	NORMAL_TRADING,
	NOT_TRADING,
	REVERSE_TRADING
};

typedef struct ItemFromList {
	char Name[64];
	char NameEnglish[64];
	char Descrition[128]; // Description
	bool CanAgroup;
	bool Unknow;
	WORD ItemType;
	BYTE UnkValues[8];
	WORD UseEffect;
	BYTE Unk_other[6];
	DWORD DelayUse, // delay ao usar item consumivel
		PriceHonor, // valor
		PriceMedal, // valor
		PriceGold, // valor
		SellPrince,
		Null_0;
	WORD CharacterClass; // dword
	BYTE Null_1[6];
	WORD MeshID;
	bool Null_00;
	DWORD BonusEquip;
	BYTE Null_3[5];
	WORD IconID,
		MaxAmount,
		SetItemId;
	WORD Null_4;
	bool Expires; // 
	BYTE Unk_Bool;
	WORD Level;
	DWORD Null_001;
	DWORD Duration;
	BYTE Nothing_2[18];
	WORD pDamage,
		pDefense,
		mDamage,
		mDefense;
	BYTE Null0[6];
	WORD HP,
		MP; //
	BYTE Null001[14];
	BYTE RarityType; // Se  raro...normal....  [0~~7]
	WORD Null01;
	ItemTradeType TypeTrade;
	BYTE Null02[12];
	BYTE Durability;
	BYTE Null_5[5];
	WORD EF[3], EFV[3];
	bool AlreadyChanged, // Mudança de aparencia
		AlreadyReduced, // se ja foi reduzido
		AlreadyFortificated; //
	BYTE Rank;
	DWORD Null,
		MaxLvl,
		Null3;
	WORD TypePriceItem; // Quando o item é vendido por outros itens
	WORD TypePriceItemValue;
	BYTE Null4[16];
	WORD CanSealed;
	WORD Null5;

	BYTE GetEquipSlot();
}*PItemFromList;

#pragma endregion

class ItemList {
private:
	static ItemList* _instance;

	std::map<WORD, ItemFromList> _items;

	WORD _itemCount = 0;
protected:
	ItemList() {}
	~ItemList() {}
public:
	ItemList(ItemList& other) = delete;
	void operator=(const ItemList&) = delete;

	static ItemList* GetInstance();

	bool Add(const ItemFromList item);
	bool Get(const WORD itemId, ItemFromList* item);
	ItemFromList Get(const WORD itemId);

	ItemFromList operator[](const WORD itemId);
};

#endif
