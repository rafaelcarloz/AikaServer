#pragma once

#ifndef _MOB_H_
#define _MOB_H_

#include <stdio.h>
#include <map>

#include <boost/json.hpp>

#include "GameEntity.h"

struct MobDrop {
	uint16_t Index;
	uint16_t Rate;
};

struct MobPosData {
	Position Start;
	uint16_t StartRange;
	uint16_t StartWait;

	Position Destination;
	uint16_t DestinationRange;
	uint16_t DestinationWait;
};


struct MobSkill {
	uint16_t SkillId;
	time_t LastUsedTime;
};

class Mob: public GameEntity {
private:
	uint16_t _positionId;
	MobPosData _positionData;
	MobPosData _originalPositionData;

	bool _fighting = false;

	time_t _lastMovemmentTime;
	Position _currentDestination;
	uint16_t _awaitTime;

	uint16_t _respawnTime;

	time_t _lastActionTime;

	const uint16_t MOB_DAMAGE_PER_LEVEL = 10;
	const uint16_t DEFAULT_MOB_ATTACK_SKILL = 0;
	const uint16_t DEFAULT_MOB_ATTACK_ANIMATION = 6;

	bool FromJSON(boost::json::value data);

	void HandleMovement();
	void HandleAttack();
	void HandleRespawn();
	void HandleDeath();

	void MoveTowards(Position destination, uint16_t speed = 25);
	void Walk(Position destination);
	void RotateTowards(Position destination);
public:
	bool isDungeonMob;
	uint16_t mobElevation;

	uint16_t dropIndex;
	std::map<uint16_t, MobDrop> drops;
	std::map<uint16_t, MobSkill>* skills;

	uint64_t GetMobBaseExp();

	bool Initialize(uint16_t entityId, boost::json::value data, uint16_t positionId);

	void PerformUpdate();
};

typedef Mob* PMob;


#endif