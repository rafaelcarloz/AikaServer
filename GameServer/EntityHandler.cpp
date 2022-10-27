#include "ServerInstance.h"
#include "Player.h"

std::mutex EntityHandler::_mutex;

bool EntityHandler::AddEntity(GameEntity* entity, EntityType entityType, UINT32* entityId) {
	if (!this->FindFreeEntityId(entityType, entityId)) {
		return false;
	}

	std::lock_guard<std::mutex> lock(_mutex);

	switch (entityType)
	{
	case EntityPlayer:
		playerCount++;
		this->entities.insert(std::pair<UINT32, GameEntity*>(*entityId, entity));
		break;
	case EntityNPC:
		break;
	case EntityMob:

		break;
	default:
		return false;
		break;
	}

	return true;
}

bool EntityHandler::RemoveEntity(const UINT32 entityId) {
	std::lock_guard<std::mutex> lock(_mutex);

	if (this->GetEntityType(entityId) == EntityPlayer) {
		this->playerCount--;
	}

	this->entities.erase(entityId);

	return true;
}

bool EntityHandler::DisconnectPlayers() {
	for (auto& entity : this->GetEntities()) {
		if (this->GetEntityType(entity.first) != EntityPlayer) {
			continue;
		}

		PPlayer player = (PPlayer)entity.second;
		player->Disconnect();
	}

	while (this->playerCount > 0)
	{
		ServerInstance* instance = ServerInstance::GetInstance();
		instance->HandleInactiveConnections();
	}

	return true;
}


bool EntityHandler::FindFreeEntityId(EntityType entityType, UINT32* entityId) {
	*entityId = INVALID_ENTITY;

	std::lock_guard<std::mutex> lock(_mutex);

	switch (entityType)
	{
	case INVALID_ENTITY:
		return false;
		break;
	case EntityPlayer:
		for (UINT32 i = PLAYER_ID_START; i < PLAYER_ID_END; i++) {
			if (this->entities.find(i) == this->entities.end()) {
				*entityId = i;

				return true;
			}
		}

		break;
	case EntityNPC:
		for (UINT32 i = NPC_ID_START; i < NPC_ID_END; i++) {
			if (this->entities.find(i) == this->entities.end()) {
				*entityId = i;

				return true;
			}
		}

		break;
	case EntityMob:
		for (UINT32 i = MOB_ID_START; i < MOB_ID_END; i++) {
			if (this->entities.find(i) == this->entities.end()) {
				*entityId = i;

				return true;
			}
		}

		break;
	default:
		break;
	}

	return false;
}

bool EntityHandler::GetEntity(const UINT32 entityId, PGameEntity& entity) {
	std::lock_guard<std::mutex> lock(_mutex);

	if (this->entities.find(entityId) == this->entities.end()) {
		return false;
	}

	entity = this->entities[entityId];

	return true;
}

PGameEntity& EntityHandler::GetEntity(const UINT32 entityId) {
	PGameEntity entity = NULL;

	ServerInstance* instance = ServerInstance::GetInstance();

	instance->entityHandler->GetEntity(entityId, entity);

	return entity;
}

#pragma region Player

bool EntityHandler::GetPlayerById(const UINT32 entityId, PPlayer& player) {
	std::lock_guard<std::mutex> lock(_mutex);

	if (entityId < PLAYER_ID_START && entityId > PLAYER_ID_END) {
		return false;
	}

	if (this->entities.find(entityId) == this->entities.end()) {
		return false;
	}

	player = (Player*)(this->entities[entityId]);

	return true;
}

#pragma endregion

std::map<UINT32, GameEntity*>& EntityHandler::GetEntities() {
	std::lock_guard<std::mutex> lock(_mutex);

	return this->entities;
}

EntityType EntityHandler::GetEntityType(const UINT32 entityId) {
	if (entityId >= PLAYER_ID_START && entityId <= PLAYER_ID_END) {
		return EntityPlayer;
	}

	if (entityId >= NPC_ID_START && entityId <= NPC_ID_END) {
		return EntityNPC;
	}

	if (entityId >= MOB_ID_START && entityId <= MOB_ID_END) {
		return EntityMob;
	}


	return INVALID_ENTITY;
}

void  EntityHandler::ForEach(std::function<void(GameEntity*, UINT32)> procedure) {
	ServerInstance* instance = ServerInstance::GetInstance();

	for (auto& entity : instance->entityHandler->GetEntities()) {
		procedure(entity.second, entity.first);
	}
}
