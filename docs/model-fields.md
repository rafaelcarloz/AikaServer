# AikaWeb model fields

[Documentation index](README.md) · [Database guide](database.md)

Extracted from the C# model declarations. Types are C# types; `?` marks nullable fields. API output uses default camel-case names. A key marks the EF primary key; keyless active-session records are documented separately.

## AccountModel

SQL object: `Accounts`. Source: [model file](../AikaWeb/Models/AccountModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| AccountID | long | Primary key |
| AccountStatus | int |  |
| AccountUsername | string? |  |
| AccountPassword | string? |  |

## BuffModel

SQL object: `CharacterBuffs`. Source: [model file](../AikaWeb/Models/BuffModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| CharacterBuffID | long | Primary key |
| BuffStatus | int |  |
| CharacterID | long |  |
| SkillListID | int |  |
| BuffDuration | DateTime? |  |

## CharacterModel

SQL object: `Characters`. Source: [model file](../AikaWeb/Models/CharacterModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| CharacterID | long | Primary key |
| CharacterStatus | byte |  |
| AccountID | long |  |
| CharacterName | string? |  |
| CharacterSlot | byte |  |
| NumericToken | string? |  |
| NumericTokenError | byte |  |
| CharacterLastRoration | int |  |
| LoggedHour | int |  |
| PlayerKillActive | byte |  |
| CharacterClass | int |  |
| CharacterFirstLogin | int |  |
| CharacterLevel | int |  |
| AddinitionalStrength | int? |  |
| AddinitionalAgility | int? |  |
| AddinitionalIntelligence | int? |  |
| AddinitionalConstituion | int? |  |
| AddinitionalLuck | int? |  |
| CurrentHelthPoints | int? |  |
| CurrentManaPoints | int? |  |
| CurrentHonorPoints | int |  |
| AllHonorPoints | int |  |
| CurrentKillPoints | int |  |
| AllKillPoints | int |  |
| InfamiaStatus | int |  |
| CurrentSkillPoints | int |  |
| CurrentExperience | long |  |
| CharacterGuildID | int? |  |
| CharacterGold | long |  |
| CharacterPositionX | int |  |
| CharacterPositionY | int |  |
| CharacterActiveTitleID | int? |  |
| CharacterActiveAction | int? |  |
| ScheduledDeleteOn | DateTime? |  |
| CreatedOn | DateTime |  |
| CreatedByAddress | string? |  |
| LoggedOn | DateTime? |  |
| LoggedByAddress | string? |  |
| SavedPositionX | int |  |
| SavedPositionY | int |  |

## CharacterQuestModel

SQL object: `CharacterQuests`. Source: [model file](../AikaWeb/Models/CharacterQuestModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| CharacterQuestID | long | Primary key |
| IsActive | int |  |
| QuestStatus | string? |  |
| CharacterID | long |  |
| QuestID | int |  |
| GoalCompleted_1 | int |  |
| GoalCompleted_2 | int |  |
| GoalCompleted_3 | int |  |
| GoalCompleted_4 | int |  |
| GoalCompleted_5 | int |  |
| AcceptedOn | DateTime? |  |
| CompletedOn | DateTime? |  |
| UpdatedOn | DateTime? |  |

## HotBarModel

SQL object: `CharacterHotBar`. Source: [model file](../AikaWeb/Models/HotBarModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| CharacterHotBarID | long | Primary key |
| ItemStatus | byte |  |
| CharacterID | long |  |
| ItemType | byte |  |
| ItemSlot | byte |  |
| ItemID | int |  |

## ItemModel

SQL object: `CharacterItems`. Source: [model file](../AikaWeb/Models/ItemModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| CharacterItemID | long | Primary key |
| ItemStatus | byte |  |
| CharacterID | long |  |
| InventoryType | byte |  |
| InventorySlot | byte |  |
| ItemListID | int |  |
| ItemAppearanceID | int |  |
| ItemIdentication | int |  |
| ItemEffect1_ID | int |  |
| ItemEffect1_Value | int |  |
| ItemEffect2_ID | int |  |
| ItemEffect2_Value | int |  |
| ItemEffect3_ID | int |  |
| ItemEffect3_Value | int |  |
| ItemDurabilityMin | byte |  |
| ItemDurabilityMax | byte |  |
| ItemAmount | int |  |
| ItemExpireTime | int |  |

## NationModel

SQL object: `Nations`. Source: [model file](../AikaWeb/Models/NationModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| NationID | int | Primary key |
| NationName | string? |  |
| NationRank | int |  |
| NationStatus | int |  |
| MarshalGuildID | long? |  |
| TaxCitizens | int |  |
| TaxVisitors | int |  |
| TaxSettlement | int |  |
| NationAlliedID | int? |  |
| NationAlliedDate | DateTime? |  |
| NationGold | long |  |
| CreatedOn | DateTime |  |
| UpdatedOn | DateTime? |  |

## ServerModel

SQL object: `Servers`. Source: [model file](../AikaWeb/Models/ServerModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| ServerID | int | Primary key |
| ServerType | int |  |
| ServerStatus | int |  |
| NationID | int |  |
| ServerAddress | string? |  |
| ServerPort | int |  |
| CreatedOn | DateTime |  |
| UpdatedOn | DateTime? |  |

## SessionModel

SQL object: `Sessions`. Source: [model file](../AikaWeb/Models/SessionModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| SessionID | long | Primary key |
| SessionStatus | int |  |
| AccountID | long |  |
| CreatedByAddress | string? |  |
| CreatedOn | DateTime |  |
| UpdatedOn | DateTime |  |

## SessionActiveModel

SQL object: `vwSessionsActive`. Source: [model file](../AikaWeb/Models/SessionModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| SessionID | long |  |
| SessionStatus | int |  |
| AccountID | long |  |
| AccountNation | int |  |
| AccountUsername | string? |  |
| CreatedOn | DateTime |  |
| CreatedByAddress | string? |  |
| UpdatedOn | DateTime |  |
| SessionHash | string? |  |

## SkillModel

SQL object: `CharacterSkills`. Source: [model file](../AikaWeb/Models/SkillModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| CharacterSkillID | long | Primary key |
| SkillStatus | byte |  |
| CharacterID | long |  |
| SkillType | byte |  |
| SkillSlot | byte |  |
| SkillListID | int |  |

## TitleModel

SQL object: `CharacterTitles`. Source: [model file](../AikaWeb/Models/TitleModel.cs).

| Property | C# type | Key |
| --- | --- | --- |
| CharacterTitleID | long | Primary key |
| TitleStatus | int |  |
| CharacterID | long |  |
| TitleID | int |  |
| TitleLevel | int |  |
| TitleProgress | int |  |
| CreatedOn | DateTime? |  |
| UpdatedOn | DateTime? |  |
