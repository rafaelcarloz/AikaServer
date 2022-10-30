#pragma once

#ifndef _CHARACTER_CONTROLLER_H_
#define _CHARACTER_CONTROLLER_H_

#include "PlayerData.h"
#include "CharacterController.fwd.h"
#include "Player.fwd.h"
#include "MiscData.h"
#include "Packets.h"
#include "InventoryController.h"
#include "StatusController.h"
#include "VisibleController.h"

class CharacterController {
private:
	long _characterId;
	PPlayer _player;

	char _numericToken[4];

	time_t _CreatedOn;
	time_t _LoggedOn;

	bool _firstLogin;

	bool GetCharacterData();
	bool GetSkillsData();
	bool GetHotBarData();

	boost::json::value CharacterToJSON();

	bool SaveCharacterData();
	bool SaveInventoryData();
	bool SaveSkillsData();
	bool SaveHotBarData();
	bool SaveBuffsData();
public:

	Title activeTitle;

	Position savedPosition;

	bool activePlayerKill;

	void Initialize(long characterId, PPlayer player);
	bool SaveCharacter();

	bool SendToWorld();
	bool EnterIngame();

	void RespawnCharacter(bool sendSelf = false, SpawnType spawnType = SpawnNormal);
	void UnspawnCharacter(bool sendSelf = false, DeleteType deleteType = DeleteNormal);

	void Teleport(Position position);
	void Teleport(int positionX, int positionY);

	void SendToSavedPosition();

	void SendItemBarSlot(BYTE slot, BYTE destType = 0, WORD index = 0);
};


#endif
