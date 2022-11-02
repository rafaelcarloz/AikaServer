#pragma once

#ifndef _DATA_LOADER_H_
#define _DATA_LOADER_H_

#include <stdio.h>
#include <string>

#include "ServerInstance.h"

class DataLoader {
private:
	std::string _currentDir;
	ServerInstance* _server;

	std::string GetCurrentDir();

	bool ForEachFileInFolder(std::string folderPath, bool (*procedure)(std::string workingrPath, std::string fileName), int* found);

	//Characters
	bool LoadInitialCharacters();

	//Items
	bool LoadItemList();
	bool LoadSetItem();
	bool LoadReinforce2();
	bool LoadReinforce3();

	bool LoadPremiumItem();

	//Items Use
	bool LoadItemConsumeFunctions();
	bool LoadItemConsumeEffects();

	//Skills
	bool LoadSkillData();

	//Titles
	bool LoadTitles();

	//Teleport Positions
	bool LoadTeleportPositions();

	//MapData
	bool LoadMapData();

	//NPCs
	bool LoadNPCs();

	//Mobs
	bool LoadMobPos();
	bool LoadMobs();
public:
	bool Startup(ServerInstance* server);
};

#endif
