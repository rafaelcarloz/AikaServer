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

#pragma region "Skill Use Functions"

enum SkillFunction : uint32_t {
	AttackMeele,

	Slugger,
	ChargeDash,
	ArmorBreak,
	Incite,
	Berserker,
	BladeStrom,
	HowlOfGlory,
	Stark,
	Fortitude,
	DragonicBody,
	HowlOfValor,
	ImpactGround,
	HowlOfWar,
	Awakening,
	Thrust,
	HowlOfDefence,
	DeathWound,
	Halmont,
	WeaponGuard,

	RoteShield = 25,
	Remedy,
	Nemesis,
	MassIncite,
	Guardian,
	HolyBlood,
	LockOn,
	PointDefence,
	Punishment,
	Revelation,
	DivineUnion,
	Protect,
	DivineEmission,
	ShieldReflection,
	ElTycia,
	ElIaster,
	ELThymos,
	ELAegis,

	AttackShooter = 48,
	Countdown,
	FatalFlaw,
	HighAngleGun,
	LegShot,
	ShooterHiding,
	Najmun,
	CruelChaser,
	Precision,
	Concentration,
	CriticalPower,
	Detonation,
	FirstEncounter,
	ClosingTarget,
	SonicBlast,
	VitalSpot,
	MarkOfPursuer,
	X14Demolition,
	BlowBack,
	Elimination,
	X14Destruction = 69,

	Return = 72,
	Mjolnir,
	VenomSpain,
	PromptAction,
	Restraint,
	DualGunnerHiding,
	FireWildly,
	SlowPoison,
	Requiem,
	SharpGaze,
	GraceMovement,
	BlackRose,
	ManaPoison,
	BloodySmoke,
	InstanceShock,
	ShiningExplosion,
	AntiCurePoison,
	RapidFire,
	HeartRipper,
	Dodge,

	AttackMagic = 96,
	ChaosFlame,
	Change = 99,
	ShockWave,
	BlackShield,
	ChaosInferno,
	Hitch,
	Erode,
	ManaStorm,
	ManaHarmony,
	DarknessSwarm,
	Eclater,

	ChaosBlaze = 110,
	SleepingHaze,
	HandOfDarkness,
	MassTeleport,
	DarknessFall,


	Revitalize = 120,


	HolyBolt,
	Cure,
	MagicShield,
	Bless,
	HandOfCure,
	Resurrection,
	Haste,
	SunRay,
	HolyActivation,
	ManaActivation,
	MassCureShield,
	UgenesLux,
	ManaRelease,
	PreventionCure,
	ShieldOfNikita,
	FeedbackMagic,
	HolyStriking,
	Recovery,
	MagicSensor,
	Purification,


	SharpWeapon = 160,


	SummonMount = 163,

	Brave = 193,
	ElementalGuard,
	FlameStriking,
	FairyForm,
	Courage,
	FireThorns,
	BlazeEdge,
	BurningMagic,
	ManaStream = 201,


	LightingLance = 225,
	Pain = 250,

	FunFishing = 318,

	AcademySkill = 427,

	LavaBlood = 510,
};

#pragma endregion
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

	bool ContainsEffect(uint16_t effectId);
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
