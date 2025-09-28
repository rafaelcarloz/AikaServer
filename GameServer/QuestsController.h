#pragma once

#ifndef _QUESTS_CONTROLLER_H_
#define _QUESTS_CONTROLLER_H_

#include <vector>
#include <map>

#include "Player.fwd.h"
#include "Quests.h"

struct PlayerQuest {
	uint16_t Index;
	char Title[64];
	bool IsCompleted;
	std::vector<QuestDetailData> Goals;
};

class QuestsController {
private:
	long _characterId;

	PPlayer _player; 

	//database handler
	bool GetQuestsData();
	bool SaveQuestsData();

	//events
	void OnAcceptQuest(uint16_t questId);
	void OnFinishQuest(uint16_t questId);
	void OnDeliverQuest(uint16_t questId);
	void OnCancelQuest(uint16_t questId);
public:
	std::vector<uint16_t> completedQuests;
	std::vector<uint16_t> availableQuests;

	std::map<uint16_t, PlayerQuest> activeQuests;

	//initialization
	void Initialize(long characterId, PPlayer player);


	bool AvailableContains(uint16_t questId);
	bool CompletedContains(uint16_t questId);

	//evaluation routines
	void Evaluate();
	bool EvaluateQuestAvailability(uint16_t questId);
	bool EvaluateQuestAvailability(QuestData questData);


	bool AcceptQuest(uint16_t questId);


	void SendQuestUpdate(PlayerQuest quest);
};

#endif