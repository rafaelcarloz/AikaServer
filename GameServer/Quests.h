#pragma once

#ifndef _QUESTS_H_
#define _QUESTS_H_

#include <stdint.h>

#include "ItemData.h"
#include <map>
#include <vector>
#include <atomic>
#include <mutex>

#pragma pack(push,1)

enum QuestDetailType : uint32_t {
	QuestDetailNone,
	QuestDetailMonster,
	QuestDetailItem,

	QuestDetailLevel = 4,

	QuestDetailQuest = 9,

	QuestDetailItemChoice = 13,
	QuestDetailExp,
	QuestDetailGold,

	QuestDetailEngageNPC = 19,

	QuestDetailPran = 29
};

struct QuestDetailData {
	QuestDetailType TypeID;
	uint32_t Quantity1;
	uint32_t Quantity2;
	uint32_t UNK_1;

	Item Item;
};


struct QuestData {
	uint16_t Index;
	uint16_t UNK_1;
	uint32_t StartNPC;
	uint32_t EndNPC;
	uint32_t UNK_2;
	char Title[64];
	uint16_t DialogStart;
	uint16_t DialogUnfinished;
	uint16_t DialogEnd;
	uint16_t Summary;
	uint8_t UNK_3[1155];
	uint8_t Level;
	uint16_t UNK_VAL;
	uint8_t IsPlotQuest;
	uint8_t UNK_Effect1;
	uint8_t UNK_Effect2;
	uint8_t UNK_Effect3;
	uint16_t UNK_7;

	QuestDetailData PreConditions[5];
	QuestDetailData Requirements[5];
	QuestDetailData Rewards[8];
	QuestDetailData DeleteOnComplete[5];
	QuestDetailData AcceptRewards[3];
	QuestDetailData DropDetails[4];

	uint16_t UnlockedByQuestId();
	bool IsItemChoiceReward();
};


class QuestList {
private:
	static std::atomic<QuestList*> _instance;
	static std::mutex _mutex;

	std::map<WORD, QuestData> _items;

	WORD _itemCount = 0;
protected:
	QuestList() {}
	~QuestList() {}
public:
	std::map<uint16_t, std::vector<uint16_t>> unlockQuests;

	QuestList(QuestList& other) = delete;
	void operator=(const QuestList&) = delete;

	static QuestList* GetInstance();

	bool Add(const QuestData item);
	bool Get(const WORD itemId, QuestData* item);
	static QuestData Get(const uint16_t itemId);
	bool Contains(const WORD itemId);
	std::map<WORD, QuestData> GetQuests();

	bool AddUnlocks(const uint16_t questId, const uint16_t unlocksId);
	bool ContainsUnlocks(const WORD questId);
	std::vector<uint16_t> GetUnlocks(const uint16_t questId);

	QuestData operator[](const WORD itemId);
};



#endif