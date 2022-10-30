#include <iostream>
#include <boost/json.hpp>

#include <Lua/lua.hpp>
#pragma comment(lib, "lua54.lib")

#include "DataLoader.h"
#include "Logger.h"

#include "InitialCharacters.h"
#include "ItemList.h"
#include "SetItem.h"
#include "SkillData.h"
#include "ReinforceData.h"
#include "TitleList.h"
#include "MapData.h"

bool DataLoader::Startup(ServerInstance* server) {
	this->_server = server;
    this->_currentDir = this->GetCurrentDir();

	if (!this->LoadItemList()) {
		return false;
	}

	if (!this->LoadInitialCharacters()) {
		return false;
	}

	if (!this->LoadSetItem()) {
		return false; 
	}

	if (!this->LoadSkillData()) {
		return false;
	}

	if (!this->LoadReinforce2()) {
		return false;
	}

	if (!this->LoadReinforce3()) {
		return false;
	}

	if (!this->LoadItemConsumeFunctions()) {
		return false; 
	}

	if (!this->LoadItemConsumeEffects()) {
		return false; 
	}

	if (!this->LoadTitles()) {
		return false;
	}

	if (!this->LoadTeleportPositions()) {
		return false;
	}

	if (!this->LoadMapData()) {
		return false;
	}

    return true;
}

std::string DataLoader::GetCurrentDir() {
    char buffer[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

bool DataLoader::ForEachFileInFolder(std::string folderPath, bool (*procedure)(std::string workingrPath, std::string fileName), int* found) {
	HANDLE hFind;
	WIN32_FIND_DATAA data;

	hFind = FindFirstFileA(folderPath.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (procedure(this->_currentDir, data.cFileName)) {
				*found += 1;
			}
			else {
				return false;
			}
		} while (FindNextFileA(hFind, &data));
		FindClose(hFind);
	}

	return true;
}

#pragma region "Characters"

bool ProcessInitialCharacterFile(std::string workingrPath, std::string fileName) {
	std::string fullFileName = Logger::Format(R"(%s\InitialCharacters\%s)", workingrPath.c_str(), fileName.c_str());

	try {
		std::ifstream ifs(fullFileName);
		std::string	input(std::istreambuf_iterator<char>(ifs), {});
		ifs.close();

		json::value characterData = json::parse(input);

		if (characterData.is_null()) {
			return false;
		}

		WORD characterClassInfo = json::value_to<int>(characterData.at("class_info"));

		InitialCharacters* initialCharacters = InitialCharacters::GetInstance();
		return initialCharacters->Add(characterClassInfo, characterData);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[ProcessInitialCharacterFile] error: %s", e.what());
		return false;
	}

	return true;
}

bool DataLoader::LoadInitialCharacters() {
	std::string folderPathFilter = Logger::Format(R"(%s\InitialCharacters\*.json)", this->_currentDir.c_str());

	int loadedFiles = 0;

	if (!this->ForEachFileInFolder(folderPathFilter, &ProcessInitialCharacterFile, &loadedFiles)) {
		Logger::Write(Error, "error loading InitialCharacters data!");
		return false;
	}

	Logger::Write(Status, "loaded %d InitialCharacters data files!", loadedFiles);

	return true;
}

#pragma endregion
#pragma region "Items"

bool DataLoader::LoadItemList() {
	std::string fileName = Logger::Format(R"(%s\Data\ItemList.bin)", this->_currentDir.c_str());

	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		Logger::Write("error loading ItemList, file not found!", LOG_TYPE::Error);
		return false;
	}

	int itemsLoaded = 0;

	try {
		int bufferSize = sizeof(ItemFromList);
		char* buffer = new char[bufferSize];

		size_t bytesRead{ 0 };

		ItemList* itemList = ItemList::GetInstance();

		do {
			bytesRead = ifs.read(buffer, bufferSize).gcount();

			if (bytesRead == 0) {
				continue;
			}

			ItemFromList item = ItemFromList();
			memcpy(&item, &buffer[0], bufferSize);

			itemList->Add(item);

			if (item.Name[0] == 0) {
				continue;
			}

			itemsLoaded++;
		} while (bytesRead == bufferSize);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DataLoader::LoadItemList] error: %s", e.what());
		return false;
	}

	Logger::Write(Status, "loaded %d items from ItemList.bin", itemsLoaded);

	return true;
}

bool DataLoader::LoadSetItem() {
	std::string fileName = Logger::Format(R"(%s\Data\SetItem.bin)", this->_currentDir.c_str());

	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		Logger::Write("error loading SetItem, file not found!", LOG_TYPE::Error);
		return false;
	}

	int itemsLoaded = 0;

	try {
		int bufferSize = sizeof(SetItemFromList);
		char* buffer = new char[bufferSize];

		size_t bytesRead{ 0 };

		SetItem* setItem = SetItem::GetInstance();

		do {
			bytesRead = ifs.read(buffer, bufferSize).gcount();

			if (bytesRead == 0) {
				continue;
			}

			SetItemFromList item = SetItemFromList();
			memcpy(&item, &buffer[0], bufferSize);

			setItem->Add(item);

			if (item.Name[0] == 0) {
				continue;
			}

			itemsLoaded++;
		} while (bytesRead == bufferSize);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DataLoader::LoadSetItem] error: %s", e.what());
		return false;
	}

	Logger::Write(Status, "loaded %d items from SetItem.bin", itemsLoaded);

	return true;
}


bool DataLoader::LoadReinforce2() {
	std::string fileName = Logger::Format(R"(%s\Data\Reinforce2.bin)", this->_currentDir.c_str());

	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		Logger::Write("error loading Reinforce2, file not found!", LOG_TYPE::Error);
		return false;
	}

	int itemsLoaded = 0;

	try {
		int bufferSize = sizeof(ItemAttributeReinforce);
		char* buffer = new char[bufferSize];

		size_t bytesRead{ 0 };

		ReinforceData* reinforceData = ReinforceData::GetInstance();

		do {
			bytesRead = ifs.read(buffer, bufferSize).gcount();

			if (bytesRead == 0) {
				continue;
			}

			ItemAttributeReinforce item = ItemAttributeReinforce();
			memcpy(&item, &buffer[0], bufferSize);

			reinforceData->AddReinforce2(item);

			itemsLoaded++;
		} while (bytesRead == bufferSize);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DataLoader::LoadReinforce2] error: %s", e.what());
		return false;
	}

	Logger::Write(Status, "loaded %d items from Reinforce2.bin", itemsLoaded);

	return true;
}

bool DataLoader::LoadReinforce3() {
	std::string fileName = Logger::Format(R"(%s\Data\Reinforce3.bin)", this->_currentDir.c_str());

	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		Logger::Write("error loading SetItem, file not found!", LOG_TYPE::Error);
		return false;
	}

	int itemsLoaded = 0;

	try {
		int bufferSize = sizeof(ArmorAttributeReinforce);
		char* buffer = new char[bufferSize];

		size_t bytesRead{ 0 };

		ReinforceData* reinforceData = ReinforceData::GetInstance();

		do {
			bytesRead = ifs.read(buffer, bufferSize).gcount();

			if (bytesRead == 0) {
				continue;
			}

			ArmorAttributeReinforce item = ArmorAttributeReinforce();
			memcpy(&item, &buffer[0], bufferSize);

			reinforceData->AddReinforce3(item);

			itemsLoaded++;
		} while (bytesRead == bufferSize);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DataLoader::LoadReinforce3] error: %s", e.what());
		return false;
	}

	Logger::Write(Status, "loaded %d items from Reinforce3.bin", itemsLoaded);

	return true;
}


#pragma endregion
#pragma region "Items Use"

bool ProcessItemConsumeFunction(std::string workingrPath, std::string fileName) {
	std::string fullFileName = Logger::Format(R"(%s\Scripts\Itens\ConsumeFunctions\%s)", workingrPath.c_str(), fileName.c_str());

	lua_State* L = luaL_newstate();

	int r = luaL_dofile(L, fullFileName.c_str());

	if (r == LUA_OK) {
		lua_getglobal(L, "CONSUME_FUNCTION_ID");

		uint32_t index = (uint32_t)lua_tointeger(L, -1);

		if (index <= 0) {
			return false;
		}
	
		ItemList::GetInstance()->AddUseItem(index, fullFileName, 0);
	}
	else {
		std::string errormessage = lua_tostring(L, -1);
		
		Logger::Write(Error, "[ProcessItemConsumeFunction] error: %s", errormessage.c_str());
	}

	lua_close(L);

	return true;
}

bool DataLoader::LoadItemConsumeFunctions() {
	std::string folderPathFilter = Logger::Format(R"(%s\Scripts\Itens\ConsumeFunctions\*.lua)", this->_currentDir.c_str());

	int loadedFiles = 0;

	if (!this->ForEachFileInFolder(folderPathFilter, &ProcessItemConsumeFunction, &loadedFiles)) {
		Logger::Write(Error, "error loading ItemConsumeFunction data!");
		return false;
	}

	Logger::Write(Status, "loaded %d ItemConsumeFunction data files!", loadedFiles);

	return true;
}

bool ProcessItemConsumeEffect(std::string workingrPath, std::string fileName) {
	std::string fullFileName = Logger::Format(R"(%s\Scripts\Itens\ConsumeEffects\%s)", workingrPath.c_str(), fileName.c_str());

	lua_State* L = luaL_newstate();

	int r = luaL_dofile(L, fullFileName.c_str());

	if (r == LUA_OK) {
		lua_getglobal(L, "CONSUME_EFFECT_ID");

		uint32_t index = (uint32_t)lua_tointeger(L, -1);

		if (index <= 0) {
			return false;
		}

		ItemList::GetInstance()->AddUseItem(index, fullFileName, 1);
	}
	else {
		std::string errormessage = lua_tostring(L, -1);

		Logger::Write(Error, "[ProcessItemConsumeEffect] error: %s", errormessage.c_str());
	}

	lua_close(L);

	return true;
}


bool DataLoader::LoadItemConsumeEffects() {
	std::string folderPathFilter = Logger::Format(R"(%s\Scripts\Itens\ConsumeEffects\*.lua)", this->_currentDir.c_str());

	int loadedFiles = 0;

	if (!this->ForEachFileInFolder(folderPathFilter, &ProcessItemConsumeEffect, &loadedFiles)) {
		Logger::Write(Error, "error loading ItemConsumeEffects data!");
		return false;
	}

	Logger::Write(Status, "loaded %d ItemConsumeEffects data files!", loadedFiles);

	return true;
}

#pragma endregion
#pragma region "Skills"

bool DataLoader::LoadSkillData() {
	std::string fileName = Logger::Format(R"(%s\Data\SkillData.bin)", this->_currentDir.c_str());

	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		Logger::Write("error loading SkillData, file not found!", LOG_TYPE::Error);
		return false;
	}

	int itemsLoaded = 0;

	try {
		int bufferSize = sizeof(SkillFromList);
		char* buffer = new char[bufferSize];

		size_t bytesRead{ 0 };

		SkillData* itemList = SkillData::GetInstance();

		do {
			bytesRead = ifs.read(buffer, bufferSize).gcount();

			if (bytesRead == 0) {
				continue;
			}

			SkillFromList item = SkillFromList();
			memcpy(&item, &buffer[0], bufferSize);

			itemList->Add(item);

			if (item.Name[0] == 0) {
				continue;
			}

			itemsLoaded++;
		} while (bytesRead == bufferSize);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DataLoader::LoadSkillData] error: %s", e.what());
		return false;
	}

	Logger::Write(Status, "loaded %d items from SkillData.bin", itemsLoaded);

	return true;
}


#pragma endregion
#pragma region "Titles"

bool DataLoader::LoadTitles() {
	std::string fileName = Logger::Format(R"(%s\Data\Title.bin)", this->_currentDir.c_str());

	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		Logger::Write("error loading Titles, file not found!", LOG_TYPE::Error);
		return false;
	}

	int itemsLoaded = 0;

	try {
		int bufferSize = sizeof(TitleFromList);
		char* buffer = new char[bufferSize];

		size_t bytesRead{ 0 };

		TitleList* titleList = TitleList::GetInstance();

		do {
			bytesRead = ifs.read(buffer, bufferSize).gcount();

			if (bytesRead == 0) {
				continue;
			}

			TitleFromList item = TitleFromList();
			memcpy(&item, &buffer[0], bufferSize);

			titleList->Add(item);

			if (item.Level[0].TitleIndex == 0) {
				continue;
			}

			itemsLoaded++;
		} while (bytesRead == bufferSize);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DataLoader::LoadTitles] error: %s", e.what());
		return false;
	}

	Logger::Write(Status, "loaded %d items from Title.bin", itemsLoaded);

	return true;
}

#pragma endregion
#pragma region "Teleport Positions"

bool DataLoader::LoadTeleportPositions() {
	std::string fileName = Logger::Format(R"(%s\Data\ScrollPos.bin)", this->_currentDir.c_str());

	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		Logger::Write("error loading teleport list, file not found!", LOG_TYPE::Error);
		return false;
	}

	int itemsLoaded = 0;

	try {
		int bufferSize = sizeof(ScrollTeleportPosition);
		char* buffer = new char[bufferSize];

		size_t bytesRead{ 0 };

		MapData* mapData = MapData::GetInstance();

		do {
			bytesRead = ifs.read(buffer, bufferSize).gcount();

			if (bytesRead == 0) {
				continue;
			}

			ScrollTeleportPosition item = ScrollTeleportPosition();
			memcpy(&item, &buffer[0], bufferSize);

			mapData->AddTeleport(item);

			if (item.MapName[0] == 0) {
				continue;
			}

			itemsLoaded++;
		} while (bytesRead == bufferSize);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DataLoader::LoadTeleportPositions] error: %s", e.what());
		return false;
	}

	Logger::Write(Status, "loaded %d items from ScrollPos.bin", itemsLoaded);

	return true;
}

#pragma endregion
#pragma region "Map Data"

bool DataLoader::LoadMapData() {
	std::string fileName = Logger::Format(R"(%s\Data\Map.bin)", this->_currentDir.c_str());

	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		Logger::Write("error loading map bounds list, file not found!", LOG_TYPE::Error);
		return false;
	}

	int itemsLoaded = 0;

	try {
		int bufferSize = sizeof(MapBounds);
		char* buffer = new char[bufferSize];

		size_t bytesRead{ 0 };

		MapData* mapData = MapData::GetInstance();

		do {
			bytesRead = ifs.read(buffer, bufferSize).gcount();

			if (bytesRead == 0) {
				continue;
			}

			MapBounds item = MapBounds();
			memcpy(&item, &buffer[0], bufferSize);

			mapData->AddBounds(item);

			if (item.EndPositionX == 0) {
				continue;
			}

			itemsLoaded++;
		} while (bytesRead == bufferSize);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[DataLoader::LoadMapData] error: %s", e.what());
		return false;
	}

	Logger::Write(Status, "loaded %d items from Map.bin", itemsLoaded);

	return true;
}

#pragma endregion
