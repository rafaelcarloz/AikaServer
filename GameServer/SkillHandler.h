#pragma once

#ifndef _SKILL_HANDLER_H_
#define _SKILL_HANDLER_H_

#include <stdint.h>

#include "GameEntity.fwd.h"

class SkillHandler {
private:
	static bool BasicAttack(PGameEntity attacker, void* packetData, void* target);

	//wizzard skills
	static bool ChaosFlame(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool Change(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool ShockWave(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool BlackShield(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool ChaosInferno(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool ManaStream(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool Pain(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool Return(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);

public:
	static bool OnAttackTarget(PGameEntity attacker, char* buffer);
	static bool OnSkillUse(PGameEntity attacker, char* buffer);
};


#endif