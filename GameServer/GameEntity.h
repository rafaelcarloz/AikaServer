#pragma once

#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include "GameEntity.fwd.h"

#include "MiscData.h"
#include "PlayerData.h"
#include "StatusController.h"
#include "VisibleController.h"
#include "BuffsController.h"
#include "Packets.h"

class GameEntity {
public:
	uint16_t index;

	Position position;
	uint32_t rotation;

	Character baseCharacter;
	Character character;
	
	bool IsIstantiated = false;

	BuffsController buffsController;
	StatusController statusController;
	VisibleController* visibleController;

	virtual ~GameEntity() {};

	bool IsDead();

	void GetCreateMobPacket(packets::PacketCreateMob& packet, int targetId);
	void SendCreateMob(uint16_t spawnType, int targetId);
	void SendSpawnMob(int targetId);

	void SendRemoveMob(int targetId, uint16_t deleteType);
	void SendUnspawnEntity(int entityId, DeleteType deleteType = DeleteNormal);
};

#endif
