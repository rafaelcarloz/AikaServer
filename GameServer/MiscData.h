#pragma once
#ifndef _MISC_DATA_H_
#define _MISC_DATA_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma pack(push,1)

#pragma region "Position Data"

typedef struct Position {
	float X;
	float Y;

public:
	Position() = default;
	Position(float x, float y);

	WORD Distance(Position pos);
	static WORD Distance(Position pos1, Position pos2);
} *PPosition;

#pragma endregion
#pragma region "Spawn Data"

enum SpawnType : BYTE {
	SpawnNormal,
	SpawnTeleport,
	SpawnBabyGen
};

enum DeleteType : BYTE {
	DeleteNormal,
	DeleteDead,
	DeleteDisconnect,
	DeleteUnspawn
};

#pragma endregion

#endif