#include "SkillData.h"
#include "Logger.h"
#include <cmath>

SkillData* SkillData::_instance{ nullptr };

SkillData* SkillData::GetInstance() {
	if (_instance == nullptr)
	{
		_instance = new SkillData();
	}
	return _instance;
}

bool SkillData::Add(const SkillFromList item) {
	this->_itemCount++;

	if (item.Name[0] == 0) {
		return true;
	}

	try {
		this->_items.insert(std::make_pair(this->_itemCount - 1, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[SkillData::Add] error: %s", e.what());
		return false;
	}

	return true;
}

bool SkillData::Get(const WORD itemId, SkillFromList* item) {
	try {
		if (this->_items.size() == 0) {
			throw std::exception("empty skill list");
		}

		*item = this->_items[itemId];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[SkillData::Get] error: %s", e.what());
		return false;
	}

	return true;
}

SkillFromList SkillData::Get(const WORD itemId) {
	SkillFromList item = SkillFromList();

	if (!SkillData::GetInstance()->Get(itemId, &item)) {
		throw std::exception(Logger::Format("skill [%d] not found", itemId).c_str());
	}

	return item;
}

SkillFromList SkillData::operator[](const WORD itemId) {
	SkillFromList item = SkillFromList();

	if (!SkillData::GetInstance()->Get(itemId, &item)) {
		throw std::exception("skill not found");
	}

	return item;
}

BYTE SkillData::GetSendToWorldSkill(WORD skillId, DWORD& skill) {
	skill = pow(2, this->Get(skillId).Level + 1) - 2;

	if (skill < 65536) {
		return 1;
	}
	else {
		return 2;
	}
}