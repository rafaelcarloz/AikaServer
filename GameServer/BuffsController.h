#pragma once

#ifndef _BUFFS_CONTROLLER_H_
#define _BUFFS_CONTROLLER_H_

#include <stdio.h>
#include <map>

#include "PlayerData.h"

class BuffsController {
private:
	PCharacter _character;

	uint16_t _entityId;
public:
	void Initialize(uint16_t entityId, PCharacter character);

	void Recalculate();
};

#endif
