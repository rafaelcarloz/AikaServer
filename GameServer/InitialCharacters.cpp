#include <iostream>

#include "InitialCharacters.h"
#include "ItemList.h"
#include "Logger.h"

InitialCharacters* InitialCharacters::_instance{ nullptr };

namespace json = boost::json;

InitialCharacters* InitialCharacters::GetInstance() {
	if (_instance == nullptr)
	{
		_instance = new InitialCharacters();
	}
	return _instance;
}

Character InitialCharacters::FromJSON(boost::json::value characterData) {
	Character character = Character();
	ZeroMemory(&character, sizeof(character));
	
	character.ClassInfo = (CharacterClass)json::value_to<int>(characterData.at("class_info"));
	character.Level = json::value_to<int>(characterData.at("level"));

	character.Status.Attributes = {
		(WORD)json::value_to<int>(characterData.at("status").at("str")),
		(WORD)json::value_to<int>(characterData.at("status").at("dex")),
		(WORD)json::value_to<int>(characterData.at("status").at("int")),
		(WORD)json::value_to<int>(characterData.at("status").at("const")),
		(WORD)json::value_to<int>(characterData.at("status").at("spirit")),
		(WORD)json::value_to<int>(characterData.at("status").at("stats"))
	};

	character.Status.Sizes = {
		(BYTE)json::value_to<int>(characterData.at("sizes").at("altura")),
		(BYTE)json::value_to<int>(characterData.at("sizes").at("tronco")),
		(BYTE)json::value_to<int>(characterData.at("sizes").at("perna")),
		(BYTE)json::value_to<int>(characterData.at("sizes").at("corpo")),
	};

	character.Gold = json::value_to<int>(characterData.at("gold"));

	ItemList* itemList = ItemList::GetInstance();

	json::array equipments = characterData.at("start_equips").as_array();

	for (auto& equipment : equipments) {
		int slot = json::value_to<int>(equipment.at("slot"));

		PItem item = &character.Equipaments[slot];

		item->Index = json::value_to<int>(equipment.at("index"));
		item->Apparence = item->Index;

		item->MaxDurability = itemList->Get(item->Index).Durability;
		item->MinDurability = item->MaxDurability;
	}

	json::array inventory = characterData.at("start_inventory").as_array();

	for (auto& equipment : inventory) {
		int slot = json::value_to<int>(equipment.at("slot"));

		PItem item = &character.Inventory[slot];

		item->Index = json::value_to<int>(equipment.at("index"));
		item->Apparence = item->Index;

		item->MaxDurability = itemList->Get(item->Index).Durability;
		item->MinDurability = item->MaxDurability;

		item->Amount = json::value_to<int>(equipment.at("amount"));
	}

	json::array skills = characterData.at("start_skills").as_array();

	for (auto& skill : skills) {
		int slot = json::value_to<int>(skill.at("slot"));

		PCharacterSkill item = &character.SkillList[slot];

		item->skillID = (WORD)json::value_to<int>(skill.at("index"));
	}

	json::array itemBar = characterData.at("start_item_bar").as_array();

	for (auto& bar : itemBar) {
		int slot = json::value_to<int>(bar.at("slot"));

		PCharacterItemBar item = &character.ItemBar[slot];

		item->itemID = (DWORD)json::value_to<int>(bar.at("index"));
	}

	return character;
}


bool InitialCharacters::Add(WORD characterClass, boost::json::value characterData) {
	try {
		Character character = this->FromJSON(characterData);

		this->_characters.insert(std::make_pair(characterClass, character));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[InitialCharacters::Add] error: %s", e.what());
		return false;
	}

	return true;
}

bool InitialCharacters::Get(WORD characterClass, Character* character) {
	try {
		*character = this->_characters.at(characterClass);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[InitialCharacters::Get] error: %s", e.what());
		return false;
	}

	return true;
}

Character InitialCharacters::Get(WORD characterClass) {
	Character character = Character();

	if (!this->Get(characterClass, &character)) {
		throw std::exception(Logger::Format("character class [%d] not found", characterClass).c_str());
	}

	return character;
}

