#pragma once

#ifndef _PLAYER_DATA_H_
#define _PLAYER_DATA_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "ItemData.h"

#pragma pack(push,1)

enum class Citizenship : BYTE {
	None,
	Server1,
	Server2,
	Server3,
	Server4,
	Server5
};

typedef enum PlayerStatus {
	WaitingLogin,
	CharList,
	Numeric,
	Waiting,
	Playing
}*PPlayerStatus;

#pragma region "Character Basic Info"

enum class CharacterClass : BYTE
{
	Warrior = 1,
	Gladiator,
	Paladin = 11,
	Crusader,
	Rifleman = 21,
	None3,
	DualGunner = 31,
	None4,
	Warlock = 41,
	None5,
	Cleric = 51,
	None6
};

struct CharacterSize {
	BYTE Altura;
	BYTE Tronco;
	BYTE Perna;
	BYTE Corpo;
};

struct CharacterAttributes {
	WORD Strength;
	WORD Dexterity;
	WORD Intelligence;
	WORD Constitution;
	WORD Spirit;
	WORD Status;
};

struct CharacterLife {
	DWORD MaxHP, CurHP;
	DWORD MaxMP, CurMP;
};

struct CharacterDamage {
	WORD PhysDamage, PhysDefense;
	WORD MagDamage, MagDefense;
	WORD BonusDamage;
};

struct CharacterStatus {
	CharacterAttributes Attributes;
	CharacterSize Sizes;
	CharacterLife Life;
	DWORD ServerReset;
	DWORD HonorPoints;
	DWORD KillPoints;
	DWORD ChaosTime;
	WORD EvilPoints;
	WORD SkillPoints;
	WORD UNK_1;
	BYTE NULL_1[60];
	WORD UNK_2;
	CharacterDamage Damage;
	BYTE NULL_2[10];
	WORD CriticalRate;
	BYTE DodgeRate;
	BYTE HitRate;
};


struct Quest {
	WORD Index;
	BYTE UNK_1[10];
};

typedef struct Title {
	BYTE Index;
	BYTE Level;
	WORD Progress;

	DWORD GetLevelValue(BYTE slot);
}*PTITLE;

#pragma endregion
#pragma region "Character Data"

typedef struct CharacterSkill {
	UINT64 databaseID;
	WORD skillID;
}*PCharacterSkill;

typedef struct CharacterItemBar {
	UINT64 databaseID;
	DWORD itemID;
}*PCharacterItemBar;

typedef struct CharacterTitle {
	UINT64 databaseID;
	Title title;
}*PCharacterTitle;


typedef struct Character {
	UINT64 CharacterId;
	char Name[16];
	CharacterClass ClassInfo;
	CharacterStatus Status;
	UINT64 ExpPoints;
	WORD Level;
	WORD GuildIndex;

	Item Equipaments[16];
	UINT64 EquipamentsDB[16];
	Item Inventory[64];
	UINT64 InventoryDB[64];

	UINT64 Gold;

	CharacterSkill SkillList[60];
	CharacterItemBar ItemBar[24];
	CharacterTitle Titles[96];

}*PCharacter;

#pragma pack(pop)

#endif