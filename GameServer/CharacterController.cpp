#include <stdio.h>
#include <boost/json.hpp>
#include <functional>

#include "CharacterController.h"
#include "Player.h"
#include "InitialCharacters.h"
#include "WebHandler.h"
#include "Packets.h"

#include "SkillData.h"
#include "ServerInstance.h"
#include "DateFunctions.h"
#include "TitleList.h"

using namespace packets;

namespace json = boost::json;

#pragma region "Init & GetData"

void CharacterController::Initialize(long characterId, PPlayer player) {
	this->_characterId = characterId;
	this->_player = player;

	this->_player->visibleController = new VisibleController();
	this->_player->buffsController = new BuffsController();

	if (!this->GetCharacterData()) {
		this->_player->isActive = false;
		return;
	}

	if (!this->GetSkillsData()) {
		this->_player->isActive = false;
		return;
	}

	if (!this->GetHotBarData()) {
		this->_player->isActive = false;
		return;
	}

	if (!this->_player->inventoryController.Initialize(&this->_player->character, _player)) {
		this->_player->isActive = false;
		return;
	}

	this->_player->SendSignalData(0xCCCC, 1);

	this->_player->SendSignalData(0x186, 1);
	this->_player->SendSignalData(0x186, 1);
	this->_player->SendSignalData(0x186, 1);

	this->SendToWorld();
}

bool CharacterController::GetCharacterData() {
	WebHandler webHandler;

	ZeroMemory(&this->_player->character, sizeof(Character));

	InitialCharacters* initialCharacters = InitialCharacters::GetInstance();

	try {
		json::value character = json::value();

		if (!webHandler.GetCharacter(this->_characterId, &character)) {
			Logger::Write(Error, "[CharacterController::GetCharacterData] request error");
			return false;
		}

		this->_player->character.CharacterId = json::value_to<int>(character.at("characterID"));

		this->_player->character.ClassInfo = (CharacterClass)json::value_to<int>(character.at("characterClass"));

		this->_player->baseCharacter = initialCharacters->Get((WORD)this->_player->character.ClassInfo);

		strcpy_s(this->_player->character.Name, json::value_to<std::string>(character.at("characterName")).c_str());

		this->_player->position.X = static_cast<float>(json::value_to<int>(character.at("characterPositionX")));
		this->_player->position.Y = static_cast<float>(json::value_to<int>(character.at("characterPositionY")));

		this->savedPosition.X = static_cast<float>(json::value_to<int>(character.at("savedPositionX")));
		this->savedPosition.Y = static_cast<float>(json::value_to<int>(character.at("savedPositionY")));

		if (this->savedPosition.X == 0 && this->savedPosition.Y == 0) {
			this->savedPosition.X = 3450;
			this->savedPosition.Y = 690;
		}
	
		this->_player->character.Gold = json::value_to<int>(character.at("characterGold"));
		this->_player->character.GuildIndex = character.at("characterGuildID").is_null() ? 0u : (WORD)json::value_to<int>(character.at("characterGuildID"));

		this->_player->character.Level = (WORD)json::value_to<int>(character.at("characterLevel"));
		this->_player->character.ExpPoints = json::value_to<int>(character.at("currentExperience"));

		this->_player->character.Status.SkillPoints = json::value_to<int>(character.at("currentSkillPoints"));

		this->_player->character.Status.Life.CurHP = character.at("currentHelthPoints").is_null() ? 0u : json::value_to<int>(character.at("currentHelthPoints"));
		this->_player->character.Status.Life.CurMP = character.at("currentManaPoints").is_null() ? 0u : json::value_to<int>(character.at("currentManaPoints"));

		this->_player->character.Status.HonorPoints = json::value_to<int>(character.at("currentHonorPoints"));
		this->_player->character.Status.KillPoints = json::value_to<int>(character.at("currentKillPoints")); 

		this->_firstLogin = (bool)json::value_to<int>(character.at("characterFirstLogin"));

		this->activePlayerKill = (bool)json::value_to<int>(character.at("playerKillActive"));

		this->_player->character.Status.Attributes.Strength = character.at("addinitionalStrength").is_null() ? 0u : json::value_to<int>(character.at("addinitionalStrength"));
		this->_player->character.Status.Attributes.Dexterity = character.at("addinitionalAgility").is_null() ? 0u : json::value_to<int>(character.at("addinitionalAgility"));
		this->_player->character.Status.Attributes.Intelligence = character.at("addinitionalIntelligence").is_null() ? 0u : json::value_to<int>(character.at("addinitionalIntelligence"));
		this->_player->character.Status.Attributes.Constitution = character.at("addinitionalConstituion").is_null() ? 0u : json::value_to<int>(character.at("addinitionalConstituion"));
		this->_player->character.Status.Attributes.Spirit = character.at("addinitionalLuck").is_null() ? 0u : json::value_to<int>(character.at("addinitionalLuck"));

		this->activeTitle.Index = character.at("characterActiveTitleID").is_null() ? 0u : json::value_to<int>(character.at("characterActiveTitleID"));

		this->_player->rotation = character.at("characterLastRoration").is_null() ? 0u : json::value_to<int>(character.at("characterLastRoration"));

		this->_LoggedOn = DateFunctions::StringToDate(character.at("loggedOn").is_null() ? "" : json::value_to<std::string>(character.at("loggedOn")));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[CharacterController::GetCharacterData] error: %s", e.what());
		return false;
	}

	return true;
}

bool CharacterController::GetSkillsData() {
	WebHandler webHandler;

	try {
		json::value skills = json::value();

		if (!webHandler.GetCharacterSkills(this->_characterId, &skills)) {
			Logger::Write(Error, "[CharacterController::GetSkillsData] request error");
			return false;
		}

		for (auto& skill : skills.as_array()) {
			int slot = json::value_to<int>(skill.at("skillSlot"));
			uint64_t databaseID = json::value_to<int>(skill.at("characterSkillID"));

			this->_player->character.SkillList[slot].databaseID = databaseID;
			this->_player->character.SkillList[slot].skillID = json::value_to<int>(skill.at("skillListID"));
		}

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[CharacterController::GetSkillsData] error: %s", e.what());
	}

	return false;
}

bool CharacterController::GetHotBarData() {
	WebHandler webHandler;

	try {
		json::value items = json::value();

		if (!webHandler.GetCharacterHotBar(this->_characterId, &items)) {
			Logger::Write(Error, "[CharacterController::GetHotBarData] request error");
			return false;
		}

		for (auto& item : items.as_array()) {
			int slot = json::value_to<int>(item.at("itemSlot"));
			uint64_t databaseID = json::value_to<int>(item.at("characterHotBarID"));

			this->_player->character.ItemBar[slot].databaseID = databaseID;

			this->_player->character.ItemBar[slot].itemType = json::value_to<int>(item.at("itemType"));
			this->_player->character.ItemBar[slot].itemID = json::value_to<int>(item.at("itemID"));
		}

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[CharacterController::GetHotBarData] error: %s", e.what());
	}

	return false;
}

#pragma endregion
#pragma region "Save Data" 

boost::json::value CharacterController::CharacterToJSON() {
	json::value characterData;

	try {
		json::object& _character = characterData.emplace_object();

		_character.emplace("CharacterID", this->_characterId);

		_character.emplace("CharacterFirstLogin", 0);

		_character.emplace("CharacterClass", (int)this->_player->character.ClassInfo);

		_character.emplace("CharacterLevel", this->_player->character.Level);
		_character.emplace("CurrentExperience", this->_player->character.ExpPoints);

		_character.emplace("CharacterGold", this->_player->character.Gold);

		_character.emplace("CharacterPositionX", (int)this->_player->position.X);
		_character.emplace("CharacterPositionY", (int)this->_player->position.Y);

		_character.emplace("SavedPositionX", (int)this->savedPosition.X);
		_character.emplace("SavedPositionY", (int)this->savedPosition.Y);

			
		_character.emplace("CharacterGuildID", this->_player->character.GuildIndex);

		_character.emplace("CurrentSkillPoints", this->_player->character.Status.SkillPoints);

		_character.emplace("CurrentHelthPoints", this->_player->character.Status.Life.CurHP);
		_character.emplace("CurrentManaPoints", this->_player->character.Status.Life.CurMP);

		_character.emplace("CurrentHonorPoints", this->_player->character.Status.HonorPoints);
		_character.emplace("CurrentKillPoints", this->_player->character.Status.KillPoints);

		_character.emplace("PlayerKillActive", (int)this->activePlayerKill);


		_character.emplace("AddinitionalStrength", this->_player->character.Status.Attributes.Strength);
		_character.emplace("AddinitionalAgility", this->_player->character.Status.Attributes.Dexterity);
		_character.emplace("AddinitionalIntelligence", this->_player->character.Status.Attributes.Intelligence);
		_character.emplace("AddinitionalConstituion", this->_player->character.Status.Attributes.Constitution);
		_character.emplace("AddinitionalLuck", this->_player->character.Status.Attributes.Spirit);

		_character.emplace("CharacterActiveTitleID", this->activeTitle.Index);

		return characterData;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[CharacterController::CharacterToJSON] error: %s", e.what());
	}

	return json::value();
}

bool CharacterController::SaveCharacter() {
	if (!this->SaveCharacterData()) {
		Logger::Write(Error, "[CharacterController::SaveCharacter] error while saving data!");
		return false;
	}

	if (!this->SaveInventoryData()) {
		Logger::Write(Error, "[CharacterController::SaveInventoryData] error while saving data!");
		return false;
	}

	if (!this->SaveSkillsData()) {
		Logger::Write(Error, "[CharacterController::SaveSkillsData] error while saving data!");
		return false;
	}

	if (!this->SaveHotBarData()) {
		Logger::Write(Error, "[CharacterController::SaveHotBarData] error while saving data!");
		return false;
	}

	if (!this->SaveBuffsData()) {
		Logger::Write(Error, "[CharacterController::SaveBuffsData] error while saving data!");
		return false;
	}

	return true;
}


bool CharacterController::SaveCharacterData() {
	WebHandler webHandler;

	json::value characterData = this->CharacterToJSON();
	json::value response;

	if (!webHandler.PostSaveCharacter(this->_characterId, characterData, &response)) {
		return false;
	}

	return true;
}

bool CharacterController::SaveInventoryData() {
	WebHandler webHandler;

	json::value inventoryData = this->_player->inventoryController.InventoryToJSON();
	json::value response;

	if (!webHandler.PostCharacterItems(this->_characterId, inventoryData, &response)) {
		return false;
	}

	return true;
}

bool CharacterController::SaveSkillsData() {
	json::value skillsData = json::value();

	json::array& items = skillsData.emplace_array();

	try {

		for (int i = 0; i < 60; i++) {
			json::value _holder;
			json::object& item = _holder.emplace_object();

			item.emplace("CharacterSkillID", this->_player->character.SkillList[i].databaseID);

			item.emplace("SkillStatus", 1);
			item.emplace("CharacterID", this->_characterId);
			item.emplace("SkillType", (i < 6)? 0 : 1);
			item.emplace("SkillSlot", i);
			item.emplace("SkillListID", this->_player->character.SkillList[i].skillID);

			items.emplace_back(item);
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[CharacterController::SaveSkillsData] error: %s", e.what());
		return false;
	}


	WebHandler webHandler;

	json::value response;

	if (!webHandler.PostCharacterSkills(this->_characterId, skillsData, &response)) {
		return false;
	}

	return true;
}

bool CharacterController::SaveHotBarData() {
	json::value hotBarData = json::value();

	json::array& items = hotBarData.emplace_array();

	try {

		for (int i = 0; i < 24; i++) {
			json::value _holder;
			json::object& item = _holder.emplace_object();

			item.emplace("CharacterHotBarID", this->_player->character.ItemBar[i].databaseID);

			item.emplace("ItemStatus", 1);
			item.emplace("CharacterID", this->_characterId);
			item.emplace("ItemType", this->_player->character.ItemBar[i].itemType);
			item.emplace("ItemSlot", i);
			item.emplace("ItemID", this->_player->character.ItemBar[i].itemID);

			items.emplace_back(item);
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[CharacterController::SaveHotBarData] error: %s", e.what());
		return false;
	}


	WebHandler webHandler;

	json::value response;

	if (!webHandler.PostCharacterHotBar(this->_characterId, hotBarData, &response)) {
		return false;
	}

	return true;
}

bool CharacterController::SaveBuffsData() {
	WebHandler webHandler;

	json::value buffsData = this->_player->buffsController->ToJSON();
	json::value response;

	if (!webHandler.PostCharacterBuffs(this->_characterId, buffsData, &response)) {
		return false;
	}

	return true;
}

#pragma endregion

bool CharacterController::SendToWorld() {
	this->_player->visibleController->Initialize(this->_player);
	this->_player->statusController.Initialize(this->_player->index, &this->_player->character, this->_player->baseCharacter);
	this->_player->buffsController->Initialize(this->_player->index, _player);

	if (this->_firstLogin) {
		this->_player->statusController.RestaureLifePoints();
	}

	PacketSendToWorld Packet;

	ZeroMemory(&Packet, sizeof(Packet));

	Packet.header.Size = sizeof(Packet);
	Packet.header.Index = 0x7535;
	Packet.header.Code = 0x925;

	Packet.accountId = this->_player->account->accountId;

	Packet.clientId = this->_player->index;
	Packet.characterClass = this->_player->character.ClassInfo;
	Packet.nation = (Citizenship)this->_player->account->accountNation;

	Packet.FirstLogin = this->_firstLogin;

	strcpy_s(Packet.name, this->_player->character.Name);

	Packet.CreationTime = (DWORD)this->_CreatedOn;
	Packet.CurrentScore = this->_player->statusController.volatileStatus;
	Packet.EndDayTime = (DWORD)time(0);

	memcpy(&Packet.Equipaments[0], &this->_player->character.Equipaments[0], sizeof(Packet.Equipaments));
	memcpy(&Packet.Inventory[0], &this->_player->character.Inventory[0], sizeof(Packet.Inventory));

	SkillData* skillData = SkillData::GetInstance();

#pragma region "Set Item Bar"

	for (int i = 0; i < 24; i++) {
		Packet.ItemBar[i] = (this->_player->character.ItemBar[i].itemID << 4) + this->_player->character.ItemBar[i].itemType;
	}

#pragma endregion
#pragma region "Set Skill List"

	int j = 0;
	int k = 0;

	DWORD Skill = 0;

	for (int i = 0; i < 60; i++) {
		if (this->_player->character.SkillList[i].skillID == 0) {
			j++;
			continue;
		}

		k = skillData->GetSendToWorldSkill(this->_player->character.SkillList[i].skillID, Skill);

		memcpy(&Packet.SkillList[j], &Skill, 2 * k);

		j += k;
	}

#pragma endregion
#pragma region "Set Titles List"

	Packet.ActiveTitle = this->activeTitle.Index;

	for (BYTE i = 0; i < 96; i++) {
		PTITLE CurrentTitle = &this->_player->character.Titles[i].title;

		if (CurrentTitle->Index == 0) { continue; }

		BYTE TitleCategory = (BYTE)trunc(CurrentTitle->Index / 8);
		BYTE TitleSlot = CurrentTitle->Index % 8;

		Packet.TitleCategoryLevel[TitleCategory] += CurrentTitle->GetLevelValue(TitleSlot);

		switch (TitleList::Get(CurrentTitle->Index).Level[CurrentTitle->Level - 1].TitleType)
		{

		case 8:
			Packet.TitleProgressType8[TitleList::Get(CurrentTitle->Index).Level[CurrentTitle->Level - 1].TitleIndex - 1] =
				CurrentTitle->Progress;
			break;
		case 9:
			Packet.TitleProgressType9[1] = CurrentTitle->Progress;
			break;
		case 4:
			Packet.TitleProgressType4 = CurrentTitle->Progress;
			break;
		case 10:
			Packet.TitleProgressType10 = CurrentTitle->Progress;
			break;
		case 7:
			Packet.TitleProgressType7 = CurrentTitle->Progress;
			break;
		case 11:
			Packet.TitleProgressType11 = CurrentTitle->Progress;
			break;
		case 12:
			Packet.TitleProgressType12 = CurrentTitle->Progress;
			break;
		case 13:
			Packet.TitleProgressType13 = CurrentTitle->Progress;
			break;
		case 15:
			Packet.TitleProgressType15 = CurrentTitle->Progress;
			break;
		case 16:
			Packet.TitleProgressType16[TitleList::Get(CurrentTitle->Index).Level[CurrentTitle->Level - 1].TitleIndex - 1] =
				CurrentTitle->Progress;
			break;
		case 23:
			Packet.TitleProgressType23 = CurrentTitle->Progress;
			break;

		default:
			break;
		}
	}

#pragma endregion

	Packet.ExpPoints = this->_player->character.ExpPoints;
	Packet.Gold = this->_player->character.Gold;
	Packet.GuildIndex = this->_player->character.GuildIndex;
	Packet.Level = this->_player->character.Level;
	Packet.LoginTime = (DWORD)this->_LoggedOn;

	memcpy(&Packet.NumericToken[0], &this->_numericToken[0], 4);

	this->_player->SendPacket(&Packet, Packet.header.Size);

	return true;
}

bool CharacterController::EnterIngame() {
	this->_player->SendCreateMob(SpawnNormal, this->_player->index);

	this->_player->playingStatus = Playing;

	this->_player->IsIstantiated = true;

	this->_LoggedOn = time(0);

	this->_player->timeLastBasicAttack = time(0);
	this->_player->timeLastReceivedAttack = time(0);

	this->_player->timeLastBuffTick = time(0);
	this->_player->timeLastRegenerationTick = time(0);

	return true;
}

void CharacterController::RespawnCharacter(bool sendSelf, SpawnType spawnType) {
	auto procedureCall = std::bind(&GameEntity::SendCreateMob, this->_player, std::placeholders::_1, std::placeholders::_1);

	if (sendSelf) {
		this->_player->SendCreateMob(spawnType, this->_player->index);
	}

	this->_player->visibleController->ForEach(procedureCall, spawnType);
}

void CharacterController::UnspawnCharacter(bool sendSelf, DeleteType deleteType) {
	if (this->_player == nullptr) {
		return;
	}

	if (!this->_player->IsIstantiated) {
		return;
	}

	auto procedureCall = std::bind(&GameEntity::SendRemoveMob, this->_player, std::placeholders::_1, std::placeholders::_1);

	if (sendSelf) {
		this->_player->SendRemoveMob(this->_player->index, deleteType);
	}

	this->_player->visibleController->ForEach(procedureCall, deleteType);
}


void CharacterController::Teleport(Position position) {
	PacketMovemment packet;

	ZeroMemory(&packet, sizeof(PacketMovemment));

	packet.Header.Size = sizeof(PacketMovemment);
	packet.Header.Index = this->_player->index;
	packet.Header.Code = 0x301;

	packet.Destination = position;
	packet.MoveType = MovemmentTeleport;

	this->_player->SendPacket(&packet, packet.Header.Size);

	this->UnspawnCharacter(false, DeleteUnspawn);

	this->_player->position = position;
}

void CharacterController::Teleport(int positionX, int positionY) {
	Position position = Position();

	position.X = static_cast<float>(positionX);
	position.Y = static_cast<float>(positionY);

	this->Teleport(position);
}

void CharacterController::SendToSavedPosition() {
	this->Teleport(this->savedPosition);
}


void CharacterController::SendItemBarSlot(BYTE slot, BYTE destType, WORD index) {
	PacketItemBar packet;

	ZeroMemory(&packet, sizeof(PacketItemBar));

	packet.header.Size = sizeof(PacketItemBar);
	packet.header.Index = 0x7535;
	packet.header.Code = 0x31E;

	packet.destSlot = slot;

	packet.destType = (index == 0) ? this->_player->character.ItemBar[slot].itemType : destType;
	packet.index = (index == 0) ? this->_player->character.ItemBar[slot].itemID : index;

	this->_player->SendPacket(&packet, packet.header.Size);
}
