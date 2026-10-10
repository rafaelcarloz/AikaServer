#include "DropList.h"
#include "Logger.h"

std::atomic<DropList*> DropList::_instance{ nullptr };
std::mutex DropList::_mutex;

DropList* DropList::GetInstance() {
	DropList* tmp = _instance.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);

	if (tmp == nullptr) {
		std::lock_guard<std::mutex> lock(_mutex);

		tmp = _instance.load(std::memory_order_relaxed);
		if (tmp == nullptr) {
			tmp = new DropList();
			std::atomic_thread_fence(std::memory_order_release);
			_instance.store(tmp, std::memory_order_relaxed);
		}
	}

	return tmp;
}

bool DropList::Add(const uint16_t index, const DropItems item) {
	try {
		this->_items.insert(std::make_pair(index, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DropList::Add] error: %s", e.what());
		return false;
	}

	return true;
}

bool DropList::Get(const uint16_t index, DropItems* item) {
	try {
		if (this->_items.size() == 0) {
			throw std::exception("empty item list");
		}

		if (this->_items.find(index) == this->_items.end()) {
			Logger::Write(Warnings, "[DropList::Get] DropItems %d not found", index);
			return false;
		}

		*item = this->_items[index];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DropList::Get] error: %s", e.what());
		return false;
	}

	return true;
}

DropItems DropList::Get(const uint16_t index) {
	DropItems item = DropItems();;

	if (!DropList::GetInstance()->Get(index, &item)) {
		throw std::exception(Logger::Format("Drops [%d] not found", index).c_str());
	}

	return item;
}

bool DropList::Contains(const uint16_t index) {
	return (this->_items.find(index) != this->_items.end());
}


DropItems DropList::operator[](const uint16_t index) {
	DropItems item = DropItems();

	if (!this->Get(index, &item)) {
		throw std::exception("Drops not found");
	}

	return item;
}


