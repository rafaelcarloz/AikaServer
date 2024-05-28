IF OBJECT_ID(N'[__EFMigrationsHistory]') IS NULL
BEGIN
    CREATE TABLE [__EFMigrationsHistory] (
        [MigrationId] nvarchar(150) NOT NULL,
        [ProductVersion] nvarchar(32) NOT NULL,
        CONSTRAINT [PK___EFMigrationsHistory] PRIMARY KEY ([MigrationId])
    );
END;
GO

BEGIN TRANSACTION;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [Accounts] (
        [AccountID] bigint NOT NULL IDENTITY,
        [AccountStatus] int NOT NULL,
        [AccountUsername] nvarchar(max) NULL,
        [AccountPassword] nvarchar(max) NULL,
        CONSTRAINT [PK_Accounts] PRIMARY KEY ([AccountID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [CharacterBuffs] (
        [CharacterBuffID] bigint NOT NULL IDENTITY,
        [BuffStatus] int NOT NULL,
        [CharacterID] bigint NOT NULL,
        [SkillListID] int NOT NULL,
        [BuffDuration] datetime2 NULL,
        CONSTRAINT [PK_CharacterBuffs] PRIMARY KEY ([CharacterBuffID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [CharacterHotBar] (
        [CharacterHotBarID] bigint NOT NULL IDENTITY,
        [ItemStatus] tinyint NOT NULL,
        [CharacterID] bigint NOT NULL,
        [ItemType] tinyint NOT NULL,
        [ItemSlot] tinyint NOT NULL,
        [ItemID] int NOT NULL,
        CONSTRAINT [PK_CharacterHotBar] PRIMARY KEY ([CharacterHotBarID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [CharacterItems] (
        [CharacterItemID] bigint NOT NULL IDENTITY,
        [ItemStatus] tinyint NOT NULL,
        [CharacterID] bigint NOT NULL,
        [InventoryType] tinyint NOT NULL,
        [InventorySlot] tinyint NOT NULL,
        [ItemListID] int NOT NULL,
        [ItemAppearanceID] int NOT NULL,
        [ItemIdentication] int NOT NULL,
        [ItemEffect1_ID] int NOT NULL,
        [ItemEffect1_Value] int NOT NULL,
        [ItemEffect2_ID] int NOT NULL,
        [ItemEffect2_Value] int NOT NULL,
        [ItemEffect3_ID] int NOT NULL,
        [ItemEffect3_Value] int NOT NULL,
        [ItemDurabilityMin] tinyint NOT NULL,
        [ItemDurabilityMax] tinyint NOT NULL,
        [ItemAmount] int NOT NULL,
        [ItemExpireTime] int NOT NULL,
        CONSTRAINT [PK_CharacterItems] PRIMARY KEY ([CharacterItemID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [Characters] (
        [CharacterID] bigint NOT NULL IDENTITY,
        [CharacterStatus] tinyint NOT NULL,
        [AccountID] bigint NOT NULL,
        [CharacterName] nvarchar(max) NULL,
        [CharacterSlot] tinyint NOT NULL,
        [NumericToken] nvarchar(max) NULL,
        [NumericTokenError] tinyint NOT NULL,
        [CharacterLastRoration] int NOT NULL,
        [LoggedHour] int NOT NULL,
        [PlayerKillActive] tinyint NOT NULL,
        [CharacterClass] int NOT NULL,
        [CharacterFirstLogin] int NOT NULL,
        [CharacterLevel] int NOT NULL,
        [AddinitionalStrength] int NULL,
        [AddinitionalAgility] int NULL,
        [AddinitionalIntelligence] int NULL,
        [AddinitionalConstituion] int NULL,
        [AddinitionalLuck] int NULL,
        [CurrentHelthPoints] int NULL,
        [CurrentManaPoints] int NULL,
        [CurrentHonorPoints] int NOT NULL,
        [AllHonorPoints] int NOT NULL,
        [CurrentKillPoints] int NOT NULL,
        [AllKillPoints] int NOT NULL,
        [InfamiaStatus] int NOT NULL,
        [CurrentSkillPoints] int NOT NULL,
        [CurrentExperience] bigint NOT NULL,
        [CharacterGuildID] int NULL,
        [CharacterGold] bigint NOT NULL,
        [CharacterPositionX] int NOT NULL,
        [CharacterPositionY] int NOT NULL,
        [CharacterActiveTitleID] int NULL,
        [CharacterActiveAction] int NULL,
        [ScheduledDeleteOn] datetime2 NULL,
        [CreatedOn] datetime2 NOT NULL,
        [CreatedByAddress] nvarchar(max) NULL,
        [LoggedOn] datetime2 NULL,
        [LoggedByAddress] nvarchar(max) NULL,
        [SavedPositionX] int NOT NULL,
        [SavedPositionY] int NOT NULL,
        CONSTRAINT [PK_Characters] PRIMARY KEY ([CharacterID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [CharacterSkills] (
        [CharacterSkillID] bigint NOT NULL IDENTITY,
        [SkillStatus] tinyint NOT NULL,
        [CharacterID] bigint NOT NULL,
        [SkillType] tinyint NOT NULL,
        [SkillSlot] tinyint NOT NULL,
        [SkillListID] int NOT NULL,
        CONSTRAINT [PK_CharacterSkills] PRIMARY KEY ([CharacterSkillID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [CharacterTitles] (
        [CharacterTitleID] bigint NOT NULL IDENTITY,
        [TitleStatus] int NOT NULL,
        [CharacterID] bigint NOT NULL,
        [TitleID] int NOT NULL,
        [TitleLevel] int NOT NULL,
        [TitleProgress] int NOT NULL,
        [CreatedOn] datetime2 NULL,
        [UpdatedOn] datetime2 NULL,
        CONSTRAINT [PK_CharacterTitles] PRIMARY KEY ([CharacterTitleID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [Nations] (
        [NationID] int NOT NULL IDENTITY,
        [NationName] nvarchar(max) NULL,
        [NationRank] int NOT NULL,
        [NationStatus] int NOT NULL,
        [MarshalGuildID] bigint NULL,
        [TaxCitizens] int NOT NULL,
        [TaxVisitors] int NOT NULL,
        [TaxSettlement] int NOT NULL,
        [NationAlliedID] int NULL,
        [NationAlliedDate] datetime2 NULL,
        [NationGold] bigint NOT NULL,
        [CreatedOn] datetime2 NOT NULL,
        [UpdatedOn] datetime2 NULL,
        CONSTRAINT [PK_Nations] PRIMARY KEY ([NationID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [Servers] (
        [ServerID] int NOT NULL IDENTITY,
        [ServerType] int NOT NULL,
        [ServerStatus] int NOT NULL,
        [NationID] int NOT NULL,
        [ServerAddress] nvarchar(max) NULL,
        [ServerPort] int NOT NULL,
        [CreatedOn] datetime2 NOT NULL,
        [UpdatedOn] datetime2 NULL,
        CONSTRAINT [PK_Servers] PRIMARY KEY ([ServerID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [Sessions] (
        [SessionID] bigint NOT NULL IDENTITY,
        [SessionStatus] int NOT NULL,
        [AccountID] bigint NOT NULL,
        [CreatedByAddress] nvarchar(max) NULL,
        [CreatedOn] datetime2 NOT NULL,
        [UpdatedOn] datetime2 NOT NULL,
        CONSTRAINT [PK_Sessions] PRIMARY KEY ([SessionID])
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    CREATE TABLE [vwSessionsActive] (
        [SessionID] bigint NOT NULL,
        [SessionStatus] int NOT NULL,
        [AccountID] bigint NOT NULL,
        [AccountNation] int NOT NULL,
        [AccountUsername] nvarchar(max) NULL,
        [CreatedOn] datetime2 NOT NULL,
        [CreatedByAddress] nvarchar(max) NULL,
        [UpdatedOn] datetime2 NOT NULL,
        [SessionHash] nvarchar(max) NULL
    );
END;
GO

IF NOT EXISTS(SELECT * FROM [__EFMigrationsHistory] WHERE [MigrationId] = N'20240512201745_mssql.onprem_migration_186')
BEGIN
    INSERT INTO [__EFMigrationsHistory] ([MigrationId], [ProductVersion])
    VALUES (N'20240512201745_mssql.onprem_migration_186', N'6.0.20');
END;
GO

COMMIT;
GO

