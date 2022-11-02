#pragma once

#ifndef _PACKETS_H_
#define _PACKETS_H_

#include <iostream>

#include "PlayerData.h"
#include "MiscData.h"

#pragma pack(push,1)

namespace packets{

	enum class PacketCode : WORD {
		CheckTokenPacket = 0x81,
		ResponseLoginPacket,

		PacketRemoveMob = 0x101,
		PacketDisplayDamage,
		PacketCharacterLife,

		PacketNPCShop = 0x106,
		PacketCharacterSkillsLevel,
		PacketCharacterLevel,
		PacketUpdateAttributes,
		PacketUpdateStatus,
		PacketUpdateBuffsAll,
		PacketUnknow10C,
		PacketUnknow10D, //pacote de pontos de exclamacao das quest
		PacketNPCTalk,
		PacketCloseNPC,
		PacketUnknow110,
		PacketAnimationTalk,
		PacketShowNPCOption,

		PacketSoundPlay = 0x115, //?? n é ctz
		PacketPranLevel,
		PacketClientIndex,

		PacketDungeonDialog = 0x119,

		PacketGiveExpGold = 0x11B,
		PacketUnknow11C,
		PacketPartyMemberPosition,

		PacketUnknow11F = 0x11F,

		PacketCastleStatus = 0x121,
		PacketUnknow122, //para o som
		PacketUnknow123, //resultado guerra castelo som

		PacketGuildAddPlayer = 0x125,

		PacketUnknow127 = 0x127,
		PacketUnknow128,

		PacketUpdateHonorPvP = 0x12A,
		PacketUpdateDurability,
		PacketSendSkills,

		PacketPlayerGuild = 0x130,
		PacketUnknow131,

		PacketReliques = 0x136,
		PacketStorage,
		PacketPremiumStorage,

		PacketBattlefildLobby = 0x13C,

		PacketBattleTimer = 0x142,

		PacketDungeonLobby = 0x145,

		PacketAccountStatus = 0x14F,

		PacketUpdateClientId = 0x150,
		PacketSendOnlineTime, //msg verde do tempo online embaixo na tela
		PacketUnknow152,
		PacketGiveExperience,
		PacketUnknow154,

		PacketUnknow160 = 0x160, //resposta de algum tipo de aprimoramento

		PacketUnknow16B = 0x16B,

		PacketUnknow16D = 0x16D, //msg "Desmontado" e dados aleatorios
		PacketUpdateBuffs,
		PacketUpdateBuff,
		PacketUnknow170,

		PacketTitleList = 0x17D,

		PacketUnknow186, //on send to world ??

		PacketCharacterViewRequest = 0x19E,

		PacketDuelResult = 0x1A0,

		PacketDuelTimer = 0x1A2,

		PacketRaidGiveLeader = 0x207,

		PacketGetStatusPoint = 0x213,

		PacketBuffItem = 0x21B,

		PacketUnknow227 = 0x227,

		PacketCharacterViewResponse = 0x22C,
		PacketChatItem,

		PacketUnknow233 = 0x233,

		PacketMovemment = 0x301,
		PacketAttackTarget,
		PacketRevive,
		PacketCharacterAction,
		PacketCharacterRotation,
		PacketRequestMobInfoformation,
		PacketPKStatus,

		PacketOpenNPC = 0x30F,
		PacketNpcDialog,

		PacketRefreshGold = 0x312,
		PacketBuyItem,
		PacketSellItem,
		PacketTradeRequest,
		PacketTradeResponse,
		PacketTrade,
		PacketPersonalShopClose,
		PacketPersonalShopData,
		PacketPersonalShopOpen,
		PacketPersonalShopBuy,
		PacketAcquireSkill,
		PacketUseItem,
		PacketItemBar,

		PacketSkillUse = 0x320,
		PacketUpdateSizes,
		PacketPartyRequest,
		PacketPartyResponse,
		PacketPartyKick,
		PacketPartyDestroy,
		PacketPartyUpdate,

		PacketRemoveBuff = 0x329,

		PacketCraftItem = 0x32B,
		PacketDeleteItem,
		PacketItemAttribute,
		PacketReinforceResponse,
		PacketQuestCancel,
		PacketQuestRewardSelect,
		PacketQuestInfo,
		PacketJoinItems,
		PacketSplitItem,
		PacketDungeonSelect,

		PacketCollectTimer = 0x336,

		PacketPartyMemberPosition0 = 0x338,

		PacketCancelCollect = 0x33A,

		PacketUnknow33D = 0x33D, //envia ao entrar

		PacketCreateGuild = 0x341,
		PacketRaidInvite,
		PacketRaidResponse,
		PacketRaidRemoveParty,

		PacketCloseUsingNPC = 0x348,
		PacketCreateMob,
		PacketTeleportMage,
		PacketPartyGiveLeader,

		PacketBattlefieldResult = 0x353,
		PacketDungeonConfirm = 0x355,
		PacketPremiumGift,
		PacketUnknow357,
		PacketServerVersion = 0x358, //msg "Versão do servidor foi atualizada e o jogo precisa ser reiniciado" fecha o game depois de timer 5s
		PacketSendCaptcha = 0x35A,
		PacketSpawnMob = 0x35E,
		PacketUpgradeResult2 = 0x35F,

		PacketActiveTitle = 0x361,
		PacketFishingStart,
		PacketFishingResult,

		PacketFriendRequest = 0x372,

		PacketUpgradeResult = 0x38C, //Resultado melhoramento?

		PacketDuelRequest = 0x395,
		PacketDuelResponse,

		PacketAHackStatusCheck = 0x39D,

		PacketUnknow3A2 = 0x3A2,

		PacketCharacterDelete = 0x603,

		PacketGuildChangeMaster = 0x619,

		PacketGuildGoldDeposit = 0x661,

		PacketFriendResponse = 0x673,


		PacketLoginRequest = 0x685,

		PacketMoveItem = 0x70F,

		PacketFriendSocialPacket = 0x870,

		PacketCharacterList = 0x901,

		PacketPranToWorld = 0x907,

		PacketGuildAlly = 0x911,

		PacketGuildsCastleSiege = 0x91A,

		PacketSendToWorld = 0x925,

		PacketGuildPerfilMessage = 0x932,

		PacketNationLeader = 0x936,

		PacketUnknow94C = 0x94C,

		PacketDevirReliqueTimes = 0x953,

		PacketReliqueUpgradeData = 0x957,

		PacketGuildInfo = 0x965,

		PacketAllyInfo = 0x967,

		PacketGuildLogin = 0x969,
		PacketGuildLogout,
		PacketPranDevotion,

		PacketFriendLogin = 0x96F,

		PacketFriendLogout = 0x971,

		PacketFriendInfo = 0x975,

		PacketGuildInvite = 0x97C,
		PacketGuildMembers = 0x97F,

		PacketSendMessage = 0x984,

		PacketDevirInfoRequest = 0xB52,


		PacketGuildGold = 0xD18,

		PacketGuildMemberLevel = 0xD1E,

		PacketUnknowD41 = 0xD41, //envia numeros 

		PacketAltarChest = 0xD56,

		PacketGuildChest = 0xD58,

		PacketNationTaxes = 0xE3A,

		PacketNumericToken = 0xF02,

		PacketChannelChange = 0xF05,
		PacketChannelChangeInfo,

		PacketEnterIngame = 0xF0B,

		PacketRefreshItem = 0xF0E,

		PacketGuildChestPermission = 0xF10,

		PacketGuildAllyRequest = 0xF12,

		PacketGuildMemberRank = 0xF1D,

		PacketGuildNotices = 0xF20,
		PacketGuildSites = 0xF21,

		PacketNationTreasure = 0xF34,

		PacketNationAllyRequest = 0xF3B,
		PacketNationAllyCancelRequest = 0xF3E,

		PacketDisplayGuildChest = 0xF55,
		PacketChangeChestGold = 0xF59,

		PacketChatItemLink = 0xF6F,

		PacketFriendChat = 0xF26,
		PacketFriendWindowOpen,

		PacketFriendWindowClose = 0xF30,

		PacketFriendDelete = 0xF74,

		PacketChat = 0xF86,

		PacketMobMovement = 0x30BF,

		PacketCharacterDeletePermanent = 0x3E01,
		PacketPramRename,
		PacketCharacterCreate = 0x3E04,

		PacketAuctionRegister = 0x3F0B,

		PacketAuctionBuyOffer = 0x3F0C,
		PacketAuctionItemsSearch,

		PacketAuctionCancelOffer = 0x3F10,
		PacketAuctionPlayerItems,

	};

#pragma region "Header and Basics"

	typedef struct PacketHeader
	{
		WORD Size;
		BYTE Key;
		BYTE ChkSum;
		WORD Index;
		PacketCode Code;
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


	typedef struct PacketClientIndex {
		PacketHeader Header;
		DWORD Index;
		DWORD Effect;
	}*PPacket_117;

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

	typedef struct PacketSpawnMob {
		PacketHeader Header;
		WORD Equipaments[8];
		Position CurrentPosition;
		DWORD Rotation;
		DWORD CurHP, CurMP;
		DWORD MaxHP, MaxMP;
		WORD unk_1;
		WORD Level;
		WORD Null_1;
		WORD IsService;
		BYTE Effects[4];
		SpawnType spawnType;
		CharacterSize Sizes;
		WORD MobType;
		BYTE unk_2;
		WORD MobName;
		WORD unk_3[3];
	}*PPacket_35E;

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
#pragma region "Buffs Packets"

	typedef struct PacketRefreshAllBuffs {
		PacketHeader header;
		WORD buffsIDs[60];
		DWORD buffsDuration[60];
	}*PPacket_10B;

	typedef struct PacketRefreshBuffs {
		PacketHeader header;
		WORD buffsIDs[40];
		DWORD buffsDuration[40];
	}*PPacket_16E;

	typedef struct PacketUpdateBuff {
		PacketHeader header;
		DWORD buffID;
		DWORD buffDuration;
		DWORD UNK_0;
	}*PPacket_16F;

	typedef struct PacketRemoveBuff {
		PacketHeader header;
		DWORD buffID;
	}*PPacket_329;

#pragma endregion
#pragma region "Item Bar Packets"

	typedef struct PacketItemBar {
		PacketHeader header;
		DWORD destSlot;
		DWORD destType;
		DWORD index;
	}*PPacket_31E;


#pragma endregion
#pragma region "NPC Packets"

	typedef struct PacketOpenNPC {
		PacketHeader Header;
		DWORD Index;
		DWORD Type1;
		DWORD Type2;
	}*PPacket_30F;

	typedef struct PacketShowNPCOption {
		PacketHeader Header;
		DWORD Option;
		DWORD Null;
		char Text[64];
		DWORD Show;
	}*PPacket_112;

	typedef struct PacketNPCShop {
		PacketHeader header;
		WORD index;
		WORD shopType;
		WORD items[40];
	}*PPacket_106;

#pragma endregion
#pragma region "Titles"

	typedef struct PacketUpdateActiveTitle {
		PacketHeader Header;
		DWORD TitleIndex;
		DWORD TitleLevel;
	}*PPacket_361;

#pragma endregion
#pragma region "Premium Items/Cash Packets"

	typedef struct PacketUpdateCashInventory {
		PacketHeader Header;
		ItemCash Items[24];
	}*PPacket_138;

	typedef struct PacketSendPremiumGift {
		PacketHeader Header;
		WORD GiftTarget;
		WORD Slot;
		char Nick[16];
	}*PPacket_356;


#pragma endregion
#pragma region "Chat Packets"

	enum class ChatType : WORD {
		Normal,
		Whisper,
		Party,
		Guild,
		Shout
	};

	typedef struct PacketChat {
		PacketHeader Header;
		ChatType ChatType;
		BYTE NULL_1[6];
		DWORD Color;
		char Nickname[16];
		char ChatText[128];
	}*PPacket_F86;

#pragma endregion
#pragma region "Attack e Skill"

	enum class DamageType : BYTE {
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
		None
	};

	typedef struct PacketAttackTarget {
		PacketHeader Header;
		WORD Index;
		BYTE null_0[14];
		WORD Animation;
		WORD Skill;
		Position AttackerPosition;
		Position TargetPosition;
	}*PPacket_302;

	typedef struct PacketSkillUse {
		PacketHeader header;
		DWORD skill;
		DWORD index;
		Position position;
	}*PPacket_320;

	typedef struct PacketDisplayDamage {
		PacketHeader header;
		DWORD skill;
		Position attackerPosition;
		DWORD null_1;
		WORD attackerId;
		BYTE null_2;
		BYTE animation;
		BYTE null_3[12];
		DWORD attackerHp;
		BYTE null_4[8];
		WORD targetId;
		DamageType damageType;
		BYTE mobAnimation;
		uint64_t damage;
		DWORD null_5;
		DWORD targetHp;
		Position deathPosition;
	}*PPacket_102;

#pragma endregion


}

#pragma pack(pop)

#endif
