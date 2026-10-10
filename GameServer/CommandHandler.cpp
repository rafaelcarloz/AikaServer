#include <algorithm>
#include <cctype>
#include <string>

#include "CommandHandler.h"


std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); } // correct
    );
    return s;
}

bool CommandHandler::CommandControl(PPlayer player, char* commandString) {
    auto params = split(commandString, " ");

    std::string command = str_tolower(params[0]);

    params.erase(params.begin());

    if (command == "`addquest") {
        return CommandHandler::AddQuest(player, params);
    }

    if (command == "`deliverquest") {
        return CommandHandler::DeliverQuest(player, params);
    }

    if (command == "`addeffect") {
        return CommandHandler::AddEffect(player, params);
    }

	return true;
}

#pragma region "Quests"

bool CommandHandler::AddQuest(PPlayer player, std::vector<std::string> params) {
    if (params.size() == 1) {
        int questId = std::atoi(params[0].c_str());

        if (questId <= 0) {
            return true;
        }

        player->questsController->AcceptQuest(questId);

        return true;
    }

	return true;
}

bool CommandHandler::DeliverQuest(PPlayer player, std::vector<std::string> params) {
    if (params.size() == 1) {
        int questId = std::atoi(params[0].c_str());

        if (questId <= 0) {
            return true;
        }

        player->questsController->DeliverQuest(questId);

        return true;
    }

    return true;
}

#pragma endregion
#pragma region "Effects"

bool CommandHandler::AddEffect(PPlayer player, std::vector<std::string> params) {
    if (params.size() == 1) {
        int effectId = std::atoi(params[0].c_str());

        if (effectId <= 0) {
            return true;
        }

        player->characterController->SendEntityEffect(player->index, effectId);

        return true;
    }

    return true;
}

#pragma endregion
