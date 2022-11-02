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

	entity->DisplayDamage(packet.Skill, packet.Animation, 100, DamageType::Critical, (PGameEntity)player);

	return true;
}

bool SkillHandler::OnSkillUse(PPlayer player, char* buffer) {
	auto& packet = reinterpret_cast<PacketSkillUse&>(buffer[0]);

	player->visibleController->SendToVisible(&packet, packet.header.Size);

	return true;
}