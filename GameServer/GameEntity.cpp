#include "ServerInstance.h"
#include "GameEntity.h"
#include "Player.h"

using namespace packets;

bool GameEntity::IsDead() {
	return (this->statusController.volatileStatus.Life.CurHP == 0);
}


void GameEntity::GetCreateMobPacket(PacketCreateMob& packet, int targetId) {
	ZeroMemory(&packet, sizeof(packet));

	packet.Header.Size = sizeof(packet);
	packet.Header.Index = this->index;
	packet.Header.Code = 0x349;

	strcpy_s(packet.Name, this->character.Name);

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

	packet.UNK_1 = 0x0A;
	packet.SpeedMove = (BYTE)this->statusController.aditionalStatus.MoveSpeed;

	packet.SetBuff = this->character.Equipaments[2].Index;

	packet.Sizes = this->statusController.volatileStatus.Sizes;

	if (EntityHandler::GetEntityType(this->index) == EntityPlayer) {
		auto player = (PPlayer)this;

		packet.Nation = (Citizenship)((BYTE)player->account->accountNation * 16);

		packet.ActiveTitle.Index = player->characterController->activeTitle.Index;
		packet.ActiveTitle.Level = player->characterController->activeTitle.Level - 1;
	}
}

void GameEntity::SendCreateMob(uint16_t spawnType, int targetId) {
	if (EntityHandler::GetEntityType(targetId) != EntityPlayer) {
		return;
	}

	ServerInstance* serverInstance = ServerInstance::GetInstance();

	PacketCreateMob packet;

	this->GetCreateMobPacket(packet, targetId);

	packet.SpawnType = (BYTE)spawnType;

	serverInstance->SendPacketToClient(targetId, &packet, packet.Header.Size);
}

void GameEntity::SendSpawnMob(int targetId) {
	this->SendCreateMob(SpawnNormal, targetId);
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
	packet.Header.Code = 0x101;

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
	packet.Header.Code = 0x101;

	packet.DeleteType = deleteType;
	packet.Index = entityId;

	serverInstance->SendPacketToClient(this->index, &packet, packet.Header.Size);
}