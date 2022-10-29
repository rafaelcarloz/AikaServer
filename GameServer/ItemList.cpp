#include "ItemList.h"
#include "Logger.h"

std::atomic<ItemList*> ItemList::_instance{ nullptr };
std::mutex ItemList::_mutex;

ItemList* ItemList::GetInstance() {
	ItemList* tmp = _instance.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);

	if (tmp == nullptr) {
		std::lock_guard<std::mutex> lock(_mutex);

		tmp = _instance.load(std::memory_order_relaxed);
		if (tmp == nullptr) {
			tmp = new ItemList();
			std::atomic_thread_fence(std::memory_order_release);
			_instance.store(tmp, std::memory_order_relaxed);
		}
	}

	return tmp;
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

		if (this->_items.find(itemId) == this->_items.end()) {
			Logger::Write(Warnings, "[ItemList::Get] itemId %d not found", itemId);
			return false;
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

	if (!ItemList::GetInstance()->Get(itemId, &item)) {
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



bool ItemList::AddUseItem(const WORD index, std::string fileName, BYTE scriptType) {
	try {
		if (scriptType == 0) {
			if (this->_consumeFunctions.find(index) != this->_consumeFunctions.end()) {
				throw std::exception(Logger::Format("item function %d already added", index).c_str());
			}

			this->_consumeFunctions.insert(std::make_pair(index, fileName));
		}
		else {
			if (this->_consumeEffects.find(index) != this->_consumeEffects.end()) {
				throw std::exception(Logger::Format("item effect %d already added", index).c_str());
			}

			this->_consumeEffects.insert(std::make_pair(index, fileName));
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ItemList::AddConsumeEffect] error: %s", e.what());
		return false;
	}

	return true;
}

std::string ItemList::GetUseItem(const WORD index, BYTE scriptType) {
	const char* result = "";

	try {
		if (scriptType == 0) {
			if (this->_consumeFunctions.find(index) == this->_consumeFunctions.end()) {
				throw std::exception(Logger::Format("item function %d not found", index).c_str());
			}

			return this->_consumeFunctions[index];
		}
		else {
			if (this->_consumeEffects.find(index) == this->_consumeEffects.end()) {
				throw std::exception(Logger::Format("item effect %d not found", index).c_str());
			}

			return this->_consumeEffects[index];
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ItemList::GetUseItem] error: %s", e.what());
		return result;
	}

	return result;
}


BYTE ItemFromList::GetEquipSlot() {
	if (this->ItemType > 0 && this->ItemType < 16) {
		return this->ItemType;
	}

	if (this->ItemType == 50 || this->ItemType == 52) {
		return 15;
	}

	if (this->ItemType > 1000 && this->ItemType < 1009) {
		return 6;
	}

	return 0;
}


