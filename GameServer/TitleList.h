#pragma once

#ifndef _TITLE_LIST_H_
#define _TITLE_LIST_H_

#include <stdio.h>
#include <map>
#include <mutex>

typedef struct TitleLevel {
	bool HideTitle;
	uint8_t TitleType;
	uint16_t TitleGoal;
	uint16_t EF[3], EFV[3];
	char TitleName[32];
	uint8_t TitleCategory;
	uint8_t TitleIndex;
	uint16_t NULL_1;
	uint32_t TitleColor;
}*PTitleLevel;

typedef struct TitleFromList {
	TitleLevel Level[4];
}*PTitleFromList;

class TitleList {
private:
	static std::atomic<TitleList*> _instance;
	static std::mutex _mutex;

	std::map<uint16_t, TitleFromList> _items;

	uint16_t _itemCount = 0;
protected:
	TitleList() {}
	~TitleList() {}
public:
	TitleList(TitleList& other) = delete;
	void operator=(const TitleList&) = delete;

	static TitleList* GetInstance();

	bool Add(const TitleFromList item);
	bool Get(const uint16_t itemId, TitleFromList* item);
	static TitleFromList Get(const uint16_t itemId);
};

#endif