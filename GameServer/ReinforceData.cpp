#include <cmath>

#include "ReinforceData.h"
#include "Logger.h"
#include "ItemList.h"

ReinforceData* ReinforceData::_instance{ nullptr };

ReinforceData* ReinforceData::GetInstance() {
	if (_instance == nullptr)
	{
		_instance = new ReinforceData();
	}
	return _instance;
}


bool ReinforceData::AddReinforce2(const ItemAttributeReinforce item) {
	this->_reinforce2Count++;

	try {
		this->_reinforce2.insert(std::make_pair(this->_reinforce2Count - 1, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ReinforceData::AddReinforce2] error: %s", e.what());
		return false;
	}

	return true;
}

bool ReinforceData::GetReinforce2(const WORD itemId, ItemAttributeReinforce* item) {
	try {
		if (this->_reinforce2.size() == 0) {
			throw std::exception("empty reinforce2 list");
		}

		WORD reinforce2Id = this->GetReinforce2Index(itemId);

		*item = this->_reinforce2[reinforce2Id];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ReinforceData::GetReinforce2] error: %s", e.what());
		return false;
	}

	return true;
}

ItemAttributeReinforce ReinforceData::GetReinforce2(const WORD itemId) {
	ItemAttributeReinforce item = ItemAttributeReinforce();

	if (!this->GetReinforce2(itemId, &item)) {
		throw std::exception(Logger::Format("reinforce2 [%d] not found", itemId).c_str());
	}

	return item;
}


bool ReinforceData::AddReinforce3(const ArmorAttributeReinforce item) {
	this->_reinforce3Count++;

	try {
		this->_reinforce3.insert(std::make_pair(this->_reinforce3Count - 1, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ReinforceData::AddReinforce3] error: %s", e.what());
		return false;
	}

	return true;
}

bool ReinforceData::GetReinforce3(const WORD itemId, ArmorAttributeReinforce* item) {
	try {
		if (this->_reinforce3.size() == 0) {
			throw std::exception("empty reinforce3 list");
		}

		WORD reinforce3Id = this->GetReinforce3Index(itemId);

		*item = this->_reinforce3[reinforce3Id];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ReinforceData::GetReinforce3] error: %s", e.what());
		return false;
	}

	return true;
}

ArmorAttributeReinforce ReinforceData::GetReinforce3(const WORD itemId) {
	ArmorAttributeReinforce item = ArmorAttributeReinforce();

	if (!this->GetReinforce3(itemId, &item)) {
		throw std::exception(Logger::Format("reinforce3 [%d] not found", itemId).c_str());
	}

	return item;
}


WORD ReinforceData::GetReinforce2Index(const WORD itemId) {
	ItemList* itemList = ItemList::GetInstance();

	try {
		ItemFromList item = itemList->Get(itemId);

		WORD ReinforceIndex = 0;
		WORD ItemUseEffect = item.UseEffect;

		if (ItemUseEffect > 0 && ItemUseEffect <= 35) {
			ReinforceIndex = reinforce2sectionSize * 0;
		}
		else if (ItemUseEffect > 35 && ItemUseEffect <= 70) {
			ReinforceIndex = reinforce2sectionSize * 1;
			ReinforceIndex -= 35;
		}
		else if (ItemUseEffect > 70 && ItemUseEffect <= 105) {
			ReinforceIndex = reinforce2sectionSize * 2;
			ReinforceIndex -= 70;
		}

		BYTE ClassInfo = (BYTE)(item.CharacterClass / 10);
		BYTE EquipSlot = item.GetEquipSlot();

		if (EquipSlot == 6) {
			switch (ClassInfo)
			{

			case 0:
				ReinforceIndex += Reinforce2_Area_Sword;
				break;
			case 1:
				ReinforceIndex += Reinforce2_Area_Blade;
				break;
			case 2:
				ReinforceIndex += Reinforce2_Area_Rifle;
				break;
			case 3:
				ReinforceIndex += Reinforce2_Area_Pistol;
				break;
			case 4:
				ReinforceIndex += Reinforce2_Area_Staff;
				break;
			case 5:
				ReinforceIndex += Reinforce2_Area_Wand;
				break;

			default:
				break;
			}

			return (ReinforceIndex + ItemUseEffect);
		}

		switch (EquipSlot)
		{

		case 2:
			ReinforceIndex += Reinforce2_Area_Helmet + (ClassInfo * 30);
			break;
		case 3:
			ReinforceIndex += Reinforce2_Area_Armor + (ClassInfo * 30);
			break;
		case 4:
			ReinforceIndex += Reinforce2_Area_Gloves + (ClassInfo * 30);
			break;
		case 5:
			ReinforceIndex += Reinforce2_Area_Shoes + (ClassInfo * 30);
			break;

		case 7:
			ReinforceIndex += Reinforce2_Area_Shield;
			break;


		default:
			break;
		}

		return (ReinforceIndex + ItemUseEffect);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ReinforceData::GetReinforce2Index] error: %s", e.what());

		return 0;
	}

	return 0;
}

WORD ReinforceData::GetReinforce3Index(const WORD itemId) {
	ItemList* itemList = ItemList::GetInstance();

	try {
		ItemFromList item = itemList->Get(itemId);

		WORD ReinforceIndex = 0;
		WORD ItemUseEffect = item.UseEffect;

		if (ItemUseEffect > 0 && ItemUseEffect <= 35) {
			ReinforceIndex = reinforce3sectionSize * 0;
		}
		else if (ItemUseEffect > 35 && ItemUseEffect <= 70) {
			ReinforceIndex = reinforce3sectionSize * 1;
			ReinforceIndex -= 35;
		}
		else if (ItemUseEffect > 70 && ItemUseEffect <= 105) {
			ReinforceIndex = reinforce3sectionSize * 2;
			ReinforceIndex -= 70;
		}

		BYTE EquipSlot = item.GetEquipSlot();

		switch (EquipSlot)
		{

		case 2:
			ReinforceIndex += Reinforce3_Area_Helmet;
			break;
		case 3:
			ReinforceIndex += Reinforce3_Area_Armor;
			break;
		case 4:
			ReinforceIndex += Reinforce3_Area_Gloves;
			break;
		case 5:
			ReinforceIndex += Reinforce3_Area_Shoes;
			break;

		case 7:
			ReinforceIndex += Reinforce3_Area_Shield;
			break;

		default:
			break;
		}

		return (ReinforceIndex + ItemUseEffect);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ReinforceData::GetReinforce3Index] error: %s", e.what());

		return 0;
	}

	return 0;
}