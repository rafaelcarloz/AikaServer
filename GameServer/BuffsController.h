#pragma once

#ifndef _BUFFS_CONTROLLER_H_
#define _BUFFS_CONTROLLER_H_

#include <stdio.h>
#include <map>
#include <mutex>

#include <boost/json.hpp>

#include "PlayerData.h"
#include "GameEntity.fwd.h"


class BuffsController {
private:
	PGameEntity _parent;

	uint16_t _entityId;

	uint64_t databaseBuff[60];
	std::map<uint16_t, time_t>* _buffs;

	std::mutex* _rwlock;

	bool _modified = false;

	void CalculateBuffDuration(uint16_t buffId, uint32_t& buffDuration);
	void EvaluateBuffTimes();
public:
	void Initialize(uint16_t entityId, PGameEntity parent);

	void Recalculate();

	std::map<uint16_t, time_t> GetBuffs();

	bool AddBuff(uint16_t buffId, uint32_t buffDuration);
	bool AddBuff(uint16_t buffId);

	void RemoveBuff(uint16_t buffId);

	bool SetBuffDuration(uint16_t buffId, uint32_t buffDuration);


	void SendRefreshBuffs();

	bool GetDatabaseData();
	boost::json::value ToJSON();
};

#endif
