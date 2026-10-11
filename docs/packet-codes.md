# Packet code inventory

[Documentation index](README.md) · [Protocol guide](protocol.md)

Extracted from `GameServer/Packets.h`. Numeric values include implicit enum increments. Inbound dispatch identifies cases in the current PacketControl switches, not complete feature support. Codes without an inbound case may be outbound, reserved, or unfinished.

| Code | Enum name | Inbound dispatch |
| --- | --- | --- |
| 0x81 | CheckTokenPacket | MainServer |
| 0x82 | ResponseLoginPacket | No inbound case |
| 0x101 | PacketRemoveMob | No inbound case |
| 0x102 | PacketDisplayDamage | No inbound case |
| 0x103 | PacketCharacterLife | No inbound case |
| 0x106 | PacketNPCShop | No inbound case |
| 0x107 | PacketCharacterSkillsLevel | No inbound case |
| 0x108 | PacketCharacterLevel | No inbound case |
| 0x109 | PacketUpdateAttributes | No inbound case |
| 0x10A | PacketUpdateStatus | No inbound case |
| 0x10B | PacketUpdateBuffsAll | No inbound case |
| 0x10C | PacketUnknow10C | No inbound case |
| 0x10D | PacketEntityEffect | No inbound case |
| 0x10E | PacketNPCTalk | No inbound case |
| 0x10F | PacketCloseNPC | No inbound case |
| 0x110 | PacketUnknow110 | No inbound case |
| 0x111 | PacketAnimationTalk | No inbound case |
| 0x112 | PacketShowNPCOption | No inbound case |
| 0x115 | PacketSoundPlay | No inbound case |
| 0x116 | PacketPranLevel | No inbound case |
| 0x117 | PacketClientIndex | No inbound case |
| 0x119 | PacketDungeonDialog | No inbound case |
| 0x11B | PacketGiveExpGold | No inbound case |
| 0x11C | PacketUnknow11C | No inbound case |
| 0x11D | PacketPartyMemberPosition | No inbound case |
| 0x11F | PacketUnknow11F | No inbound case |
| 0x121 | PacketCastleStatus | No inbound case |
| 0x122 | PacketUnknow122 | No inbound case |
| 0x123 | PacketUnknow123 | No inbound case |
| 0x125 | PacketGuildAddPlayer | No inbound case |
| 0x127 | PacketUnknow127 | No inbound case |
| 0x128 | PacketUnknow128 | No inbound case |
| 0x12A | PacketUpdateHonorPvP | No inbound case |
| 0x12B | PacketUpdateDurability | No inbound case |
| 0x12C | PacketSendSkills | No inbound case |
| 0x130 | PacketPlayerGuild | No inbound case |
| 0x131 | PacketUnknow131 | No inbound case |
| 0x136 | PacketReliques | No inbound case |
| 0x137 | PacketStorage | No inbound case |
| 0x138 | PacketPremiumStorage | No inbound case |
| 0x13C | PacketBattlefildLobby | No inbound case |
| 0x142 | PacketBattleTimer | No inbound case |
| 0x145 | PacketDungeonLobby | No inbound case |
| 0x14F | PacketAccountStatus | No inbound case |
| 0x150 | PacketUpdateClientId | No inbound case |
| 0x151 | PacketSendOnlineTime | No inbound case |
| 0x152 | PacketUnknow152 | No inbound case |
| 0x153 | PacketGiveExperience | No inbound case |
| 0x154 | PacketUnknow154 | No inbound case |
| 0x160 | PacketUnknow160 | No inbound case |
| 0x16B | PacketUnknow16B | No inbound case |
| 0x16D | PacketUnknow16D | No inbound case |
| 0x16E | PacketUpdateBuffs | No inbound case |
| 0x16F | PacketUpdateBuff | No inbound case |
| 0x170 | PacketUnknow170 | No inbound case |
| 0x17D | PacketTitleList | No inbound case |
| 0x17E | PacketUnknow186 | No inbound case |
| 0x19E | PacketCharacterViewRequest | No inbound case |
| 0x1A0 | PacketDuelResult | No inbound case |
| 0x1A2 | PacketDuelTimer | No inbound case |
| 0x207 | PacketRaidGiveLeader | No inbound case |
| 0x213 | PacketGetStatusPoint | No inbound case |
| 0x21B | PacketBuffItem | No inbound case |
| 0x224 | PacketUnsealItem | GameServer |
| 0x227 | PacketUnknow227 | No inbound case |
| 0x22C | PacketCharacterViewResponse | No inbound case |
| 0x22D | PacketChatItem | No inbound case |
| 0x233 | PacketUnknow233 | No inbound case |
| 0x301 | PacketMovemment | GameServer |
| 0x302 | PacketAttackTarget | GameServer |
| 0x303 | PacketRevive | GameServer |
| 0x304 | PacketCharacterAction | GameServer |
| 0x305 | PacketCharacterRotation | GameServer |
| 0x306 | PacketRequestMobInfoformation | GameServer |
| 0x307 | PacketPKStatus | No inbound case |
| 0x30F | PacketOpenNPC | GameServer |
| 0x310 | PacketNpcDialog | No inbound case |
| 0x312 | PacketRefreshGold | No inbound case |
| 0x313 | PacketBuyItem | GameServer |
| 0x314 | PacketSellItem | GameServer |
| 0x315 | PacketTradeRequest | No inbound case |
| 0x316 | PacketTradeResponse | No inbound case |
| 0x317 | PacketTrade | No inbound case |
| 0x318 | PacketPersonalShopClose | No inbound case |
| 0x319 | PacketPersonalShopData | No inbound case |
| 0x31A | PacketPersonalShopOpen | No inbound case |
| 0x31B | PacketPersonalShopBuy | No inbound case |
| 0x31C | PacketAcquireSkill | GameServer |
| 0x31D | PacketUseItem | GameServer |
| 0x31E | PacketItemBar | GameServer |
| 0x320 | PacketSkillUse | GameServer |
| 0x321 | PacketUpdateSizes | No inbound case |
| 0x322 | PacketPartyRequest | No inbound case |
| 0x323 | PacketPartyResponse | No inbound case |
| 0x324 | PacketPartyKick | No inbound case |
| 0x325 | PacketPartyDestroy | No inbound case |
| 0x326 | PacketPartyUpdate | No inbound case |
| 0x329 | PacketRemoveBuff | No inbound case |
| 0x32A | PacketResetSkills | GameServer |
| 0x32B | PacketCraftItem | No inbound case |
| 0x32C | PacketDeleteItem | GameServer |
| 0x32D | PacketItemAttribute | No inbound case |
| 0x32E | PacketReinforceResponse | No inbound case |
| 0x32F | PacketQuestRemove | GameServer |
| 0x330 | PacketQuestRewardSelect | GameServer |
| 0x331 | PacketQuestInfo | No inbound case |
| 0x332 | PacketJoinItems | GameServer |
| 0x333 | PacketSplitItem | GameServer |
| 0x334 | PacketDungeonSelect | No inbound case |
| 0x336 | PacketCollectTimer | No inbound case |
| 0x338 | PacketPartyMemberPosition0 | No inbound case |
| 0x33A | PacketCancelCollect | No inbound case |
| 0x33D | PacketUnknow33D | No inbound case |
| 0x341 | PacketCreateGuild | No inbound case |
| 0x342 | PacketRaidInvite | No inbound case |
| 0x343 | PacketRaidResponse | No inbound case |
| 0x344 | PacketRaidRemoveParty | No inbound case |
| 0x348 | PacketCloseUsingNPC | GameServer |
| 0x349 | PacketCreateMob | No inbound case |
| 0x34A | PacketTeleportMage | No inbound case |
| 0x34B | PacketPartyGiveLeader | No inbound case |
| 0x353 | PacketBattlefieldResult | No inbound case |
| 0x355 | PacketDungeonConfirm | No inbound case |
| 0x356 | PacketPremiumGift | No inbound case |
| 0x357 | PacketUnknow357 | No inbound case |
| 0x358 | PacketServerVersion | No inbound case |
| 0x35A | PacketSendCaptcha | No inbound case |
| 0x35E | PacketSpawnMob | No inbound case |
| 0x35F | PacketUpgradeResult2 | No inbound case |
| 0x361 | PacketActiveTitle | GameServer |
| 0x362 | PacketFishingStart | No inbound case |
| 0x363 | PacketFishingResult | No inbound case |
| 0x372 | PacketFriendRequest | No inbound case |
| 0x38C | PacketUpgradeResult | No inbound case |
| 0x395 | PacketDuelRequest | No inbound case |
| 0x396 | PacketDuelResponse | No inbound case |
| 0x39D | PacketAHackStatusCheck | GameServer |
| 0x3A2 | PacketUnknow3A2 | No inbound case |
| 0x603 | PacketCharacterDelete | GameServer |
| 0x619 | PacketGuildChangeMaster | No inbound case |
| 0x661 | PacketGuildGoldDeposit | No inbound case |
| 0x673 | PacketFriendResponse | No inbound case |
| 0x685 | PacketLoginRequest | GameServer |
| 0x70F | PacketMoveItem | GameServer |
| 0x870 | PacketFriendSocialPacket | No inbound case |
| 0x901 | PacketCharacterList | No inbound case |
| 0x907 | PacketPranToWorld | No inbound case |
| 0x911 | PacketGuildAlly | No inbound case |
| 0x91A | PacketGuildsCastleSiege | No inbound case |
| 0x925 | PacketSendToWorld | No inbound case |
| 0x932 | PacketGuildPerfilMessage | No inbound case |
| 0x936 | PacketNationLeader | No inbound case |
| 0x94C | PacketUnknow94C | No inbound case |
| 0x953 | PacketDevirReliqueTimes | No inbound case |
| 0x957 | PacketReliqueUpgradeData | No inbound case |
| 0x965 | PacketGuildInfo | No inbound case |
| 0x967 | PacketAllyInfo | No inbound case |
| 0x969 | PacketGuildLogin | No inbound case |
| 0x96A | PacketGuildLogout | No inbound case |
| 0x96B | PacketPranDevotion | No inbound case |
| 0x96F | PacketFriendLogin | No inbound case |
| 0x971 | PacketFriendLogout | No inbound case |
| 0x975 | PacketFriendInfo | No inbound case |
| 0x97C | PacketGuildInvite | No inbound case |
| 0x97F | PacketGuildMembers | No inbound case |
| 0x984 | PacketSendMessage | No inbound case |
| 0xB52 | PacketDevirInfoRequest | No inbound case |
| 0xD18 | PacketGuildGold | No inbound case |
| 0xD1E | PacketGuildMemberLevel | No inbound case |
| 0xD41 | PacketUnknowD41 | No inbound case |
| 0xD56 | PacketAltarChest | No inbound case |
| 0xD58 | PacketGuildChest | No inbound case |
| 0xE3A | PacketNationTaxes | No inbound case |
| 0xF02 | PacketNumericToken | GameServer |
| 0xF05 | PacketChannelChange | No inbound case |
| 0xF06 | PacketChannelChangeInfo | No inbound case |
| 0xF0B | PacketEnterIngame | GameServer |
| 0xF0E | PacketRefreshItem | No inbound case |
| 0xF10 | PacketGuildChestPermission | No inbound case |
| 0xF12 | PacketGuildAllyRequest | No inbound case |
| 0xF1D | PacketGuildMemberRank | No inbound case |
| 0xF20 | PacketGuildNotices | No inbound case |
| 0xF21 | PacketGuildSites | No inbound case |
| 0xF34 | PacketNationTreasure | No inbound case |
| 0xF3B | PacketNationAllyRequest | No inbound case |
| 0xF3E | PacketNationAllyCancelRequest | No inbound case |
| 0xF55 | PacketDisplayGuildChest | No inbound case |
| 0xF59 | PacketChangeChestGold | No inbound case |
| 0xF6F | PacketChatItemLink | No inbound case |
| 0xF26 | PacketFriendChat | No inbound case |
| 0xF27 | PacketFriendWindowOpen | No inbound case |
| 0xF30 | PacketFriendWindowClose | No inbound case |
| 0xF74 | PacketFriendDelete | No inbound case |
| 0xF86 | PacketChat | GameServer |
| 0x30BF | PacketMobMovement | No inbound case |
| 0x3E01 | PacketCharacterDeletePermanent | GameServer |
| 0x3E02 | PacketPramRename | No inbound case |
| 0x3E04 | PacketCharacterCreate | GameServer |
| 0x3F0B | PacketAuctionRegister | No inbound case |
| 0x3F0C | PacketAuctionBuyOffer | No inbound case |
| 0x3F0D | PacketAuctionItemsSearch | No inbound case |
| 0x3F10 | PacketAuctionCancelOffer | No inbound case |
| 0x3F11 | PacketAuctionPlayerItems | No inbound case |
