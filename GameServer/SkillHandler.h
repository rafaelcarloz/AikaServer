#pragma once

#ifndef _SKILL_HANDLER_H_
#define _SKILL_HANDLER_H_

#include <stdint.h>

#include "GameEntity.fwd.h"

class SkillHandler {
private:
	static bool BasicAttack(PGameEntity attacker, void* packetData, void* target);

	//common
	static bool Return(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);

	//warrior
	static bool Slugger(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	ChargeDash(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	ArmorBreak(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	Incite(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	Berserker(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	BladeStrom(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	HowlOfGlory(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	Stark(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	Fortitude(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	DragonicBody(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	HowlOfValor(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	ImpactGround(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	HowlOfWar(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	Awakening(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	Thrust(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	HowlOfDefence(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	DeathWound(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	Halmont(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	WeaponGuard(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool	Revitalize(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);

	//wizzard skills
	static bool ChaosFlame(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool Change(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool ShockWave(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool BlackShield(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool ChaosInferno(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool ManaStream(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	static bool Pain(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation);
	

public:
	static bool OnAttackTarget(PGameEntity attacker, char* buffer);
	static bool OnSkillUse(PGameEntity attacker, char* buffer);
};


#endif