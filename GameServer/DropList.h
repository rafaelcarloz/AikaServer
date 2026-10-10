#pragma once

#ifndef _DROP_LIST_H_
#define _DROP_LIST_H_

#include <stdio.h>
#include <map>
#include <vector>
#include <mutex>

#pragma region "DropList Data"

#pragma endregion

enum DropRarity : uint16_t {
	DROP_NORMAL = 1,
	DROP_SUPERIOR,
	DROP_RARE,
	DROP_LEGENDARY
};

enum DropRarityChance : uint16_t {
	DROP_NORMAL_CHANCE = 50,
	DROP_SUPERIOR_CHANCE = 10,
	DROP_RARE_CHANCE = 5,
	DROP_LEGENDARY_CHANCE = 3
};

struct DropItem {
	uint16_t itemId;
	DropRarity dropRarity;
	DropRarityChance dropRarityChance;
};

struct DropItems {
	std::vector<DropItem> items;
};

class DropList {
private:
	static std::atomic<DropList*> _instance;
	static std::mutex _mutex;

	std::map<uint16_t, DropItems> _items;
protected:
	DropList() {}
	~DropList() {}
public:
	DropList(DropList& other) = delete;
	void operator=(const DropList&) = delete;

	static DropList* GetInstance();

	bool Add(const uint16_t index, const DropItems item);
	bool Get(const uint16_t index, DropItems* item);
	static DropItems Get(const uint16_t index);

	bool Contains(const uint16_t index);

	DropItems operator[](const uint16_t index);
};

#endif
