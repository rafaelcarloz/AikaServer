#include "ExpList.h"
#include "Logger.h"

std::atomic<ExpList*> ExpList::_instance{ nullptr };
std::mutex ExpList::_mutex;

ExpList* ExpList::GetInstance() {
	ExpList* tmp = _instance.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);

	if (tmp == nullptr) {
		std::lock_guard<std::mutex> lock(_mutex);

		tmp = _instance.load(std::memory_order_relaxed);
		if (tmp == nullptr) {
			tmp = new ExpList();
			std::atomic_thread_fence(std::memory_order_release);
			_instance.store(tmp, std::memory_order_relaxed);
		}
	}

	return tmp;
}

bool ExpList::Add(const uint64_t item) {
	this->_itemCount++;

	if (item == 0) {
		return true;
	}

	try {
		this->_items.insert(std::make_pair(this->_itemCount - 1, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ExpList::Add] error: %s", e.what());
		return false;
	}

	return true;
}

bool ExpList::Get(const uint16_t itemId, uint64_t* item) {
	try {
		if (this->_items.size() == 0) {
			throw std::exception("empty item list");
		}

		if (this->_items.find(itemId) == this->_items.end()) {
			Logger::Write(Warnings, "[ExpList::Get] level %d not found", itemId);
			return false;
		}

		*item = this->_items[itemId];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ExpList::Get] error: %s", e.what());
		return false;
	}

	return true;
}

uint64_t ExpList::Get(const uint16_t itemId) {
	uint64_t item = 0;

	if (!ExpList::GetInstance()->Get(itemId, &item)) {
		throw std::exception(Logger::Format("level [%d] not found", itemId).c_str());
	}

	return item;
}

uint64_t ExpList::operator[](const uint16_t itemId) {
	uint64_t item = 0;

	if (!this->Get(itemId, &item)) {
		throw std::exception("Level not found");
	}

	return item;
}


