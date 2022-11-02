#include "MobPos.h"
#include "Logger.h"
#include <boost/json.hpp>
#include <iostream>
#include <fstream>

std::atomic<MobPos*> MobPos::_instance{ nullptr };
std::mutex MobPos::_mutex;

namespace json = boost::json;

MobPos* MobPos::GetInstance() {
	MobPos* tmp = _instance.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);

	if (tmp == nullptr) {
		std::lock_guard<std::mutex> lock(_mutex);

		tmp = _instance.load(std::memory_order_relaxed);
		if (tmp == nullptr) {
			tmp = new MobPos();
			std::atomic_thread_fence(std::memory_order_release);
			_instance.store(tmp, std::memory_order_relaxed);
		}
	}

	return tmp;
}

bool MobPos::Add(const MobPosFromList item, uint16_t mobId) {
	try {
		this->_items.insert(std::make_pair(mobId, item));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[MobPos::Add] error: %s", e.what());
		return false;
	}

	return true;
}

bool MobPos::Get(const uint16_t mobId, MobPosFromList* item) {
	try {
		if (this->_items.size() == 0) {
			throw std::exception("empty title list");
		}

		if (this->_items.find(mobId) == this->_items.end()) {
			Logger::Write(Warnings, "[MobPos::Get] mobId %d not found", mobId);
			return false;
		}

		*item = this->_items[mobId];
	}
	catch (std::exception e) {
		Logger::Write(Error, "[MobPos::Get] error: %s", e.what());
		return false;
	}

	return true;
}

MobPosFromList MobPos::Get(const uint16_t mobId) {
	MobPosFromList item = MobPosFromList();

	if (!MobPos::GetInstance()->Get(mobId, &item)) {
		throw std::exception(Logger::Format("mob [%d] not found", mobId).c_str());
	}

	return item;
}

void
pretty_print(std::ostream& os, json::value const& jv, std::string* indent = nullptr)
{
	std::string indent_;
	if (!indent)
		indent = &indent_;
	switch (jv.kind())
	{
	case json::kind::object:
	{
		os << "{\n";
		indent->append(4, ' ');
		auto const& obj = jv.get_object();
		if (!obj.empty())
		{
			auto it = obj.begin();
			for (;;)
			{
				os << *indent << json::serialize(it->key()) << " : ";
				pretty_print(os, it->value(), indent);
				if (++it == obj.end())
					break;
				os << ",\n";
			}
		}
		os << "\n";
		indent->resize(indent->size() - 4);
		os << *indent << "}";
		break;
	}

	case json::kind::array:
	{
		os << "[\n";
		indent->append(4, ' ');
		auto const& arr = jv.get_array();
		if (!arr.empty())
		{
			auto it = arr.begin();
			for (;;)
			{
				os << *indent;
				pretty_print(os, *it, indent);
				if (++it == arr.end())
					break;
				os << ",\n";
			}
		}
		os << "\n";
		indent->resize(indent->size() - 4);
		os << *indent << "]";
		break;
	}

	case json::kind::string:
	{
		os << json::serialize(jv.get_string());
		break;
	}

	case json::kind::uint64:
		os << jv.get_uint64();
		break;

	case json::kind::int64:
		os << jv.get_int64();
		break;

	case json::kind::double_:
		os << jv.get_double();
		break;

	case json::kind::bool_:
		if (jv.get_bool())
			os << "true";
		else
			os << "false";
		break;

	case json::kind::null:
		os << "null";
		break;
	}

	if (indent->empty())
		os << "\n";
}

bool MobPos::SaveToJSON() {
	try {
		json::value data = json::value();

		json::array& items = data.emplace_array();

		for (auto& mob : this->_items) {
			json::value _holder = json::value();

			json::object& item = _holder.emplace_object();

			item.emplace("Index", mob.second.MobIndex);
			item.emplace("Name", mob.second.MobName);

			json::value temp = json::value();
			json::array positions = temp.emplace_array();
			for (int i = 0; i < 50; i++) {
				if (mob.second.position[i].X == 0) {
					continue;
				}

				json::value _holder2 = json::value();

				json::object& pos = _holder2.emplace_object();

				pos.emplace("x", mob.second.position[i].X);
				pos.emplace("y", mob.second.position[i].Y);

				positions.emplace_back(pos);
			}

			item.emplace("positions", positions);

			items.emplace_back(item);
		}

		std::string fileName = Logger::Format(R"(%s\Data\MobPos.json)", R"(C:\AikaProject\AikaServer\x64\Debug)");

		std::ofstream ifs(fileName, std::ios::out | std::ios::binary);

		if (!ifs.is_open()) {
			Logger::Write("error loading Mob Positions, file not found!", LOG_TYPE::Error);
			return false;
		}

		pretty_print(ifs, data);
		ifs.close();
	}
	catch (std::exception e) {
		Logger::Write(Error, "[MobPos::SaveToJSON] error: %s", e.what());
		return false;
	}
}
