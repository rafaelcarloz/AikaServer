#include "MapData.h"
#include "Logger.h"

std::atomic<MapData*> MapData::_instance{ nullptr };
std::mutex MapData::_mutex;

MapData* MapData::GetInstance() {
	MapData* tmp = _instance.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);

	if (tmp == nullptr) {
		std::lock_guard<std::mutex> lock(_mutex);

		tmp = _instance.load(std::memory_order_relaxed);
		if (tmp == nullptr) {
			tmp = new MapData();
			std::atomic_thread_fence(std::memory_order_release);
			_instance.store(tmp, std::memory_order_relaxed);
		}
	}

	return tmp;
}

bool MapData::AddTeleport(const ScrollTeleportPosition item) {
	this->_teleportCount++;

	try {
		this->_teleports.insert(std::make_pair(this->_teleportCount - 1, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[MapData::AddTeleport] error: %s", e.what());
		return false;
	}

	return true;
}

bool MapData::GetTeleport(const uint16_t itemId, ScrollTeleportPosition* item) {
	try {
		if (this->_teleports.size() == 0) {
			throw std::exception("empty teleport list");
		}

		if (this->_teleports.find(itemId) == this->_teleports.end()) {
			Logger::Write(Warnings, "[MapData::GetTeleport] teleport position %d not found", itemId);
			return false;
		}

		*item = this->_teleports[itemId];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[MapData::GetTeleport] error: %s", e.what());
		return false;
	}

	return true;
}


Position ScrollTeleportPosition::ToPosition() {
	return Position(static_cast<float>(this->PositionX), static_cast<float>(this->PositionY));
}


ScrollTeleportPosition MapData::GetTeleport(const uint16_t itemId) {
	ScrollTeleportPosition item = ScrollTeleportPosition();

	if (!MapData::GetInstance()->GetTeleport(itemId, &item)) {
		throw std::exception(Logger::Format("teleport position [%d] not found", itemId).c_str());
	}

	return item;
}


bool MapData::AddBounds(const MapBounds item) {
	this->_boundsCount++;

	try {
		this->_bounds.insert(std::make_pair(this->_boundsCount - 1, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[MapData::AddBounds] error: %s", e.what());
		return false;
	}

	return true;
}

bool MapData::GetBounds(const uint16_t itemId, MapBounds* item) {
	try {
		if (this->_bounds.size() == 0) {
			throw std::exception("empty bounds list");
		}

		if (this->_bounds.find(itemId) == this->_bounds.end()) {
			Logger::Write(Warnings, "[MapData::GetBounds] bounds position %d not found", itemId);
			return false;
		}

		*item = this->_bounds[itemId];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[MapData::GetTeleport] error: %s", e.what());
		return false;
	}

	return true;
}

MapBounds MapData::GetBounds(const uint16_t itemId) {
	MapBounds item = MapBounds();

	if (!MapData::GetInstance()->GetBounds(itemId, &item)) {
		throw std::exception(Logger::Format("bounds position [%d] not found", itemId).c_str());
	}

	return item;
}
