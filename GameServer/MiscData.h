#pragma once
#ifndef _MISC_DATA_H_
#define _MISC_DATA_H_

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
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
#pragma region "Damage Data"

enum DamageScope : BYTE {
	DamagePhysical,
	DamageMagical
};

enum DamageType : BYTE {
	Normal,
	Critical,
	Double,
	DoubleCritical,
	Immune,
	ImmuneCritical,
	ImmuneDouble,
	ImmuneDoubleCritical,
	Miss,
	MissCritical,
	MissDouble,
	MissCritical2,
	Miss2,
	Miss2Critical,
	Miss2Double,
	Miss2Critical2,
	Block,
	BlockCritical,
	BlockDouble,
	BlockCritical2,
	Immune2,
	Immune2Critical,
	Immune2Double,
	Immune2Critical2,
	Miss3,
	Miss3Critical,
	Miss3Double,
	Miss3Critical2,
	Miss4,
	Miss4Critical,
	Miss4Double,
	Miss4Critical2,
	NA
};

struct EntityDamage {
	UINT64 DamageValue;
	DamageScope DamageScope;
	DamageType DamageType;
};

#pragma endregion

#endif