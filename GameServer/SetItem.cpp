#include "SetItem.h"
#include "Logger.h"

SetItem* SetItem::_instance{ nullptr };

SetItem* SetItem::GetInstance() {
	if (_instance == nullptr)
	{
		_instance = new SetItem();
	}
	return _instance;
}

bool SetItem::Add(const SetItemFromList item) {
	this->_itemCount++;

	if (item.Name[0] == 0) {
		return true;
	}

	try {
		this->_items.insert(std::make_pair(this->_itemCount - 1, item));

		for (int i = 0; i < 12; i++) {
			int itemId = item.RequiredEquips[i];

			if (itemId > 0) {
				this->_itemsBySett.insert(std::make_pair(itemId, this->_itemCount - 1));
			}
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[SetItem::Add] error: %s", e.what());
		return false;
	}

	return true;
}

bool SetItem::Get(const WORD itemId, SetItemFromList* item) {
	try {
		if (this->_items.size() == 0) {
			throw std::exception("empty set item list");
		}

		*item = this->_items[itemId];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[SetItem::Get] error: %s", e.what());
		return false;
	}

	return true;
}

SetItemFromList SetItem::Get(const WORD itemId) {
	SetItemFromList item = SetItemFromList();

	if (!this->Get(itemId, &item)) {
		throw std::exception(Logger::Format("set item [%d] not found", itemId).c_str());
	}

	return item;
}

SetItemFromList SetItem::GetByItemId(const WORD itemId) {
	SetItemFromList result = SetItemFromList();

	if (itemId <= 0) {
		return result;
	}

	if (this->_itemsBySett.find(itemId) == this->_itemsBySett.end()) {
		return result;
	}

	WORD setId = this->_itemsBySett.at(itemId);

	return this->Get(setId);
}

WORD SetItem::GetSetIdByItemId(const WORD itemId) {
	if (itemId <= 0) {
		return 0;
	}

	if (this->_itemsBySett.find(itemId) == this->_itemsBySett.end()) {
		return 0;
	}

	return this->_itemsBySett.at(itemId);
}


SetItemFromList SetItem::operator[](const WORD itemId) {
	SetItemFromList item = SetItemFromList();

	if (!this->Get(itemId, &item)) {
		throw std::exception("set Item not found");
	}

	return item;
}