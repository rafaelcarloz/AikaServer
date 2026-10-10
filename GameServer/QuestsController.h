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
	time_t AcceptedOn;
	time_t CompletedOn;
	std::vector<PlayerQuestGoal> Goals;
};

struct DatabaseQuest {
	uint64_t ID;
	time_t AcceptedOn;
	time_t CompletedOn;
	bool isCompleted;
};

struct QuestAditionalDrop {
	uint16_t QuestId;

	uint32_t MobId;
	uint32_t DropRate;
	Item DropItem;
};

class QuestsController {
private:
	long _characterId;

	PPlayer _player; 

	//database handler
	bool GetQuestsData();
	bool SaveQuestsData();
	void UpdateQuestDatabaseInfo(PlayerQuest questData);
	DatabaseQuest GetDatabaseQuest(uint16_t questId);

	//events
	void OnAcceptQuest(uint16_t questId);
	void OnFinishQuest(uint16_t questId);
	void OnDeliverQuest(uint16_t questId);
	void OnCancelQuest(uint16_t questId);

	//quest helpers
	PlayerQuestGoal ParseQuestGoal(QuestDetailData questDetail);
	void AddQuestDrops(uint16_t questId);
	void RemoveQuestDrops(uint16_t questId);
public:
	std::vector<uint16_t> completedQuests;
	std::vector<uint16_t> availableQuests;

	std::map<uint16_t, PlayerQuest> activeQuests;

	std::map<uint16_t, DatabaseQuest> questsDatabase;

	std::vector<QuestAditionalDrop> questDrops;

	//initialization
	void Initialize(long characterId, PPlayer player);
	bool Destroy();


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
	bool CancelQuest(uint16_t questId);

	void SendQuestUpdate(PlayerQuest quest, bool isNew = false);
	void SendQuestRemove(uint16_t questId);
	void SendQuestRewardChoice(uint16_t questId);
	void SendQuests();
};

#endif