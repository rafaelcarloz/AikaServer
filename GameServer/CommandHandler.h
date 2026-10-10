#pragma once

#ifndef _COMMAND_HANDLER_H_
#define _COMMAND_HANDLER_H_

#include "Player.h"

class CommandHandler {
private:
	//Quests
	static bool AddQuest(PPlayer player, std::vector<std::string> params);
	static bool DeliverQuest(PPlayer player, std::vector<std::string> params);

	//Effects
	static bool AddEffect(PPlayer player, std::vector<std::string> params);
public:
	static bool CommandControl(PPlayer player, char* commandString);
};

#endif