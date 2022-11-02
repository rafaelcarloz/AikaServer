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
private:
	
public:
	uint16_t index;

	Position position;
	uint32_t rotation;

	Character baseCharacter;
	Character character;
	
	bool IsIstantiated = false;

	BuffsController* buffsController;
	StatusController statusController;
	VisibleController* visibleController;

	time_t timeLastReceivedAttack;
	time_t timeLastBasicAttack;

	time_t timeLastRegenerationTick;
	time_t timeLastBuffTick;

	std::string textTitle;

	uint16_t entityMobType;
	uint16_t serviceId;

	virtual ~GameEntity() {};

	bool IsDead();
	void DisplayDamage(uint16_t skill, uint16_t animation, uint64_t damage, packets::DamageType damageType, GameEntity* attacker);
	void GetCreateMobPacket(packets::PacketCreateMob& packet, int targetId);
	void SendCreateMob(uint16_t spawnType, int targetId);
	void GetSpawnMobPacket(packets::PacketSpawnMob& packet, int targetId);
	void SendSpawnMob(uint16_t spawnType, int targetId);

	void SendRemoveMob(int targetId, uint16_t deleteType);
	void SendUnspawnEntity(int entityId, DeleteType deleteType = DeleteNormal);

	void SendRotation();
	void SendAction(WORD actionId);
	void SendMovemment(Position position, BYTE moveType, BYTE speed);
	void SendEffect(WORD effectId);
};

#endif
