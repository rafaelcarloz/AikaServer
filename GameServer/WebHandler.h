#pragma once

#ifndef _WEB_HANDLER_H_
#define _WEB_HANDLER_H_

#include <boost/json.hpp>
#include "WinHttpWrapper.h"

namespace json = boost::json;

class WebHandler{
private:
	const std::wstring _domain = L"localhost";
	const std::wstring _requestHeader = L"Content-Type: application/json";
	int _port = 8090;
	bool _https = true;

	bool ProcessResponse(WinHttpWrapper::HttpResponse & responseHttp, json::value* response);
public:
	bool GetSessionDataByToken(std::string username, std::string token, json::value* response);

	bool GetNation(int nationId, json::value* response);

	bool GetServers(json::value* response);

	//Characters
	bool GetCharactersList(int accountId, json::value* response);
	bool GetCharacter(int characterId, json::value* response);
	bool PostCharacter(const json::value characterData, json::value* response);
	bool PostSaveCharacter(int characterId, const json::value characterData, json::value* response);
	bool PostCharacterDeleteStatus(int characterId, const json::value characterData, json::value* response);
	bool PostCharacterNumericToken(int characterId, const json::value characterData, json::value* response);

	//Items
	bool GetCharacterItems(int characterId, int inventoryType, json::value* response);
	bool PostCharacterItems(int characterId, const json::value items, json::value* response);

	//Skills
	bool GetCharacterSkills(int characterId, json::value* response);
	bool PostCharacterSkills(int characterId, const json::value items, json::value* response);

	//HotBar
	bool GetCharacterHotBar(int characterId, json::value* response);
	bool PostCharacterHotBar(int characterId, const json::value items, json::value* response);

	//Buffs
	bool GetCharacterBuffs(int characterId, json::value* response);
	bool PostCharacterBuffs(int characterId, const json::value items, json::value* response);
};

#endif
