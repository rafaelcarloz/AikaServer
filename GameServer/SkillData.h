#pragma once

#ifndef _SKILL_DATA_H_
#define _SKILL_DATA_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdio.h>
#include <map>

#pragma pack(push,1)


#pragma region "SkillData File"

typedef struct SkillFromList {
	uint32_t Index,
		MinLevel,
		Null_0,
		Level,
		Classification;
	char EnglishName[64];
	char Name[64];

	DWORD SkillPoints;
	DWORD LearnCosts;
	DWORD Classe;

	BYTE UNK_0[12];

	DWORD MagicPointsRequired;

	DWORD NULL_1;

	DWORD PreCooldown;
	DWORD Cooldown;
	DWORD CooldownType;
	DWORD TargetType;
	DWORD UniqueType;
	DWORD MaxTargets;
	DWORD UNK_2;
	DWORD Range;
	DWORD DamageRange;
	DWORD SuccessRate;
	DWORD Agressive;
	DWORD BuffDebuff;
	DWORD Attribute;
	BYTE UNK_3[16];
	DWORD Damage;
	DWORD Adicional;
	DWORD UNK_4;

	DWORD EF[4];
	DWORD EFV[4];

	DWORD Duration;

	BYTE UNK_5[24];
	DWORD CastTime;
	BYTE UNK_6[24];
	DWORD TargetAnimation;
	DWORD UNK_7;
	DWORD Animation;
	BYTE UNK_8[68];
	char Description[288];
}*PSkillFromList;

#pragma endregion

class SkillData {
private:
	static SkillData* _instance;

	std::map<WORD, SkillFromList> _items;

	WORD _itemCount = 0;
protected:
	SkillData() {}
	~SkillData() {}
public:
	SkillData(SkillData& other) = delete;
	void operator=(const SkillData&) = delete;

	static SkillData* GetInstance();

	bool Add(const SkillFromList item);
	bool Get(const WORD itemId, SkillFromList* item);
	static SkillFromList Get(const WORD itemId);

	SkillFromList operator[](const WORD itemId);

	BYTE GetSendToWorldSkill(WORD skillId, DWORD& skillValue);
};

#pragma pack(pop)

#endif
