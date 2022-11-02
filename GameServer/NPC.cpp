#include "NPC.h"
#include "Logger.h"
#include "ItemList.h"

#include <sstream>
#include <iostream>

namespace json = boost::json;

bool NPC::Initialize(uint16_t entityId, boost::json::value data) {
	this->index = entityId;

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

	return true;
}

bool NPC::FromJSON(boost::json::value data) {
	ZeroMemory(&this->character, sizeof(Character));
	ZeroMemory(&this->baseCharacter, sizeof(Character));

	ItemList* itemList = ItemList::GetInstance();

	BYTE slot = 0;
	try {
		strcpy_s(this->character.Name, json::value_to<std::string>(data.at("Name")).c_str());
		this->textTitle = json::value_to<std::string>(data.at("Description"));

		this->rotation = json::value_to<int>(data.at("Rotation"));
		this->position.X = json::value_to<float>(data.at("Position").at(0));
		this->position.Y = json::value_to<float>(data.at("Position").at(1));

		slot = 0;
		for (auto& item : data.at("Equip").as_array()) {
			PItem _item = &this->character.Equipaments[slot];
			slot++;

			_item->Index = json::value_to<unsigned int>(item);
			_item->Apparence = _item->Index;

			if (_item->Index == 0) {
				continue;
			}

			if (!itemList->Contains(_item->Index)) {
				continue;
			}

			_item->MaxDurability = itemList->Get(_item->Index).Durability;
			_item->MaxDurability = _item->MinDurability;

			_item->Amount = 1;
		}

		slot = 0;
		for (auto& item : data.at("Inventory").as_array()) {
			PItem _item = &this->character.Inventory[slot];
			slot++;

			_item->Index = (WORD)json::value_to<unsigned int>(item);
			_item->Apparence = _item->Index;

			if (_item->Index == 0) {
				continue;
			}

			if (!itemList->Contains(_item->Index)) {
				continue;
			}

			_item->MaxDurability = itemList->Get(_item->Index).Durability;
			_item->MaxDurability = _item->MinDurability;

			_item->Amount = 1;
		}

		for (auto& option : data.at("options").as_array()) {
			NPCMenuOption optionMenu;

			optionMenu.Index = (uint16_t)json::value_to<int>(option.at("type"));
			optionMenu.Name = json::value_to<std::string>(option.at("text"));

			std::stringstream ss;
			ss << std::hex << json::value_to<std::string>(option.at("color"));
			ss >> optionMenu.Color;

			this->options.insert(std::make_pair(optionMenu.Index, optionMenu));
		}

		this->character.Status.Life.MaxHP = 20000;
		this->character.Status.Life.CurHP = 20000;

		this->character.Status.Life.MaxMP = 20000;
		this->character.Status.Life.CurMP = 20000;

		this->baseCharacter.Status.Sizes.Altura = json::value_to<int>(data.at("Sizes").at("Height"));
		this->baseCharacter.Status.Sizes.Tronco = json::value_to<int>(data.at("Sizes").at("UpperBody"));
		this->baseCharacter.Status.Sizes.Perna = json::value_to<int>(data.at("Sizes").at("Leg"));
		this->baseCharacter.Status.Sizes.Corpo = json::value_to<int>(data.at("Sizes").at("Body"));

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[NPC::FromJSON] NPC=%d error: %s",this->index, e.what());
	}

	return false;
}