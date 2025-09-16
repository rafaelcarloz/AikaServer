#include "Mob.h"
#include "Logger.h"
#include "ItemList.h"
#include "Packets.h"

#include <math.h>

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

	this->isDungeonMob = false;
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
		this->baseCharacter.Status.Life.CurHP = this->baseCharacter.Status.Life.MaxHP;
		this->character.Status.Life.CurHP = this->baseCharacter.Status.Life.MaxHP;

		this->baseCharacter.Status.Life.MaxMP = 10;
		this->character.Status.Life.CurMP = 10;

		this->rotation = json::value_to<int>(data.at("Rotation"));
		this->serviceId = json::value_to<int>(data.at("IsService"));

		this->character.Level = json::value_to<int>(data.at("Level"));

		this->baseCharacter.Status.Damage.PhysDamage = this->character.Level * MOB_DAMAGE_PER_LEVEL;

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

		this->_respawnTime = json::value_to<int>(data.at("RespawnTime"));
		this->dropIndex = json::value_to<int>(data.at("DropIndex"));

		this->skills = new std::map<uint16_t, MobSkill>();

		for (auto& item : data.at("Skills").as_array()) {
			auto mobSkill = MobSkill();

			mobSkill.SkillId = json::value_to<int>(item);
			mobSkill.LastUsedTime = time(0);

			this->skills->emplace(mobSkill.SkillId, mobSkill);
		}

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

		this->HandleRespawn();

		return;
	}

	if (this->IsDead()) {
		this->HandleDeath();
	}

	if (difftime(time(0), this->_lastActionTime) < 1) {
		return;
	}

	this->_lastActionTime = time(0);

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

	this->Walk(this->_currentDestination);
}

void Mob::MoveTowards(Position destination, uint16_t speed) {
	Position _moveTo;

	uint16_t moveSpeed = this->statusController.aditionalStatus.MoveSpeed;

	float moveDistance = (float)(moveSpeed / 10);

	if (this->position.X > destination.X) {
		float deltaX = this->position.X - destination.X;

		if (moveDistance > deltaX) {
			_moveTo.X = position.X - deltaX;
		}
		else {
			_moveTo.X = position.X - moveDistance;
		}
	}
	else {
		float deltaX = destination.X - this->position.X;

		if (moveDistance > deltaX) {
			_moveTo.X = position.X + deltaX;
		}
		else {
			_moveTo.X = position.X + moveDistance;
		}
	}

	if (this->position.Y > destination.Y) {
		float deltaY = this->position.Y - destination.Y;

		if (moveDistance > deltaY) {
			_moveTo.Y = position.Y - deltaY;
		}
		else {
			_moveTo.Y = position.Y - moveDistance;
		}
	}
	else {
		float deltaY = destination.Y - this->position.Y;

		if (moveDistance > deltaY) {
			_moveTo.Y = position.Y + deltaY;
		}
		else {
			_moveTo.Y = position.Y + moveDistance;
		}
	}

	this->position = _moveTo;

	this->SendMovemment(_moveTo, packets::MovemmentNormal, moveSpeed);
}

void Mob::Walk(Position destination) {
	Position _moveTo;

	uint16_t moveSpeed = 25;

	float moveDistance = 3.0f;

	if (this->position.X > destination.X) {
		float deltaX = this->position.X - destination.X;

		if (moveDistance > deltaX) {
			_moveTo.X = position.X - deltaX;
		}
		else {
			_moveTo.X = position.X - moveDistance;
		}
	}
	else {
		float deltaX = destination.X - this->position.X;

		if (moveDistance > deltaX) {
			_moveTo.X = position.X + deltaX;
		}
		else {
			_moveTo.X = position.X + moveDistance;
		}
	}

	if (this->position.Y > destination.Y) {
		float deltaY = this->position.Y - destination.Y;

		if (moveDistance > deltaY) {
			_moveTo.Y = position.Y - deltaY;
		}
		else {
			_moveTo.Y = position.Y - moveDistance;
		}
	}
	else {
		float deltaY = destination.Y - this->position.Y;

		if (moveDistance > deltaY) {
			_moveTo.Y = position.Y + deltaY;
		}
		else {
			_moveTo.Y = position.Y + moveDistance;
		}
	}

	this->position = _moveTo;

	this->SendMovemment(_moveTo, packets::MovemmentNormal, moveSpeed);
	this->_lastMovemmentTime = time(0);
}

void Mob::RotateTowards(Position destination) {
	float deltaX = destination.X - this->position.X;
	float deltaY = destination.Y - this->position.Y;

	auto radAngle = atan2(deltaY, deltaX);

	this->rotation = (int)(radAngle * (180/3.1415f));
	this->SendRotation();
}

void Mob::HandleAttack() {
	if (this->IsDead()) {
		return;
	}

	if (this->entityLastAttacker == nullptr) {
		return;
	}

	if (this->position.Distance(this->_currentDestination) > 30) {
		this->_fighting = false;
		this->entityLastAttacker = nullptr;

		return;
	}

	this->_fighting = true;

	Position mobFollowPosition = this->entityLastAttacker->position;

	if (this->entityLastAttacker->position.Distance(this->position) > 1) {
		if (mobFollowPosition.X > this->position.X) {
			mobFollowPosition.X -= 1;
		}

		if (mobFollowPosition.X < this->position.X) {
			mobFollowPosition.X += 1;
		}

		if (mobFollowPosition.Y > this->position.Y) {
			mobFollowPosition.Y -= 1;
		}

		if (mobFollowPosition.Y < this->position.Y) {
			mobFollowPosition.Y += 1;
		}


		this->MoveTowards(mobFollowPosition, 30);
		return;
	}

	if (difftime(time(0), this->timeLastAttack) < 2) {
		return;
	}

	EntityDamage damage;

	damage.DamageValue = this->statusController.volatileStatus.Damage.PhysDamage;
	damage.DamageScope = DamagePhysical;
	damage.DamageType = Normal;

	this->AttackTarget(this->entityLastAttacker, DEFAULT_MOB_ATTACK_SKILL, DEFAULT_MOB_ATTACK_ANIMATION, damage);
}

uint64_t Mob::GetMobBaseExp() {
	return this->baseCharacter.Status.Life.MaxHP;
}

void Mob::HandleDeath() {
	if (difftime(time(0), this->timeLastDeath) <= 3) {
		return;
	}

	this->entityLastAttacker = nullptr;
	this->_fighting = false;
	this->IsIstantiated = false;
}

void Mob::HandleRespawn() {
	if (difftime(time(0), this->timeLastDeath) < this->_respawnTime) {
		return;
	}

	Logger::Write(Debug, "[Entity::%d] Mob Respawned after %d seconds dead", this->index, this->_respawnTime);

	this->position = this->_positionData.Start;
	this->_currentDestination = this->_positionData.Destination;
	this->_awaitTime = this->_positionData.StartWait + ((rand() % 10 + 1));

	this->statusController.RestaureLifePoints();
	this->IsIstantiated = true;

	this->_lastMovemmentTime = time(0);
}