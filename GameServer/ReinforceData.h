#pragma once

#ifndef _REINFORCE_DATA_H_
#define _REINFORCE_DATA_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdio.h>
#include <map>

#pragma region "Reinforce Files"

#pragma region "Reinforce2"

typedef struct ItemAttributeReinforce {
	DWORD AttributePhysic[16];
	DWORD AttributeMagic[16];
}*PItemAttributeReinforce;

enum Reinforce2_Area_
{
	Reinforce2_Area_Blade = 0,
	Reinforce2_Area_Sword = 35,
	Reinforce2_Area_Pistol = 70,
	Reinforce2_Area_Rifle = 105,
	Reinforce2_Area_Wand = 140,
	Reinforce2_Area_Staff = 175,

	Reinforce2_Area_Helmet = 210,
	Reinforce2_Area_Armor = 390,
	Reinforce2_Area_Gloves = 570,
	Reinforce2_Area_Shoes = 750,

	Reinforce2_Area_Shield = 930,
};

const unsigned reinforce2sectionSize = 960u;

#pragma endregion
#pragma region "Reinforce3"

typedef struct ArmorAttributeReinforce {
	DWORD DamageReduction[16];
	DWORD HealthIncrementPoints[16];
}*PArmorAttributeReinforce;

enum Reinforce3_Area_
{
	Reinforce3_Area_Helmet = 0,
	Reinforce3_Area_Shield = Reinforce3_Area_Helmet,

	Reinforce3_Area_Armor = 30,
	Reinforce3_Area_Gloves = 60,
	Reinforce3_Area_Shoes = 90,
};

const unsigned reinforce3sectionSize = 150u;

#pragma endregion

#pragma endregion

class ReinforceData {
private:
	static ReinforceData* _instance;

	std::map<WORD, ItemAttributeReinforce> _reinforce2;
	std::map<WORD, ArmorAttributeReinforce> _reinforce3;

	WORD _reinforce2Count = 0;
	WORD _reinforce3Count = 0;

	WORD GetReinforce2Index(const WORD itemId);
	WORD GetReinforce3Index(const WORD itemId);
protected:
	ReinforceData() {}
	~ReinforceData() {}
public:
	ReinforceData(ReinforceData& other) = delete;
	void operator=(const ReinforceData&) = delete;

	static ReinforceData* GetInstance();

	bool AddReinforce2(const ItemAttributeReinforce item);
	bool GetReinforce2(const WORD itemId, ItemAttributeReinforce* item);
	ItemAttributeReinforce GetReinforce2(const WORD itemId);

	bool AddReinforce3(const ArmorAttributeReinforce item);
	bool GetReinforce3(const WORD itemId, ArmorAttributeReinforce* item);
	ArmorAttributeReinforce GetReinforce3(const WORD itemId);
};

#endif