#include "TitleList.h"
#include "Logger.h"

std::atomic<TitleList*> TitleList::_instance{ nullptr };
std::mutex TitleList::_mutex;

TitleList* TitleList::GetInstance() {
	TitleList* tmp = _instance.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);

	if (tmp == nullptr) {
		std::lock_guard<std::mutex> lock(_mutex);

		tmp = _instance.load(std::memory_order_relaxed);
		if (tmp == nullptr) {
			tmp = new TitleList();
			std::atomic_thread_fence(std::memory_order_release);
			_instance.store(tmp, std::memory_order_relaxed);
		}
	}

	return tmp;
}

bool TitleList::Add(const TitleFromList item) {
	this->_itemCount++;

	try {
		this->_items.insert(std::make_pair(this->_itemCount - 1, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[TitleList::Add] error: %s", e.what());
		return false;
	}

	return true;
}

bool TitleList::Get(const uint16_t itemId, TitleFromList* item) {
	try {
		if (this->_items.size() == 0) {
			throw std::exception("empty title list");
		}

		if (this->_items.find(itemId) == this->_items.end()) {
			Logger::Write(Warnings, "[TitleList::Get] titleId %d not found", itemId);
			return false;
		}

		*item = this->_items[itemId];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[TitleList::Get] error: %s", e.what());
		return false;
	}

	return true;
}

TitleFromList TitleList::Get(const uint16_t itemId) {
	TitleFromList item = TitleFromList();

	if (!TitleList::GetInstance()->Get(itemId, &item)) {
		throw std::exception(Logger::Format("title [%d] not found", itemId).c_str());
	}

	return item;
}
