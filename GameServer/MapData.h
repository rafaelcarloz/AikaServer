#pragma once

#ifndef _MAP_DATA_H_
#define _MAP_DATA_H_

#include <stdio.h>
#include <map>
#include <mutex>

#include "MiscData.h"

#pragma pack(push,1)

typedef struct MapBounds {
	uint16_t StartPositionX;
	uint16_t StartPositionY;
	uint16_t EndPositionX;
	uint16_t EndPositionY;
}*PMapBounds;

typedef struct ScrollTeleportPosition {
	uint32_t Index;
	uint16_t PositionX;
	uint16_t PositionY;
	uint16_t MapIndex;
	uint16_t LocationIndex;
	uint32_t null_1;
	char MapName[64];
	char LocationName[64];

	Position ToPosition();
}*PScrollTeleportPosition;


class MapData {
private:
	static std::atomic<MapData*> _instance;
	static std::mutex _mutex;

	std::map<uint16_t, ScrollTeleportPosition> _teleports;
	std::map<uint16_t, MapBounds> _bounds;

	uint16_t _teleportCount = 0;
	uint16_t _boundsCount = 0;
protected:
	MapData() {}
	~MapData() {}
public:
	MapData(MapData& other) = delete;
	void operator=(const MapData&) = delete;

	static MapData* GetInstance();

	bool AddTeleport(const ScrollTeleportPosition item);
	bool GetTeleport(const uint16_t itemId, ScrollTeleportPosition* item);
	static ScrollTeleportPosition GetTeleport(const uint16_t itemId);

	bool AddBounds(const MapBounds item);
	bool GetBounds(const uint16_t itemId, MapBounds* item);
	static MapBounds GetBounds(const uint16_t itemId);
};


#pragma pack(pop)

#endif