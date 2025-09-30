#pragma once

#ifndef _QUESTS_CONTROLLER_H_
#define _QUESTS_CONTROLLER_H_

#include <vector>
#include <map>

#include "Player.fwd.h"
#include "Quests.h"

struct PlayerQuestGoal {
	uint16_t Type;
	uint16_t Index;
	uint16_t RequiredAmount;
	uint16_t CompletedAmount;
};

struct PlayerQuest {
	uint16_t Index;
	char Title[64];
	bool IsCompleted;
	std::vector<PlayerQuestGoal> Goals;
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

	//evaluation routines
	
public:
	std::vector<uint16_t> completedQuests;
	std::vector<uint16_t> availableQuests;

	std::map<uint16_t, PlayerQuest> activeQuests;

	//initialization
	void Initialize(long characterId, PPlayer player);


	bool AvailableContains(uint16_t questId);
	bool CompletedContains(uint16_t questId);
	bool ActiveContains(uint16_t questId);

	uint16_t GetNpcQuestIcon(uint16_t npcId);

	//evaluation routines
	void Evaluate();
	void EvaluateQuestsGoals();
	bool EvaluateQuestAvailability(uint16_t questId);
	bool EvaluateQuestAvailability(QuestData questData);

	//quest goals
	void EvaluateGoalMonsterKill(uint16_t mobIndex);


	bool AcceptQuest(uint16_t questId);
	bool DeliverQuest(uint16_t questId, int8_t rewardSlotSelected = -1);
	bool SelectQuestReward(uint16_t questId, uint16_t slot);


	void SendQuestUpdate(PlayerQuest quest);
	void SendQuestRemove(uint16_t questId);
	void SendQuestRewardChoice(uint16_t questId);
};

#endif