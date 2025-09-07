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

class Mob: public GameEntity {
private:
	uint16_t _positionId;
	MobPosData _positionData;
	MobPosData _originalPositionData;

	bool _fighting = false;

	time_t _lastMovemmentTime;
	Position _currentDestination;
	uint16_t _awaitTime;

	bool FromJSON(boost::json::value data);

	void HandleMovement();
	void HandleAttack();
	void HandleRespawn();

	void MoveTowards(Position destination);
public:
	std::map<uint16_t, MobDrop> drops;

	bool Initialize(uint16_t entityId, boost::json::value data, uint16_t positionId);

	void PerformUpdate();
};

typedef Mob* PMob;


#endif