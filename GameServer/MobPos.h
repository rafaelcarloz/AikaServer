#pragma once

#ifndef _MOB_POS_H_
#define _MOB_POS_H_

#include <stdio.h>
#include <map>
#include <mutex>

struct MobPosItem {
	uint32_t X;
	uint32_t Y;
};

typedef struct MobPosFromList {
	uint32_t MobIndex;
	char MobName[12];
	MobPosItem position[50];
}*PMobPosFromList;

class MobPos {
private:
	static std::atomic<MobPos*> _instance;
	static std::mutex _mutex;

	std::map<uint16_t, MobPosFromList> _items;
protected:
	MobPos() {}
	~MobPos() {}
public:
	MobPos(MobPos& other) = delete;
	void operator=(const MobPos&) = delete;

	static MobPos* GetInstance();

	bool Add(const MobPosFromList item, uint16_t mobId);
	bool Get(const uint16_t mobId, MobPosFromList* item);
	static MobPosFromList Get(const uint16_t mobId);

	bool SaveToJSON();
};

#endif