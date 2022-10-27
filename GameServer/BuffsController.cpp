#include "BuffsController.h"

void BuffsController::Initialize(uint16_t entityId, PCharacter character) {
	this->_character = character;

	this->_entityId = entityId;

	this->Recalculate();
}

void BuffsController::Recalculate() {

}