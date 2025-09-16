#pragma once

#ifndef _EXP_LIST_H_
#define _EXP_LIST_H_

#include <stdio.h>
#include <map>
#include <mutex>

#pragma region "ItemList Data"

#pragma endregion

class ExpList {
private:
	static std::atomic<ExpList*> _instance;
	static std::mutex _mutex;

	std::map<uint16_t, uint64_t> _items;

	uint16_t _itemCount = 0;
protected:
	ExpList() {}
	~ExpList() {}
public:
	ExpList(ExpList& other) = delete;
	void operator=(const ExpList&) = delete;

	static ExpList* GetInstance();

	bool Add(const uint64_t item);
	bool Get(const uint16_t level, uint64_t* item);
	static uint64_t Get(const uint16_t level);
	bool Contains(const uint16_t level);

	uint64_t operator[](const uint16_t level);
};

#endif
