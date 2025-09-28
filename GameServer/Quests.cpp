#include "Quests.h"
#include "Logger.h"

std::atomic<QuestList*> QuestList::_instance{ nullptr };
std::mutex QuestList::_mutex;

QuestList* QuestList::GetInstance() {
	QuestList* tmp = _instance.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);

	if (tmp == nullptr) {
		std::lock_guard<std::mutex> lock(_mutex);

		tmp = _instance.load(std::memory_order_relaxed);
		if (tmp == nullptr) {
			tmp = new QuestList();
			std::atomic_thread_fence(std::memory_order_release);
			_instance.store(tmp, std::memory_order_relaxed);
		}
	}

	return tmp;
}

bool QuestList::Add(const QuestData item) {
	this->_itemCount++;

	if (item.Index == 0) {
		return true;
	}

	try {
		this->_items.insert(std::make_pair(this->_itemCount - 1, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[QuestList::Add] error: %s", e.what());
		return false;
	}

	return true;
}

bool QuestList::Get(const WORD itemId, QuestData* item) {
	try {
		if (this->_items.size() == 0) {
			throw std::exception("empty item list");
		}

		if (this->_items.find(itemId) == this->_items.end()) {
			Logger::Write(Warnings, "[QuestList::Get] itemId %d not found", itemId);
			return false;
		}

		*item = this->_items[itemId];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[QuestList::Get] error: %s", e.what());
		return false;
	}

	return true;
}

QuestData QuestList::Get(const uint16_t itemId) {
	QuestData item = QuestData();

	if (!QuestList::GetInstance()->Get(itemId, &item)) {
		throw std::exception(Logger::Format("item [%d] not found", itemId).c_str());
	}

	return item;
}

QuestData QuestList::operator[](const WORD itemId) {
	QuestData item = QuestData();

	if (!this->Get(itemId, &item)) {
		throw std::exception("Item not found");
	}

	return item;
}

bool QuestList::Contains(const WORD itemId) {
	if (this->_items.size() == 0) {
		return false;
	}

	return (this->_items.find(itemId) != this->_items.end());
}

std::map<WORD, QuestData> QuestList::GetQuests() {
	return this->_items;
}



bool QuestList::AddUnlocks(const uint16_t questId, const uint16_t unlocksId) {
	std::vector<uint16_t> unlockList = std::vector<uint16_t>();

	if (this->ContainsUnlocks(questId)) {
		unlockList = this->unlockQuests[questId];
	}

	unlockList.push_back(unlocksId);

	this->unlockQuests[questId] = unlockList;

	return true;
}

bool QuestList::ContainsUnlocks(const WORD questId) {
	if (this->unlockQuests.size() == 0) {
		return false;
	}

	return (this->unlockQuests.find(questId) != this->unlockQuests.end());
}

std::vector<uint16_t> QuestList::GetUnlocks(const uint16_t questId) {
	return std::vector<uint16_t>();
}



uint16_t QuestData::UnlockedByQuestId() {
	uint16_t questId = 0;

	for (int i = 0; i < 5; i++) {
		if (this->PreConditions[i].TypeID == QuestDetailQuest) {
			return this->PreConditions[i].Quantity1;
		}
	}

	return questId;
}
