#include "ItemList.h"
#include "Logger.h"

ItemList* ItemList::_instance{ nullptr };

ItemList* ItemList::GetInstance() {
	if (_instance == nullptr)
	{
		_instance = new ItemList();
	}
	return _instance;
}

bool ItemList::Add(const ItemFromList item) {
	this->_itemCount++;

	if (item.Name[0] == 0) {
		return true;
	}

	try {
		this->_items.insert(std::make_pair(this->_itemCount-1, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ItemList::Add] error: %s", e.what());
		return false;
	}

	return true;
}

bool ItemList::Get(const WORD itemId, ItemFromList* item) {
	try {
		if (this->_items.size() == 0) {
			throw std::exception("empty item list");
		}

		*item = this->_items[itemId];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ItemList::Get] error: %s", e.what());
		return false;
	}

	return true;
}

ItemFromList ItemList::Get(const WORD itemId) {
	ItemFromList item = ItemFromList();

	if (!this->Get(itemId, &item)) {
		throw std::exception(Logger::Format("item [%d] not found", itemId).c_str());
	}

	return item;
}

ItemFromList ItemList::operator[](const WORD itemId) {
	ItemFromList item = ItemFromList();

	if (!this->Get(itemId, &item)) {
		throw std::exception("Item not found");
	}

	return item;
}



BYTE ItemFromList::GetEquipSlot() {
	if (this->ItemType >= 0 && this->ItemType < 15) {
		return this->ItemType;
	}

	if (this->ItemType > 1000 && this->ItemType < 1009) {
		return 6;
	}
}