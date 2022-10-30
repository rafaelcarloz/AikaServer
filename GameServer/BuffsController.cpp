#include "BuffsController.h"
#include "Logger.h"
#include "GameEntity.h"
#include "Packets.h"
#include "EntityHandler.h"
#include "SkillData.h"
#include "DateFunctions.h"
#include "WebHandler.h"

using namespace packets;
namespace json = boost::json;

void BuffsController::Initialize(uint16_t entityId, PGameEntity parent) {
	this->_parent = parent;

	this->_entityId = entityId;

	this->_buffs = new std::map<uint16_t, time_t>();
	this->_rwlock = new std::mutex;

	ZeroMemory(&this->databaseBuff, 60*sizeof(uint64_t));

	if (!this->GetDatabaseData()) {
		return;
	}

	this->Recalculate();
}

void BuffsController::Recalculate() {
	if (!this->_parent->IsIstantiated) {
		return;
	}

	this->EvaluateBuffTimes();
}

void BuffsController::EvaluateBuffTimes() {
	auto currentBuffs = this->GetBuffs();

	if (currentBuffs.size() == 0) {
		return;
	}

	for (auto& buff : currentBuffs) {
		if (time(0) > buff.second) {
			this->RemoveBuff(buff.first);
		}
	}

	if (this->_modified) {
		this->SendRefreshBuffs();

		this->_parent->statusController.Recalculate();
	}
}


void BuffsController::CalculateBuffDuration(uint16_t buffId, uint32_t& buffDuration) {
	buffDuration += (uint32_t)(this->_parent->statusController.GetEffect(EF_RELIQUE_SKILL_ATIME0) * (buffDuration / 100));
}


std::map<uint16_t, time_t> BuffsController::GetBuffs() {
	if (this->_parent == nullptr) {
		return std::map<uint16_t, time_t>();
	}

	std::lock_guard<std::mutex> lock(*_rwlock);

	return *this->_buffs;
}

bool BuffsController::AddBuff(uint16_t buffId, uint32_t buffDuration) {
	if (buffId == 7257 || buffId == 9133) {
		return false;
	}
	
	auto currentBuffs = this->GetBuffs();

	if (currentBuffs.size() >= 60) {
		return false;
	}

	this->CalculateBuffDuration(buffId, buffDuration);

	if (currentBuffs.find(buffId) != currentBuffs.end()) {
		this->SetBuffDuration(buffId, buffDuration);
		return true;
	}

	std::lock_guard<std::mutex> lock(*_rwlock);
	this->_buffs->insert(std::make_pair(buffId, (time_t)(time(0)+buffDuration)));

	this->_modified = true;

	return true;
}

bool BuffsController::AddBuff(uint16_t buffId) {
	SkillFromList skillBuff = SkillData::GetInstance()->Get(buffId);

	return this->AddBuff(buffId, skillBuff.Duration);
}

void BuffsController::RemoveBuff(uint16_t buffId) {
	std::lock_guard<std::mutex> lock(*_rwlock);

	try {
		this->_buffs->erase(buffId);

		this->_modified = true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[BuffsController::RemoveBuff] error: %s", e.what());
	}
}


bool BuffsController::SetBuffDuration(uint16_t buffId, uint32_t buffDuration) {
	std::lock_guard<std::mutex> lock(*_rwlock);

	try {
		if (this->_buffs->find(buffId) == this->_buffs->end()) {
			return false;
		}

		this->_buffs->at(buffId) = (time_t)(time(0)+buffDuration);

		this->_modified = true;

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[BuffsController::SetBuffDuration] error: %s", e.what());
	}

	return false;
}


void BuffsController::SendRefreshBuffs() {
	PacketRefreshAllBuffs packet;

	ZeroMemory(&packet, sizeof(PacketRefreshAllBuffs));

	packet.header.Size = sizeof(PacketRefreshAllBuffs);
	packet.header.Index = this->_entityId;
	packet.header.Code = 0x10B;

	auto buffs = this->GetBuffs();

	BYTE slot = 0;

	for (auto& buff : buffs) {
		packet.buffsIDs[slot] = buff.first;
		packet.buffsDuration[slot] = (uint32_t)buff.second;
		
		slot++;
	}

	bool sendSelf = (EntityHandler::GetEntityType(this->_entityId) == EntityPlayer);
	this->_modified = false;

	this->_parent->visibleController->SendToVisible(&packet, packet.header.Size, sendSelf);
}


bool BuffsController::GetDatabaseData() {
	WebHandler webHandler;

	try {
		json::value response = json::value();

		if (!webHandler.GetCharacterBuffs(this->_parent->character.CharacterId, &response)) {
			return false;
		}

		BYTE slot = 0;
		for (auto& item : response.as_array()) {
			uint64_t buffID = json::value_to<int>(item.at("characterBuffID"));

			this->databaseBuff[slot] = buffID;

			uint16_t buffSkillID = json::value_to<int>(item.at("skillListID"));
			std::string buffDuration = (item.at("buffDuration").is_null()) ? "" : json::value_to<std::string>(item.at("buffDuration"));

			this->_buffs->insert(std::make_pair(buffSkillID, DateFunctions::StringToDate(buffDuration)));

			slot++;
		}

		this->_modified = true;

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[BuffsController::GetDatabaseData] error: %s", e.what());
	}

	return false;
}

boost::json::value BuffsController::ToJSON() {
	auto buffs = this->GetBuffs();

	json::value result = json::value();

	json::array& buffList = result.emplace_array();

	WORD buffIds[60];

	ZeroMemory(&buffIds, 60*sizeof(WORD));

	BYTE slot = 0;
	for (auto& buff : buffs) {
		buffIds[slot] = buff.first;

		slot++;
	}

	for (int i = 0; i < 60; i++) {
		json::value _buffHolder;
		json::object& buffItem = _buffHolder.emplace_object();

		buffItem.emplace("CharacterBuffID", this->databaseBuff[i]);
		buffItem.emplace("CharacterID", this->_parent->character.CharacterId); 
		buffItem.emplace("BuffStatus", 1);
		buffItem.emplace("SkillListID", buffIds[i]);

		if (buffIds[i] > 0) {
			buffItem.emplace("BuffDuration", DateFunctions::DateToString(buffs[buffIds[i]]));
		}

		buffList.emplace_back(buffItem);
	}

	return result;
}