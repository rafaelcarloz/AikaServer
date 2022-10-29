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

struct SkillFromList {
	DWORD Null_1,
		MinLevel,
		Null_3,
		Level,
		Null_4;
	char EnglishName[64];
	char Name[64];
	DWORD SkillPoints;
	DWORD LearnCosts;
	DWORD Classe;
	BYTE UNK_1[12];
	DWORD MagicPointsRequired;
	BYTE UNK_2[8];
	DWORD Cooldown;
	DWORD NULL_1;
	DWORD Range;
	BYTE UNK_3[52];
	DWORD Damage;
	DWORD Adicional;
	DWORD UNK_4;
	DWORD EF[4];
	DWORD EFV[4];
	INT32 Duration;
	BYTE UNK_5[24];
	DWORD CastTime;
	BYTE UNK_6[24];
	DWORD TargetAnimation;
	BYTE UNK_7[76];
	char Description[288];
};

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
	SkillFromList Get(const WORD itemId);

	SkillFromList operator[](const WORD itemId);

	BYTE GetSendToWorldSkill(WORD skillId, DWORD& skillValue);
};

#pragma pack(pop)

#endif
