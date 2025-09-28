#include "SkillHandler.h"
#include "Packets.h"
#include "Player.h"

#include "EntityHandler.h"
#include "SkillData.h"

using namespace packets;

bool SkillHandler::OnAttackTarget(PGameEntity attacker, char* buffer) {
	auto& packet = reinterpret_cast<PacketAttackTarget&>(buffer[0]);
	PPlayer player = (PPlayer)attacker;


	PGameEntity entity = EntityHandler::GetEntity(packet.Index);

	if (entity == nullptr) {
		return true;
	}

	if (packet.Skill == 0) {
		return SkillHandler::BasicAttack(attacker, &packet, entity);
	}

	auto skillData = SkillData::Get(packet.Skill);

	if (skillData.MagicPointsRequired > attacker->statusController.volatileStatus.Life.CurMP) {
		player->SendClientMessage("not enough mana");
		return true;
	}

	attacker->RemoveMana(skillData.MagicPointsRequired);
	attacker->statusController.SendCurrentLife();

	switch (skillData.Index)
	{

	case SkillFunction::Return:
		SkillHandler::Return(attacker, entity, packet.Skill, packet.Animation);
		break;


	case SkillFunction::Slugger:
		SkillHandler::Slugger(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::ChargeDash:
		SkillHandler::ChargeDash(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::ArmorBreak:
		SkillHandler::ArmorBreak(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::Incite:
		SkillHandler::Incite(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::Berserker:
		SkillHandler::Berserker(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::BladeStrom:
		SkillHandler::BladeStrom(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::HowlOfGlory:
		SkillHandler::HowlOfGlory(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::Stark:
		SkillHandler::Stark(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::Fortitude:
		SkillHandler::Fortitude(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::DragonicBody:
		SkillHandler::DragonicBody(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::HowlOfValor:
		SkillHandler::HowlOfValor(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::ImpactGround:
		SkillHandler::ImpactGround(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::HowlOfWar:
		SkillHandler::HowlOfWar(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::Awakening:
		SkillHandler::Awakening(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::Thrust:
		SkillHandler::Thrust(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::HowlOfDefence:
		SkillHandler::HowlOfDefence(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::DeathWound:
		SkillHandler::DeathWound(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::Halmont:
		SkillHandler::Halmont(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::WeaponGuard:
		SkillHandler::WeaponGuard(attacker, entity, packet.Skill, packet.Animation);
		break;
	case SkillFunction::Revitalize:
		SkillHandler::Revitalize(attacker, entity, packet.Skill, packet.Animation);
		break;


	case SkillFunction::ChaosFlame:
		SkillHandler::ChaosFlame(attacker, entity, packet.Skill, packet.Animation);
		break;

	case SkillFunction::Change:
		SkillHandler::Change(attacker, entity, packet.Skill, packet.Animation);
		break;

	case SkillFunction::ShockWave:
		SkillHandler::ShockWave(attacker, entity, packet.Skill, packet.Animation);
		break;

	case SkillFunction::BlackShield:
		SkillHandler::BlackShield(attacker, entity, packet.Skill, packet.Animation);
		break;

	case SkillFunction::ChaosInferno:
		SkillHandler::ChaosInferno(attacker, entity, packet.Skill, packet.Animation);
		break;

	case SkillFunction::ManaStream:
		SkillHandler::ManaStream(attacker, entity, packet.Skill, packet.Animation);
		break;

	case SkillFunction::Pain:
		SkillHandler::Pain(attacker, entity, packet.Skill, packet.Animation);
		break;


	default:
		break;
	}

	Logger::Write(Debug, "[SkillHandler::OnAttackTarget] Entity {%d} using skill => %d, %d", attacker->index, skillData.Index, packet.Skill);

	return true;
}

bool SkillHandler::OnSkillUse(PGameEntity attacker, char* buffer) {
	auto& packet = reinterpret_cast<PacketSkillUse&>(buffer[0]);

	attacker->visibleController->SendToVisible(&packet, packet.header.Size);

	return true;
}


bool SkillHandler::BasicAttack(PGameEntity attacker, void* packetData, void* target){
	PacketAttackTarget packet = *(PPacket_302)packetData;
	PGameEntity entity = (PGameEntity)target;

	EntityDamage entityDamage;

	entityDamage.DamageValue = attacker->statusController.volatileStatus.Damage.PhysDamage;
	entityDamage.DamageScope = DamagePhysical;
	entityDamage.DamageType = Normal;

	attacker->AttackTarget(entity, packet.Skill, packet.Animation, entityDamage);

	return true;
}

#pragma region "Common"

bool SkillHandler::Return(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->Teleport(3400, 565);

	return true;
}

#pragma endregion
#pragma region "Warrior"

bool SkillHandler::Slugger(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	auto skillData = SkillData::Get(skillId);

	EntityDamage entityDamage;

	entityDamage.DamageValue = attacker->statusController.volatileStatus.Damage.PhysDamage + skillData.Damage;
	entityDamage.DamageScope = DamagePhysical;
	entityDamage.DamageType = Normal;

	attacker->AttackTarget(target, skillId, animation, entityDamage);

	return true;
}

bool SkillHandler::ChargeDash(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	auto skillData = SkillData::Get(skillId);

	EntityDamage entityDamage;

	entityDamage.DamageValue = attacker->statusController.volatileStatus.Damage.PhysDamage + skillData.Damage + skillData.Adicional;
	entityDamage.DamageScope = DamagePhysical;
	entityDamage.DamageType = Normal;

	attacker->AttackTarget(target, skillId, animation, entityDamage);
	return true;
}

bool SkillHandler::ArmorBreak(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	auto skillData = SkillData::Get(skillId);

	EntityDamage entityDamage;

	entityDamage.DamageValue = attacker->statusController.volatileStatus.Damage.PhysDamage + skillData.Damage;
	entityDamage.DamageScope = DamagePhysical;
	entityDamage.DamageType = Normal;

	attacker->AttackTarget(target, skillId, animation, entityDamage);

	return true;
}

bool SkillHandler::Incite(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	return true;
}

bool SkillHandler::Berserker(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	attacker->buffsController->AddBuff(skillId);
	
	return true;
}

bool SkillHandler::BladeStrom(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	return true;
}

bool SkillHandler::HowlOfGlory(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	attacker->buffsController->AddBuff(skillId);

	return true;
}

bool SkillHandler::Stark(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	auto skillData = SkillData::Get(skillId);

	EntityDamage entityDamage;

	entityDamage.DamageValue = attacker->statusController.volatileStatus.Damage.PhysDamage + skillData.Adicional;
	entityDamage.DamageScope = DamagePhysical;
	entityDamage.DamageType = Normal;

	attacker->AttackTarget(target, skillId, animation, entityDamage);

	return true;
}

bool SkillHandler::Fortitude(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	return true;
}

bool SkillHandler::DragonicBody(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	return true;
}

bool SkillHandler::HowlOfValor(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	attacker->buffsController->AddBuff(skillId);
	return true;
}

bool SkillHandler::ImpactGround(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	attacker->buffsController->AddBuff(skillId);
	return true;
}

bool SkillHandler::HowlOfWar(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	attacker->buffsController->AddBuff(skillId);
	return true;
}

bool SkillHandler::Awakening(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	return true;
}

bool SkillHandler::Thrust(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	return true;
}

bool SkillHandler::HowlOfDefence(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	attacker->buffsController->AddBuff(skillId);
	return true;
}

bool SkillHandler::DeathWound(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	return true;
}

bool SkillHandler::Halmont(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	return true;
}

bool SkillHandler::WeaponGuard(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	attacker->buffsController->AddBuff(skillId);

	return true;
}

bool SkillHandler::Revitalize(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	attacker->buffsController->AddBuff(skillId);

	return true;
}

#pragma endregion

#pragma region "Wizzard"

bool SkillHandler::ChaosFlame(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	auto skillData = SkillData::Get(skillId);

	EntityDamage entityDamage;

	entityDamage.DamageValue = attacker->statusController.volatileStatus.Damage.MagDamage + skillData.Damage;
	entityDamage.DamageScope = DamageMagical;
	entityDamage.DamageType = Normal;

	attacker->AttackTarget(target, skillId, animation, entityDamage);

	target->buffsController->AddBuff(skillId);

	return true;
}

bool SkillHandler::Change(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	target->buffsController->AddBuff(skillId);

	target->SendCreateMob(SpawnNormal);

	return true;
}

bool SkillHandler::ShockWave(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	auto skillData = SkillData::Get(skillId);

	EntityDamage entityDamage;

	entityDamage.DamageValue = attacker->statusController.volatileStatus.Damage.MagDamage + skillData.Damage;
	entityDamage.DamageScope = DamageMagical;
	entityDamage.DamageType = Normal;

	attacker->AttackTarget(target, skillId, animation, entityDamage);

	target->buffsController->AddBuff(skillId);
	return true;
}

bool SkillHandler::BlackShield(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	target->buffsController->AddBuff(skillId);
	return true;
}

bool SkillHandler::ChaosInferno(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	auto skillData = SkillData::Get(skillId);

	EntityDamage entityDamage;

	entityDamage.DamageValue = attacker->statusController.volatileStatus.Damage.MagDamage + skillData.Damage;
	entityDamage.DamageScope = DamageMagical;
	entityDamage.DamageType = Normal;

	if (target->statusController.GetEffect(EF_ACCELERATION1) > 0) {
		entityDamage.DamageValue += skillData.Adicional;
	}

	attacker->AttackTarget(target, skillId, animation, entityDamage);
	return true;
}

bool SkillHandler::ManaStream(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	uint16_t recoveryMana = (attacker->statusController.volatileStatus.Life.MaxMP * 0.5);

	if (recoveryMana + attacker->statusController.volatileStatus.Life.CurMP > attacker->statusController.volatileStatus.Life.MaxMP) {
		recoveryMana = attacker->statusController.volatileStatus.Life.MaxMP - attacker->statusController.volatileStatus.Life.CurMP;
	}

	attacker->statusController.volatileStatus.Life.CurMP += recoveryMana;
	attacker->character.Status.Life.CurMP += recoveryMana;

	attacker->statusController.SendCurrentLife(true);

	return true;
}

bool SkillHandler::Pain(PGameEntity attacker, PGameEntity target, uint32_t skillId, uint16_t animation) {
	attacker->AttackTarget(target, skillId, animation, EntityDamage());

	target->buffsController->AddBuff(skillId);

	return true;
}

#pragma endregion
