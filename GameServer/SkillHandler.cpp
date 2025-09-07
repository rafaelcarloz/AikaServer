#include "SkillHandler.h"
#include "Packets.h"
#include "Player.h"

#include "EntityHandler.h"

using namespace packets;

bool SkillHandler::OnAttackTarget(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketAttackTarget&>(buffer[0]);

	PGameEntity entity = EntityHandler::GetEntity(packet.Index);

	if (entity == nullptr) {
		return true;
	}

	if (packet.Skill == 0) {
		return SkillHandler::BasicAttack(player, &packet, entity);
	}

	//entity->OnAttacked(packet.Skill, packet.Animation, 100, DamageType::Critical, (PGameEntity)player);

	return true;
}

bool SkillHandler::OnSkillUse(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketSkillUse&>(buffer[0]);

	player->visibleController->SendToVisible(&packet, packet.header.Size);

	return true;
}


bool SkillHandler::BasicAttack(PPlayer player, void* packetData, void* target){
	PacketAttackTarget packet = *(PPacket_302)packetData;
	PGameEntity entity = (PGameEntity)target;

	uint64_t rawDamage = player->statusController.volatileStatus.Damage.PhysDamage;
	uint64_t dealDamage = entity->CalculateReceivedPhysicalDamage(rawDamage);


	entity->DisplayDamage(packet.Skill, packet.Animation, dealDamage, DamageType::Normal, (PGameEntity)player);

	return true;
}