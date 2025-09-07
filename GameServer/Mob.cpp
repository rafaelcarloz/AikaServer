#include "Mob.h"
#include "Logger.h"
#include "ItemList.h"
#include "Packets.h"

namespace json = boost::json;

bool Mob::Initialize(uint16_t entityId, boost::json::value data, uint16_t positionId) {
	this->index = entityId;

	this->_positionId = positionId;

	if (!this->FromJSON(data)) {
		return false;
	}

	this->visibleController = new VisibleController();
	this->buffsController = new BuffsController();

	this->visibleController->Initialize(this);
	this->statusController.Initialize(this->index, &this->character, this->baseCharacter);
	this->buffsController->Initialize(this->index, this);

	this->IsIstantiated = true;

	this->timeLastBasicAttack = time(0);
	this->timeLastReceivedAttack = time(0);

	this->timeLastBuffTick = time(0);
	this->timeLastRegenerationTick = time(0);

	this->timeLastDeath = time(0);


	this->_currentDestination = this->_positionData.Destination;
	this->_awaitTime = this->_positionData.StartWait + ((rand() % 10 + 1));

	this->_lastMovemmentTime = time(0);

	return true;
}

bool Mob::FromJSON(boost::json::value data) {
	ZeroMemory(&this->character, sizeof(Character));
	ZeroMemory(&this->baseCharacter, sizeof(Character));

	ItemList* itemList = ItemList::GetInstance();

	BYTE slot = 0;
	try {
		this->character.CharacterId = json::value_to<int>(data.at("Index"));

		PItem faceItem = &this->character.Equipaments[0];

		faceItem->Index = json::value_to<int>(data.at("Face"));
		faceItem->Apparence = faceItem->Index;

		PItem hairItem = &this->character.Equipaments[1];

		hairItem->Index = json::value_to<int>(data.at("Hair"));
		hairItem->Apparence = faceItem->Index;

		PItem weaponItem = &this->character.Equipaments[6];

		weaponItem->Index = json::value_to<int>(data.at("Weapon"));
		weaponItem->Apparence = weaponItem->Index;


		this->baseCharacter.Status.Life.MaxHP = json::value_to<int>(data.at("Health"));
		this->character.Status.Life.CurHP = this->baseCharacter.Status.Life.MaxHP;

		this->baseCharacter.Status.Life.MaxMP = 10;
		this->character.Status.Life.CurMP = 10;

		this->rotation = json::value_to<int>(data.at("Rotation"));
		this->serviceId = json::value_to<int>(data.at("IsService"));

		this->character.Level = json::value_to<int>(data.at("Level"));

		this->baseCharacter.Status.Sizes.Altura = json::value_to<int>(data.at("Height"));
		this->baseCharacter.Status.Sizes.Tronco = json::value_to<int>(data.at("Chest"));
		this->baseCharacter.Status.Sizes.Perna = json::value_to<int>(data.at("Leg"));

		this->entityMobType = json::value_to<int>(data.at("MobType"));

		auto& positionData = data.at("Positions").at(this->_positionId);

		this->_positionData.Start.X = json::value_to<float>(positionData.at("StartPosX"));
		this->_positionData.Start.Y = json::value_to<float>(positionData.at("StartPosY"));
		this->_positionData.StartRange = json::value_to<int>(positionData.at("StartRange"));
		this->_positionData.StartWait = json::value_to<int>(positionData.at("StartWait"));

		this->_positionData.Destination.X = json::value_to<float>(positionData.at("DestPosX"));
		this->_positionData.Destination.Y = json::value_to<float>(positionData.at("DestPosY"));
		this->_positionData.DestinationRange = json::value_to<int>(positionData.at("DestRange"));
		this->_positionData.DestinationWait = json::value_to<int>(positionData.at("DestWait"));

		this->position.X = this->_positionData.Start.X;
		this->position.Y = this->_positionData.Start.Y;

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[Mob::FromJSON] Mob=%d error: %s", this->index, e.what());
	}

	return false;
}


void Mob::PerformUpdate() {
	if (!this->IsIstantiated) {
		if (!this->IsDead()) {
			return;
		}

		void HandleRespawn();

		return;
	}

	if (!this->_fighting) {
		this->HandleMovement();
	}
	
	this->HandleAttack();
}

void Mob::HandleMovement() {
	if (this->IsDead()) {
		return;
	}

	if (difftime(time(0), this->_lastMovemmentTime) <= this->_awaitTime) {
		return;
	}

	if (this->_positionData.Destination.Distance(this->position) <= 1) {
		this->_currentDestination = this->_positionData.Start;
		this->_awaitTime = this->_positionData.StartWait;
	}

	if (this->_positionData.Start.Distance(this->position) <= 1) {
		this->_currentDestination = this->_positionData.Destination;
		this->_awaitTime = this->_positionData.DestinationWait;
	}

	if (difftime(time(0), this->_lastMovemmentTime) < 1) {
		return;
	}

	this->MoveTowards(this->_currentDestination);
}

void Mob::MoveTowards(Position destination) {
	Position _moveTo;

	float moveDistance = 2.2f;

	if (this->position.X > destination.X) {
		_moveTo.X = position.X - moveDistance;
	}
	else {
		_moveTo.X = position.X + moveDistance;
	}

	if (this->position.Y > destination.Y) {
		_moveTo.Y = position.Y - moveDistance;
	}
	else {
		_moveTo.Y = position.Y + moveDistance;
	}

	this->position = _moveTo;

	this->SendMovemment(_moveTo, packets::MovemmentNormal, 25);
	this->_lastMovemmentTime = time(0);
}

void Mob::HandleAttack() {
	if (this->IsDead()) {
		return;
	}
}