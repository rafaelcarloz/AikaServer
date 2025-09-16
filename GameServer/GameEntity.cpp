#include "ServerInstance.h"
#include "GameEntity.h"
#include "Player.h"
#include "SkillData.h"

using namespace packets;

bool GameEntity::IsDead() {
	return (this->statusController.volatileStatus.Life.CurHP == 0);
}


void GameEntity::GetCreateMobPacket(PacketCreateMob& packet, int targetId) {
	ZeroMemory(&packet, sizeof(PacketCreateMob));

	packet.Header.Size = sizeof(PacketCreateMob);
	packet.Header.Index = this->index;
	packet.Header.Code = PacketCode::PacketCreateMob;

	strcpy_s(packet.Name, this->character.Name);

	if (this->textTitle.size() < 32) {
		strcpy_s(packet.Title, this->textTitle.c_str());
	}
	
	packet.ChaosPoint = (BYTE)this->character.Status.ChaosTime;

	packet.CurHP = this->statusController.volatileStatus.Life.CurHP;
	packet.CurMP = this->statusController.volatileStatus.Life.CurMP;
	packet.MaxHP = this->statusController.volatileStatus.Life.MaxHP;
	packet.MaxMP = this->statusController.volatileStatus.Life.MaxMP;

	packet.CurrentPosition = this->position;
	packet.Rotation = this->rotation;

	for (int i = 0; i < 8; i++) {
		packet.Equipaments[i] = this->character.Equipaments[i].Apparence;
	}

	packet.ItemEffects[7] = this->character.Equipaments[6].GetReinforce();

	packet.GuildIndex = (BYTE)this->character.GuildIndex;

	
	packet.SpeedMove = (BYTE)this->statusController.aditionalStatus.MoveSpeed;

	packet.SetBuff = this->character.Equipaments[2].Index;

	packet.Sizes = this->statusController.volatileStatus.Sizes;

	if (EntityHandler::GetEntityType(this->index) == EntityPlayer) {
		auto player = (PPlayer)this;

		packet.Effects[1] = 0x1D;
		packet.UNK_1 = 0x0A;

		packet.Nation = (Citizenship)((BYTE)player->account->accountNation * 16);

		if (player->characterController->activeTitle != nullptr) {
			packet.ActiveTitle.Index = player->characterController->activeTitle->Index;
			packet.ActiveTitle.Level = player->characterController->activeTitle->Level - 1;
		}
	}
	
	packet.IsService = (EntityHandler::GetEntityType(this->index) == EntityNPC);
}

void GameEntity::SendCreateMob(uint16_t spawnType, int targetId) {
	if (EntityHandler::GetEntityType(targetId) != EntityPlayer) {
		return;
	}

	ServerInstance* serverInstance = ServerInstance::GetInstance();

	if (EntityHandler::GetEntityType(this->index) == EntityMob) {
		this->SendSpawnMob(spawnType, targetId);
	}
	else {
		PacketCreateMob packet;
		this->GetCreateMobPacket(packet, targetId);
		packet.SpawnType = (BYTE)spawnType;

		serverInstance->SendPacketToClient(targetId, &packet, packet.Header.Size);
	}
}


void GameEntity::GetSpawnMobPacket(PacketSpawnMob& packet, int targetId) {
	ZeroMemory(&packet, sizeof(PacketSpawnMob));

	packet.Header.Size = sizeof(PacketSpawnMob);
	packet.Header.Index = this->index;
	packet.Header.Code = PacketCode::PacketSpawnMob;

	packet.MobName = this->character.CharacterId;

	packet.CurHP = this->statusController.volatileStatus.Life.CurHP;
	packet.CurMP = this->statusController.volatileStatus.Life.CurMP;
	packet.MaxHP = this->statusController.volatileStatus.Life.MaxHP;
	packet.MaxMP = this->statusController.volatileStatus.Life.MaxMP;

	packet.Level = this->character.Level;

	packet.CurrentPosition = this->position;
	packet.Rotation = this->rotation;

	for (int i = 0; i < 8; i++) {
		packet.Equipaments[i] = this->character.Equipaments[i].Apparence;
	}

	packet.Sizes = this->statusController.volatileStatus.Sizes;

	packet.MobType = this->entityMobType;
	packet.IsService = this->serviceId;
}


void GameEntity::SendSpawnMob(uint16_t spawnType, int targetId) {
	if (EntityHandler::GetEntityType(targetId) != EntityPlayer) {
		return;
	}

	ServerInstance* serverInstance = ServerInstance::GetInstance();

	PacketSpawnMob packet;

	this->GetSpawnMobPacket(packet, targetId);

	packet.spawnType = (SpawnType)spawnType;

	serverInstance->SendPacketToClient(targetId, &packet, packet.Header.Size);
}


void GameEntity::SendRemoveMob(int targetId, uint16_t deleteType) {
	if (EntityHandler::GetEntityType(targetId) != EntityPlayer) {
		return;
	}

	ServerInstance* serverInstance = ServerInstance::GetInstance();

	PacketRemoveMob packet;

	ZeroMemory(&packet, sizeof(packet));

	packet.Header.Size = sizeof(packet);
	packet.Header.Index = targetId;
	packet.Header.Code = PacketCode::PacketRemoveMob;

	packet.DeleteType = deleteType;
	packet.Index = this->index;

	serverInstance->SendPacketToClient(targetId, &packet, packet.Header.Size);
}

void GameEntity::SendUnspawnEntity(int entityId, DeleteType deleteType) {
	ServerInstance* serverInstance = ServerInstance::GetInstance();

	PacketRemoveMob packet;

	ZeroMemory(&packet, sizeof(packet));

	packet.Header.Size = sizeof(packet);
	packet.Header.Index = this->index;
	packet.Header.Code = PacketCode::PacketRemoveMob;

	packet.DeleteType = deleteType;
	packet.Index = entityId;

	serverInstance->SendPacketToClient(this->index, &packet, packet.Header.Size);
}

void GameEntity::SendRotation() {
	PacketSignalData packet;

	ZeroMemory(&packet, sizeof(PacketSignalData));

	packet.Header.Size = sizeof(PacketSignalData);
	packet.Header.Index = this->index;
	packet.Header.Code = PacketCode::PacketCharacterRotation;

	packet.Data = this->rotation;

	this->visibleController->SendToVisible(&packet, packet.Header.Size, false);
}

void GameEntity::SendAction(WORD actionId) {
	PacketCharacterAction packet;

	ZeroMemory(&packet, sizeof(PacketCharacterAction));

	packet.Header.Size = sizeof(PacketCharacterAction);
	packet.Header.Index = this->index;
	packet.Header.Code = PacketCode::PacketCharacterAction;

	packet.actionId = actionId;

	this->visibleController->SendToVisible(&packet, packet.Header.Size, false);
}

void GameEntity::SendMovemment(Position position, BYTE moveType, BYTE speed) {
	PacketMovemment packet;

	ZeroMemory(&packet, sizeof(PacketMovemment));

	packet.Header.Size = sizeof(PacketMovemment);
	packet.Header.Index = this->index;
	packet.Header.Code = PacketCode::PacketMovemment;

	packet.Destination = position;
	packet.MoveType = (MovemmentType)moveType;
	packet.Speed = speed;

	this->visibleController->SendToVisible(&packet, packet.Header.Size, false);
}

void GameEntity::SendEffect(WORD effectId) {
	PacketClientIndex packet;

	ServerInstance* serverInstance = ServerInstance::GetInstance();

	ZeroMemory(&packet, sizeof(PacketClientIndex));

	packet.Header.Size = sizeof(PacketClientIndex);
	packet.Header.Index = this->index;
	packet.Header.Code = PacketCode::PacketClientIndex;

	packet.Index = this->index;
	packet.Effect = effectId;

	serverInstance->SendPacketToClient(this->index, &packet, packet.Header.Size);
}



#pragma region "Damage"

uint64_t GameEntity::CalculateReceivedPhysicalDamage(uint64_t damage) {
	WORD rawDamageReduction = (this->statusController.volatileStatus.Damage.PhysDefense / 8);

	if (damage == 0) {
		return damage;
	}

	if (rawDamageReduction > damage) {
		return MIN_ENTITY_DAMAGE;
	}

	uint64_t resultDamage = (damage - rawDamageReduction);

	return resultDamage;  
}

uint64_t GameEntity::CalculateReceivedMagicalDamage(uint64_t damage) {
	WORD rawDamageReduction = (this->statusController.volatileStatus.Damage.MagDefense / 8);

	if (damage == 0) {
		return damage;
	}

	if (rawDamageReduction > damage) {
		return MIN_ENTITY_DAMAGE;
	}

	uint64_t resultDamage = (damage - rawDamageReduction);

	return resultDamage;
}

void GameEntity::DisplayDamage(uint16_t skill, uint16_t attackAnimation, uint64_t damage, DamageType damageType, GameEntity* attacker) {
	PacketDisplayDamage packet;

	ZeroMemory(&packet, sizeof(PacketDisplayDamage));

	auto skillItem = SkillData::Get(skill);

	packet.header.Size = sizeof(PacketDisplayDamage);
	packet.header.Index = this->index;
	packet.header.Code = PacketCode::PacketDisplayDamage;

	packet.animation = attackAnimation;
	packet.skill = skill;
	packet.damage = damage;
	packet.damageType = damageType;

	if (attacker != nullptr) {
		packet.attackerId = attacker->index;
		packet.attackerHp = attacker->statusController.volatileStatus.Life.CurHP;
		packet.attackerPosition = attacker->position;
	}

	packet.targetId = this->index;

	packet.targetHp = this->statusController.volatileStatus.Life.CurHP;

	if (this->IsDead()) {
		packet.deathPosition = this->position;
	}

	packet.mobAnimation = skillItem.TargetAnimation;

	this->visibleController->SendToVisible(&packet, packet.header.Size);
}

void GameEntity::AttackTarget(GameEntity* target, uint16_t skill, uint16_t animation, EntityDamage entityDamage) {
	this->entityLastTarget = target;
	this->timeLastAttack = time(0);

	if (target->IsDead()) {
		return;
	}

	if (skill == 0) {
		this->timeLastAttack = time(0);
		this->timeLastBasicAttack = time(0);
	}

	target->OnGetAttacked(this, skill, animation, entityDamage);
}

void GameEntity::OnGetAttacked(GameEntity* attacker, uint16_t skill, uint16_t attackAnimation, EntityDamage entityDamage){
	this->timeLastReceivedAttack = time(0);
	this->entityLastAttacker = attacker;

	uint64_t damage = 0;

	if (entityDamage.DamageScope == DamagePhysical) {
		damage = this->CalculateReceivedPhysicalDamage(entityDamage.DamageValue);
	}
	else {
		damage = this->CalculateReceivedMagicalDamage(entityDamage.DamageValue);
	}

	this->DealDamage(damage);

	Logger::Write(Debug, "[Entity::%d] Attacking Entity %d with damage %d, remaining health: %d", attacker->index, this->index, damage, this->statusController.volatileStatus.Life.CurHP);

	this->DisplayDamage(skill, attackAnimation, damage, entityDamage.DamageType, attacker);
}

void GameEntity::DealDamage(uint64_t damage) {
	if (damage >= this->statusController.volatileStatus.Life.CurHP) {
		this->statusController.volatileStatus.Life.CurHP = 0;
		this->OnDie(this->entityLastAttacker);
	}
	else {
		this->statusController.volatileStatus.Life.CurHP -= damage;
	}

	this->character.Status.Life.CurHP = this->statusController.volatileStatus.Life.CurHP;
}

void GameEntity::RemoveMana(uint32_t damage) {
	if (damage >= this->character.Status.Life.CurMP) {
		this->character.Status.Life.CurMP = 0;
	}
	else {
		this->character.Status.Life.CurMP -= damage;
	}

	this->statusController.volatileStatus.Life.CurMP = this->character.Status.Life.CurMP;
}


#pragma endregion

void GameEntity::Teleport(float positionX, float positionY) {
	this->Teleport(Position(positionX, positionY));
}

void GameEntity::Teleport(Position position) {
	this->position = position;

	this->SendMovemment(position, MovemmentType::MovemmentTeleport, 70);
	this->SendCreateMob(SpawnType::SpawnTeleport, this->index);
}

void GameEntity::OnDie(GameEntity* killer) {
	this->timeLastDeath = time(0);

	Logger::Write(Debug, "[Entity::%d] killed by Entity %d", this->index, killer->index);

	if (EntityHandler::GetEntityType(killer->index) == EntityPlayer) {
		PPlayer player = (PPlayer)killer;

		if (EntityHandler::GetEntityType(this->index) == EntityMob) {
			player->characterController->OnKillMob(this);
		}
	}
}
