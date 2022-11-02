#pragma once

#ifndef _VISIBLE_CONTROLLER_H_
#define _VISIBLE_CONTROLLER_H_

#include <stdio.h>
#include <list>
#include <algorithm>
#include <mutex>
#include <functional>

#include "PlayerData.h"
#include "GameEntity.fwd.h"

class VisibleController {
private:
	PGameEntity _parent;
	 
	bool _isVisible;

	std::list<uint16_t>* _visibleEntities;

	std::mutex* _rwlock;

	bool AddToVisible(const WORD entityId);
	void RemoveFromVisible(const WORD entityId);

	void EvaluateEntity(PGameEntity entity, WORD entityId);
	void EvaluateVisibles();
public:
	void Initialize(PGameEntity parent);
	void PerformUpdate();

	bool IsVisible();
	void SetVisible(bool visivility);

	std::list<uint16_t> GetEntities();

	bool SendToVisible(LPVOID packet, WORD packetSize, bool sendSelf = true);
	void ForEach(std::function<void(uint16_t, uint16_t)> procedure, uint16_t handleType = 0);
};

#endif