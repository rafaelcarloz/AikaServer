#pragma once

#ifndef _ENTITY_HANDLER_H_
#define _ENTITY_HANDLER_H_

#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <mutex>

#include "GameEntity.h"
#include "Player.fwd.h"

#include <basetsd.h>
namespace {

	const UINT32 PLAYER_ID_START = 1;
	const UINT32 PLAYER_ID_END = 2000;

	const UINT32 NPC_ID_START = 2048;
	const UINT32 NPC_ID_END = 3047;

	const UINT32 MOB_ID_START = 3048;
	const UINT32 MOB_ID_END = 9147;
}

enum EntityType { INVALID_ENTITY = 0, EntityPlayer = 1, EntityNPC = 2048, EntityMob = 3048 };

class EntityHandler {
private:
	std::map<UINT32, GameEntity*> entities;

	static std::mutex _mutex;

	int playerCount = 0;
public:
	
	bool AddEntity(GameEntity* entity, EntityType entityType, UINT32* entityId);
	bool RemoveEntity(const UINT32 entityId);

	bool DisconnectPlayers();
	
	bool FindFreeEntityId(EntityType entityType, UINT32* entityId);

	bool GetEntity(const UINT32 entityId, PGameEntity& entity);
	static PGameEntity& GetEntity(const UINT32 entityId);

	bool GetPlayerById(const UINT32 entityId, PPlayer& player);

	std::map<UINT32, GameEntity*>& GetEntities();

	static EntityType GetEntityType(const UINT32 entityId);
	static void ForEach(std::function<void(GameEntity*, UINT32)> procedure);
};

#endif