#pragma once

#ifndef _SKILL_HANDLER_H_
#define _SKILL_HANDLER_H_

#include <stdio.h>

#include "Player.fwd.h"

class SkillHandler {
private:
	static bool BasicAttack(PPlayer player, void* packetData, void* target);
public:
	static bool OnAttackTarget(PPlayer player, char* buffer);
	static bool OnSkillUse(PPlayer player, char* buffer);
};


#endif