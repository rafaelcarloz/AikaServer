#pragma once

#ifndef _NPC_H_
#define _NPC_H_

#include <stdio.h>
#include <map>

#include <boost/json.hpp>

#include "GameEntity.h"

enum NpcUsingOption: BYTE {
	NpcOptionOpen,
	NpcOptionTalk,
	NpcOptionQuest,

	NpcOptionEnterCastle = 4,
	NpcOptionStore,
	NpcOptionSkills,
	NpcOptionStorage,
	NpcOptionClose,
	NpcOptionSignCastle,
	NpcOptionGuildCreate,
	NpcOptionGuildStorage,

	NpcOptionPranStation = 13,

	NpcOptionAnvil = 15,
	NpcOptionReinforce,
	NpcOptionEnchant,
	NpcOptionTransformApp,
	NpcOptionLeveling,
	NpcOptionTimat,
	NpcOptionMenu,
	NpcOptionTakeQuest,

	NpcOptionDeliveryQuest = 24,
	NpcOptionSaveLocation,
	NpcOptionDungeon,

	NpcOptionRepair = 31,
	NpcOptionRepairAll,
	NpcOptionRecycle,

	NpcOptionInitialBuff = 35,

	NpcOptionEnchantMount = 44,

	NpcOptionAuction = 48,

	NpcOptionEnchantPran = 62
};

enum NpcDialog : BYTE {
	NpcDialogNone,
	NpcDialogStorage,
	NpcDialogPranStation,

	NpcDialogAnvil = 4,
	NpcDialogReinforce,
	NpcDialogEnchant,
	NpcDialogTransformApp,
	NpcDialogLeveling,
	NpcDialogTimat,

	NpcDialogUpgradeCastle = 13,
	NpcDialogRepair,
	NpcDialogRepairAll,

	NpcDialogRecycle = 17,
	NpcDialogAuction2,
	NpcDialogJoinCastle,

	NpcDialogAggros = 21,

	NpcDialogEnchantMount = 23,
	NpcDialogSkillMount,

	NpcDialogAuction = 26,

	NpcDialogPranJoquempo = 33,

	NpcDialogEnchantPran = 35,

	NpcDialogStoneEvolution = 38
};


enum NpcShopType {
	NpcShoSkills = 0x0B,
	NpcShopItems
};

struct NPCMenuOption {
	uint16_t Index;
	std::string Name;
	DWORD Color;
};

class NPC : public GameEntity {
private:
	bool FromJSON(boost::json::value data);
public:
	std::map<uint16_t, NPCMenuOption> options;

	bool Initialize(uint16_t entityId, boost::json::value data);
};

typedef NPC* PNpc;

#endif