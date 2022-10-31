#include <functional>

#include "VisibleController.h"
#include "ServerInstance.h"
#include "GameEntity.h"

using namespace std::placeholders;

void VisibleController::Initialize(PGameEntity parent) {
	this->_parent = parent;

	this->_isVisible = true;
	this->_visibleEntities = new std::list<uint16_t>();
	this->_rwlock = new std::mutex;

	this->PerformUpdate();
}

void VisibleController::PerformUpdate() {
	if (!this->_parent->IsIstantiated) {
		return;
	}

	this->EvaluateVisibles();

	auto procedureCall = std::bind(&VisibleController::EvaluateEntity, this, _1, _2);
	EntityHandler::ForEach(procedureCall);
}

std::list<uint16_t> VisibleController::GetEntities() {
	std::lock_guard<std::mutex> lock(*_rwlock);

	return *this->_visibleEntities;
}


bool VisibleController::AddToVisible(const WORD entityId) {
	std::lock_guard<std::mutex> lock(*_rwlock);

	if (std::find(_visibleEntities->begin(), _visibleEntities->end(), entityId) == _visibleEntities->end()) {
		this->_visibleEntities->push_back(entityId);
		return true;
	}

	return false;
}

void VisibleController::RemoveFromVisible(const WORD entityId) {
	std::lock_guard<std::mutex> lock(*_rwlock);

	this->_visibleEntities->remove(entityId);
}


void VisibleController::EvaluateEntity(GameEntity* entity, WORD entityId) {
	if (entity->index == this->_parent->index) {
		return;
	}

	if (!entity->IsIstantiated) {
		return;
	}

	if (this->_parent->position.Distance(entity->position) <= 40) {
		if (this->AddToVisible(entityId)) {
			entity->SendCreateMob(SpawnNormal, this->_parent->index);
		}

		if (entity->visibleController->AddToVisible(this->_parent->index)) {
			this->_parent->SendCreateMob(SpawnNormal, entity->index);
		}
	}
}

void VisibleController::EvaluateVisibles() {
	ServerInstance* serverInstance = ServerInstance::GetInstance();

	auto entityList = this->GetEntities();

	for (auto entityId : entityList) {
		PGameEntity entity = EntityHandler::GetEntity(entityId);

		if (entity == NULL) {
			this->RemoveFromVisible(entityId);
			this->_parent->SendUnspawnEntity(entityId);

			continue;
		}

		if (!entity->IsIstantiated) {
			this->RemoveFromVisible(entityId);
			this->_parent->SendUnspawnEntity(entityId);
			continue;
		}

		if (this->_parent->position.Distance(entity->position) > 45) {
			this->RemoveFromVisible(entityId);
			this->_parent->SendUnspawnEntity(entityId);

			entity->visibleController->RemoveFromVisible(this->_parent->index);
			entity->SendUnspawnEntity(this->_parent->index);
		}
	}
}

bool VisibleController::SendToVisible(LPVOID packet, WORD packetSize, bool sendSelf) {
	ServerInstance* serverInstance = ServerInstance::GetInstance();

	if (sendSelf) {
		serverInstance->SendPacketToClient(this->_parent->index, packet, packetSize);
	}

	for (auto entityId : this->GetEntities()) {
		if (EntityHandler::GetEntityType(entityId) != EntityPlayer) {
			continue;
		}

		serverInstance->SendPacketToClient(entityId, packet, packetSize);
	}

	return true;
}

void VisibleController::ForEach(std::function<void(uint16_t, uint16_t)> procedure, uint16_t handleType) {
	for (auto entity : this->GetEntities()) {
		procedure(entity, handleType);
	}
}