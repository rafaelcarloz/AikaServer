#include "ServerRoutines.h"
#include "Player.h"
#include "Mob.h"


#pragma region "Server Threads"

bool ServerRoutines::StartRoutines() {
	this->serverInstance = ServerInstance::GetInstance();

	this->_threadUpdatedVisible = std::thread(&ServerRoutines::RoutineUpdateVisibleEntities, this);
	this->_threadUpdatedVisible.detach();

	this->_threadUpdateBuffsAndStatus = std::thread(&ServerRoutines::RoutineUpdateBuffsAndStatus, this);
	this->_threadUpdateBuffsAndStatus.detach();

	this->_threadMobActions = std::thread(&ServerRoutines::RoutineUpdateMobActions, this);
	this->_threadMobActions.detach();

	this->_threadUpdateQuests = std::thread(&ServerRoutines::RoutineUpdateQuests, this);
	this->_threadUpdateQuests.detach();

	return true;
}


void ServerRoutines::RoutineUpdateVisibleEntities() {
	while (this->serverInstance->isActive)
	{
		auto entities = this->serverInstance->entityHandler->GetEntities();

		for (auto& entity : entities) {
			if (EntityHandler::GetEntityType(entity.first) != EntityPlayer) {
				continue;
			}

			PPlayer player;

			if (!this->serverInstance->entityHandler->GetPlayerById(entity.first, player)) {
				Logger::Write(Error, "[ServerInstance::RoutineUpdateVisibleEntities] get player error");
				continue;
			}

			if (player->IsIstantiated != true || player->index <= 0) {
				continue;
			}

			player->visibleController->PerformUpdate();
		}

		Sleep(10);
	}
}

void ServerRoutines::RoutineUpdateBuffsAndStatus() {
	while (this->serverInstance->isActive)
	{
		auto entities = this->serverInstance->entityHandler->GetEntities();

		for (auto& entity : entities) {
			if (!entity.second->IsIstantiated) {
				continue;
			}

			entity.second->buffsController->Recalculate();

			entity.second->statusController.DoRegenerationTick();
			entity.second->statusController.DoDotTick();
		}

		Sleep(100);
	}
}

void ServerRoutines::RoutineUpdateMobActions() {
	while (this->serverInstance->isActive)
	{
		auto entities = this->serverInstance->entityHandler->GetEntities();

		for (auto& entity : entities) {
			if (EntityHandler::GetEntityType(entity.first) != EntityMob) {
				continue;
			}

			PMob mob = (PMob)this->serverInstance->entityHandler->GetEntity(entity.first);

			if (mob->index <= 0) {
				continue;
			}

			mob->PerformUpdate();
		}

		Sleep(100);
	}
}

void ServerRoutines::RoutineUpdateQuests() {
	while (this->serverInstance->isActive)
	{
		auto entities = this->serverInstance->entityHandler->GetEntities();

		for (auto& entity : entities) {
			if (EntityHandler::GetEntityType(entity.first) != EntityPlayer) {
				continue;
			}

			PPlayer player;

			if (!this->serverInstance->entityHandler->GetPlayerById(entity.first, player)) {
				Logger::Write(Error, "[%s] get player error", __FUNCTION__);
				continue;
			}

			if (player->IsIstantiated != true || player->index <= 0) {
				continue;
			}

			player->questsController->Evaluate();
		}

		Sleep(2000);
	}
}

#pragma endregion