using System;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace AikaWeb.Migrations
{
    public partial class mssqlonprem_migration_186 : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Accounts",
                columns: table => new
                {
                    AccountID = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    AccountStatus = table.Column<int>(type: "int", nullable: false),
                    AccountUsername = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    AccountPassword = table.Column<string>(type: "nvarchar(max)", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Accounts", x => x.AccountID);
                });

            migrationBuilder.CreateTable(
                name: "CharacterBuffs",
                columns: table => new
                {
                    CharacterBuffID = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    BuffStatus = table.Column<int>(type: "int", nullable: false),
                    CharacterID = table.Column<long>(type: "bigint", nullable: false),
                    SkillListID = table.Column<int>(type: "int", nullable: false),
                    BuffDuration = table.Column<DateTime>(type: "datetime2", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_CharacterBuffs", x => x.CharacterBuffID);
                });

            migrationBuilder.CreateTable(
                name: "CharacterHotBar",
                columns: table => new
                {
                    CharacterHotBarID = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    ItemStatus = table.Column<byte>(type: "tinyint", nullable: false),
                    CharacterID = table.Column<long>(type: "bigint", nullable: false),
                    ItemType = table.Column<byte>(type: "tinyint", nullable: false),
                    ItemSlot = table.Column<byte>(type: "tinyint", nullable: false),
                    ItemID = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_CharacterHotBar", x => x.CharacterHotBarID);
                });

            migrationBuilder.CreateTable(
                name: "CharacterItems",
                columns: table => new
                {
                    CharacterItemID = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    ItemStatus = table.Column<byte>(type: "tinyint", nullable: false),
                    CharacterID = table.Column<long>(type: "bigint", nullable: false),
                    InventoryType = table.Column<byte>(type: "tinyint", nullable: false),
                    InventorySlot = table.Column<byte>(type: "tinyint", nullable: false),
                    ItemListID = table.Column<int>(type: "int", nullable: false),
                    ItemAppearanceID = table.Column<int>(type: "int", nullable: false),
                    ItemIdentication = table.Column<int>(type: "int", nullable: false),
                    ItemEffect1_ID = table.Column<int>(type: "int", nullable: false),
                    ItemEffect1_Value = table.Column<int>(type: "int", nullable: false),
                    ItemEffect2_ID = table.Column<int>(type: "int", nullable: false),
                    ItemEffect2_Value = table.Column<int>(type: "int", nullable: false),
                    ItemEffect3_ID = table.Column<int>(type: "int", nullable: false),
                    ItemEffect3_Value = table.Column<int>(type: "int", nullable: false),
                    ItemDurabilityMin = table.Column<byte>(type: "tinyint", nullable: false),
                    ItemDurabilityMax = table.Column<byte>(type: "tinyint", nullable: false),
                    ItemAmount = table.Column<int>(type: "int", nullable: false),
                    ItemExpireTime = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_CharacterItems", x => x.CharacterItemID);
                });

            migrationBuilder.CreateTable(
                name: "Characters",
                columns: table => new
                {
                    CharacterID = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    CharacterStatus = table.Column<byte>(type: "tinyint", nullable: false),
                    AccountID = table.Column<long>(type: "bigint", nullable: false),
                    CharacterName = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    CharacterSlot = table.Column<byte>(type: "tinyint", nullable: false),
                    NumericToken = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    NumericTokenError = table.Column<byte>(type: "tinyint", nullable: false),
                    CharacterLastRoration = table.Column<int>(type: "int", nullable: false),
                    LoggedHour = table.Column<int>(type: "int", nullable: false),
                    PlayerKillActive = table.Column<byte>(type: "tinyint", nullable: false),
                    CharacterClass = table.Column<int>(type: "int", nullable: false),
                    CharacterFirstLogin = table.Column<int>(type: "int", nullable: false),
                    CharacterLevel = table.Column<int>(type: "int", nullable: false),
                    AddinitionalStrength = table.Column<int>(type: "int", nullable: true),
                    AddinitionalAgility = table.Column<int>(type: "int", nullable: true),
                    AddinitionalIntelligence = table.Column<int>(type: "int", nullable: true),
                    AddinitionalConstituion = table.Column<int>(type: "int", nullable: true),
                    AddinitionalLuck = table.Column<int>(type: "int", nullable: true),
                    CurrentHelthPoints = table.Column<int>(type: "int", nullable: true),
                    CurrentManaPoints = table.Column<int>(type: "int", nullable: true),
                    CurrentHonorPoints = table.Column<int>(type: "int", nullable: false),
                    AllHonorPoints = table.Column<int>(type: "int", nullable: false),
                    CurrentKillPoints = table.Column<int>(type: "int", nullable: false),
                    AllKillPoints = table.Column<int>(type: "int", nullable: false),
                    InfamiaStatus = table.Column<int>(type: "int", nullable: false),
                    CurrentSkillPoints = table.Column<int>(type: "int", nullable: false),
                    CurrentExperience = table.Column<long>(type: "bigint", nullable: false),
                    CharacterGuildID = table.Column<int>(type: "int", nullable: true),
                    CharacterGold = table.Column<long>(type: "bigint", nullable: false),
                    CharacterPositionX = table.Column<int>(type: "int", nullable: false),
                    CharacterPositionY = table.Column<int>(type: "int", nullable: false),
                    CharacterActiveTitleID = table.Column<int>(type: "int", nullable: true),
                    CharacterActiveAction = table.Column<int>(type: "int", nullable: true),
                    ScheduledDeleteOn = table.Column<DateTime>(type: "datetime2", nullable: true),
                    CreatedOn = table.Column<DateTime>(type: "datetime2", nullable: false),
                    CreatedByAddress = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    LoggedOn = table.Column<DateTime>(type: "datetime2", nullable: true),
                    LoggedByAddress = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    SavedPositionX = table.Column<int>(type: "int", nullable: false),
                    SavedPositionY = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Characters", x => x.CharacterID);
                });

            migrationBuilder.CreateTable(
                name: "CharacterSkills",
                columns: table => new
                {
                    CharacterSkillID = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    SkillStatus = table.Column<byte>(type: "tinyint", nullable: false),
                    CharacterID = table.Column<long>(type: "bigint", nullable: false),
                    SkillType = table.Column<byte>(type: "tinyint", nullable: false),
                    SkillSlot = table.Column<byte>(type: "tinyint", nullable: false),
                    SkillListID = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_CharacterSkills", x => x.CharacterSkillID);
                });

            migrationBuilder.CreateTable(
                name: "CharacterTitles",
                columns: table => new
                {
                    CharacterTitleID = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    TitleStatus = table.Column<int>(type: "int", nullable: false),
                    CharacterID = table.Column<long>(type: "bigint", nullable: false),
                    TitleID = table.Column<int>(type: "int", nullable: false),
                    TitleLevel = table.Column<int>(type: "int", nullable: false),
                    TitleProgress = table.Column<int>(type: "int", nullable: false),
                    CreatedOn = table.Column<DateTime>(type: "datetime2", nullable: true),
                    UpdatedOn = table.Column<DateTime>(type: "datetime2", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_CharacterTitles", x => x.CharacterTitleID);
                });

            migrationBuilder.CreateTable(
                name: "Nations",
                columns: table => new
                {
                    NationID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    NationName = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    NationRank = table.Column<int>(type: "int", nullable: false),
                    NationStatus = table.Column<int>(type: "int", nullable: false),
                    MarshalGuildID = table.Column<long>(type: "bigint", nullable: true),
                    TaxCitizens = table.Column<int>(type: "int", nullable: false),
                    TaxVisitors = table.Column<int>(type: "int", nullable: false),
                    TaxSettlement = table.Column<int>(type: "int", nullable: false),
                    NationAlliedID = table.Column<int>(type: "int", nullable: true),
                    NationAlliedDate = table.Column<DateTime>(type: "datetime2", nullable: true),
                    NationGold = table.Column<long>(type: "bigint", nullable: false),
                    CreatedOn = table.Column<DateTime>(type: "datetime2", nullable: false),
                    UpdatedOn = table.Column<DateTime>(type: "datetime2", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Nations", x => x.NationID);
                });

            migrationBuilder.CreateTable(
                name: "Servers",
                columns: table => new
                {
                    ServerID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    ServerType = table.Column<int>(type: "int", nullable: false),
                    ServerStatus = table.Column<int>(type: "int", nullable: false),
                    NationID = table.Column<int>(type: "int", nullable: false),
                    ServerAddress = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    ServerPort = table.Column<int>(type: "int", nullable: false),
                    CreatedOn = table.Column<DateTime>(type: "datetime2", nullable: false),
                    UpdatedOn = table.Column<DateTime>(type: "datetime2", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Servers", x => x.ServerID);
                });

            migrationBuilder.CreateTable(
                name: "Sessions",
                columns: table => new
                {
                    SessionID = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    SessionStatus = table.Column<int>(type: "int", nullable: false),
                    AccountID = table.Column<long>(type: "bigint", nullable: false),
                    CreatedByAddress = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    CreatedOn = table.Column<DateTime>(type: "datetime2", nullable: false),
                    UpdatedOn = table.Column<DateTime>(type: "datetime2", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Sessions", x => x.SessionID);
                });

            migrationBuilder.CreateTable(
                name: "vwSessionsActive",
                columns: table => new
                {
                    SessionID = table.Column<long>(type: "bigint", nullable: false),
                    SessionStatus = table.Column<int>(type: "int", nullable: false),
                    AccountID = table.Column<long>(type: "bigint", nullable: false),
                    AccountNation = table.Column<int>(type: "int", nullable: false),
                    AccountUsername = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    CreatedOn = table.Column<DateTime>(type: "datetime2", nullable: false),
                    CreatedByAddress = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    UpdatedOn = table.Column<DateTime>(type: "datetime2", nullable: false),
                    SessionHash = table.Column<string>(type: "nvarchar(max)", nullable: true)
                },
                constraints: table =>
                {
                });
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Accounts");

            migrationBuilder.DropTable(
                name: "CharacterBuffs");

            migrationBuilder.DropTable(
                name: "CharacterHotBar");

            migrationBuilder.DropTable(
                name: "CharacterItems");

            migrationBuilder.DropTable(
                name: "Characters");

            migrationBuilder.DropTable(
                name: "CharacterSkills");

            migrationBuilder.DropTable(
                name: "CharacterTitles");

            migrationBuilder.DropTable(
                name: "Nations");

            migrationBuilder.DropTable(
                name: "Servers");

            migrationBuilder.DropTable(
                name: "Sessions");

            migrationBuilder.DropTable(
                name: "vwSessionsActive");
        }
    }
}
