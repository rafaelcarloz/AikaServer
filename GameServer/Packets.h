#pragma once

#ifndef _PACKETS_H_
#define _PACKETS_H_

#include "PlayerData.h"

#pragma pack(push,1)

namespace packets{

#pragma region "Header and Basics"

	typedef struct PacketHeader
	{
		WORD Size;
		BYTE Key;
		BYTE ChkSum;
		WORD Index, Code;
		DWORD Time;
	}*PPacketHeader;

	typedef struct PacketSignalData {
		PacketHeader Header;
		DWORD Data;
	}*PPacketSignalData;

#pragma endregion
#pragma region "Login Packets"

	typedef struct CheckTokenPacket {
		PacketHeader Header;
		char Username[32];
		char Token[32];
		char NULL_1[1040];
	}*PPacket_81;

	typedef struct ResponseLoginPacket {
		PacketHeader Header;
		DWORD Index;
		DWORD Time;
		DWORD Nation;
		DWORD NULL_1;
	}*PPacket_82;

	typedef struct PacketLoginRequest
	{
		PacketHeader Header;
		DWORD AccountIndex;
		char Username[32];
		DWORD Time;
		char MacAddress[14];
		WORD Version;
		DWORD NULL_1;
		char Token[32];
		BYTE NULL_2[992];
	}*PPacket_685;

#pragma endregion
#pragma region "Player Message Packets"

	enum class MESSAGE_COLOR : BYTE {
		WHITE,
		YELLOW = 16,
	};

	enum class MESSAGE_TYPE : BYTE {
		NORMAL,
		UPPER_MSG = 16,
		GM_MSG = 32,
		GM_BROADCAST = 48
	};

	typedef struct PacketSendMessage {
		PacketHeader Header;
		MESSAGE_COLOR Color;
		MESSAGE_TYPE Type;
		BYTE NULL_1;
		BYTE NULL_2;
		char Message[128];
	}*PPacket_984;

#pragma endregion
#pragma region "Character List Packets"

	typedef struct CharacterListData {
		char Name[16];
		WORD Nation;
		CharacterClass ClassInfo;
		BYTE NULL_2;
		CharacterSize Sizes;
		WORD Equipaments[8];
		BYTE ItemEffects[12];
		CharacterAttributes Attributes;
		WORD Level;
		BYTE NULL_3[6];
		UINT64 Exp;
		UINT64 Gold;
		BYTE NULL_4[4];
		DWORD DeleteTime;
		BYTE NumericError;
		bool NumRegistered;
		BYTE NULL_5[6];
	} *PCharacterListData;

	typedef struct PacketCharacterList {
		PacketHeader Header;
		DWORD AccountIndex;
		DWORD NULL_1;
		DWORD NULL_2;
		CharacterListData CharactersData[3];
	}*PPacket_901;


	typedef struct PacketCharacterCreate {
		PacketHeader Header;
		DWORD AccountIndex;
		DWORD SlotIndex;
		char Name[16];
		WORD Face;
		WORD Hair;
		BYTE NULL_1[12];
		DWORD Local;
	}*PPacket_3E04;


	typedef struct PacketCharacterDelete {
		PacketHeader Header;
		DWORD AcountIndex, SlotIndex;
		BOOL CancelDelete;
		char NumericToken[4];
	}*PPacket_603;

	typedef struct PacketCharacterDeletePermanent {
		PacketHeader Header;
		DWORD AcountIndex, SlotIndex;
		char NumericToken[4];
		BYTE NULL_1[32];
	}*PPacket_3E01;

#pragma endregion
#pragma region "Enter Ingame Packets"

	typedef struct PacketAHackStatusCheck {
		PacketHeader Header;
		DWORD Status;
	}*PPacket_39D;

	enum class NumericRequestType : DWORD {
		NumericRegister = 0,
		NumericCheck,
		NumericChange
	};

	typedef struct PacketNumericToken {
		PacketHeader Header;
		DWORD SlotIndex; 
		NumericRequestType RequestChange;
		char Numeric_1[4];
		char Numeric_2[4];
	}*PPacket_F02;

	typedef struct PacketSendToWorld {
		PacketHeader header;
		DWORD accountId;

		DWORD clientId;
		DWORD FirstLogin;
		DWORD characterId;
		char name[16];
		Citizenship nation;
		CharacterClass characterClass;
		WORD NULL_1;
		CharacterStatus CurrentScore;
		DWORD NULL_2;
		UINT64 ExpPoints;
		WORD Level;
		WORD GuildIndex;
		BYTE NULL_3[32];
		WORD buffsId[20];
		DWORD buffTimes[20];
		Item Equipaments[16];
		DWORD NULL_4;
		Item Inventory[64];
		UINT64 Gold;
		BYTE UNK_2[192];
		Quest Quests[16];
		BYTE UNK_3[212];
		DWORD UNK_4;
		DWORD Location;
		BYTE UNK_5[128];
		DWORD CreationTime;
		BYTE UNK_6[436];
		char NumericToken[4];
		BYTE UNK_7[212];
		WORD SkillList[60];
		DWORD ItemBar[24];
		DWORD NULL_5;
		DWORD TitleCategoryLevel[12];
		BYTE UNK_8[80];
		WORD ActiveTitle;
		DWORD NULL_6;

		WORD TitleProgressType8[48];
		WORD TitleProgressType9[2];
		WORD TitleProgressType4;
		WORD TitleProgressType10;
		WORD TitleProgressType7;
		WORD TitleProgressType11;
		WORD TitleProgressType12;
		WORD TitleProgressType13;
		WORD TitleProgressType15;
		WORD TitleProgress_UNK;
		WORD TitleProgressType16[22];
		WORD TitleProgressType23;

		WORD TitlesProgress[120];

		DWORD EndDayTime;
		DWORD NULL_7;
		DWORD UNK_10;
		BYTE NULL_8[52];
		DWORD UTC;
		DWORD LoginTime;
		BYTE UNK_11[12];
		char PranName[2][16];
		DWORD UNK_12;
	}*PPacket_925;


#pragma endregion
#pragma region "Spawn & Unspawn Packets"

	struct ActiveTitle {
		BYTE Index;
		BYTE Level;
	};

	typedef struct PacketCreateMob {
		PacketHeader Header;
		char Name[16];
		WORD Equipaments[8];
		BYTE ItemEffects[12];
		Position CurrentPosition;
		DWORD Rotation;
		DWORD MaxHP, MaxMP;
		DWORD CurHP, CurMP;
		BYTE  UNK_1;
		BYTE SpeedMove;
		BYTE SpawnType;
		CharacterSize Sizes;
		BYTE IsService;
		WORD EffectType;
		WORD SetBuff;
		WORD Buffs[60];
		DWORD BuffsTime[60];
		char Title[32];
		BYTE GuildIndex;
		Citizenship Nation;
		WORD Effects[4];
		BYTE UNK_2;
		BYTE ChaosPoint;
		DWORD NULL_1;
		ActiveTitle ActiveTitle;
		WORD NULL_2;
	}*PPacket_349;


	typedef struct PacketRemoveMob {
		PacketHeader Header;
		DWORD Index;
		DWORD DeleteType;
	}*PPacket_101;

	typedef struct PacketRequestMobInfoformation {
		PacketHeader Header;
		DWORD Index;
		DWORD MobType;
	}*PPacket_306;

#pragma endregion
#pragma region "Movemment Packets"

	enum MovemmentType : BYTE {
		MovemmentNormal = 0,
		MovemmentTeleport = 1,
		MovemmentGenerateSummon = 8
	};

	typedef struct PacketMovemment {
		PacketHeader Header;
		Position Destination;
		BYTE NULL_1[6];
		MovemmentType MoveType;
		BYTE Speed;
		DWORD NULL_2;
	}*PPacket_301;

	typedef struct PacketCharacterAction {
		PacketHeader Header;
		DWORD actionId;
		DWORD null_1;
	}*PPacket_304;

#pragma endregion
#pragma region "Inventory Packets"

	typedef struct PacketMoveItem {
		PacketHeader Header;
		SlotType DestType;
		WORD DestSlot;
		SlotType SrcType;
		WORD SrcSlot;
	}*PPacket_70F;


	typedef struct PacketRefreshItem {
		PacketHeader Header;
		BYTE Notice;
		BYTE SlotType;
		WORD Slot;
		Item Item;
	}*PPacket_F0E;

	typedef struct PacketJoinItems {
		PacketHeader Header;
		DWORD SrcSlot;
		DWORD DestSlot;
	}*PPacket_332;

	typedef struct PacketSplitItem {
		PacketHeader Header;
		DWORD Slot;
		DWORD Amount;
		DWORD SlotType;
	}*PPacket_333;

	typedef struct PacketDeleteItem {
		PacketHeader Header;
		DWORD Slot;
		DWORD SlotType;
	}*PPacket_32C;


	typedef struct PacketUseItem {
		PacketHeader Header;
		DWORD TypeSlot;
		DWORD Slot;
		DWORD Type1;
	}*PPacket_31D;

#pragma endregion
#pragma region "Score & Status Packets"

	typedef struct PacketCharacterLife {
		PacketHeader Header;
		CharacterLife Life;
		DWORD NULL_1;
	}*PPacket_103;

	typedef struct PacketCharacterSkillsLevel {
		PacketHeader Header;
		WORD Skills[60];
		WORD SkillPoints;
		WORD NULL_1;
	}*PPacket_107;

	typedef struct PacketCharacterLevel {
		PacketHeader Header;
		WORD Level;
		WORD NULL_1;
		UINT64 ExpPoints;
	}*PPacket_108;

	typedef struct PacketCharacterAttributtes {
		PacketHeader Header;
		CharacterAttributes Attributes;
		WORD StatusPoint;
		WORD SkillsPoint;
	}*PPacket_109;

	typedef struct PacketCharacterStatus {
		PacketHeader Header;
		CharacterDamage Damage;
		DWORD UNK_1;
		WORD SpeedMove;
		WORD UNK_2;
		BYTE UNK_3[6];
		WORD CriticalRate;
		BYTE DodgeRate, HitRate;
		WORD DoubleRate, ResistHate;
	}*PPacket_10A;


#pragma endregion


}

#pragma pack(pop)

#endif
