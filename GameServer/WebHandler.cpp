#include "WebHandler.h"
#include "Logger.h"
#include <iostream>

using namespace WinHttpWrapper;
using namespace std;

bool WebHandler::ProcessResponse(HttpResponse& responseHttp, json::value* response) {
	try {
		switch (responseHttp.statusCode)
		{
		case 200: {
			*response = json::parse(responseHttp.text.c_str());
			break;
		}

		default:
			Logger::Write(Warnings, "[WebHandler::ProcessResponse] result[%d]: %ws", responseHttp.statusCode, responseHttp.error.c_str());
			return false;
			break;
		}

		responseHttp.Reset();

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Logger::Format("[WebHandler::ProcessResponse] %s", e.what()), Error);
	}

	return false;
}

bool WebHandler::GetSessionDataByToken(string username, string token, json::value* response) {
	try {
		HttpRequest reqquest(this->_domain, this->_port, this->_https);
		HttpResponse httpResponse;

		std::string data = Logger::Format(R"({"id":"%s", "pw": "%s"})", username.c_str(), token.c_str());

		reqquest.Post(L"/session/data",
			this->_requestHeader,
			data,
			httpResponse);

		return this->ProcessResponse(httpResponse, response);
	}
	catch (std::exception e) {
		Logger::Write(Logger::Format("[WebHandler::GetSessionDataByToken] %s", e.what()), Error);
	}
	
	return false;
}

bool WebHandler::GetServers(json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	reqquest.Get(L"/servers/all",
		this->_requestHeader,
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::GetNation(int nationId, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/nations/" + std::to_wstring(nationId);

	reqquest.Get(url,
		this->_requestHeader,
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}


#pragma region "Characters"

bool WebHandler::GetCharactersList(int accountId, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(accountId) + L"/all";

	reqquest.Get(url,
		this->_requestHeader,
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::GetCharacter(int characterId, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId);

	reqquest.Get(url,
		this->_requestHeader,
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}


bool WebHandler::PostCharacter(const json::value characterData, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/add";

	reqquest.Post(url,
		this->_requestHeader,
		json::serialize(characterData),
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::PostSaveCharacter(int characterId, const json::value characterData, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId);

	reqquest.Post(url,
		this->_requestHeader,
		json::serialize(characterData),
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::PostCharacterDeleteStatus(int characterId, const json::value characterData, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/deletion";

	reqquest.Post(url,
		this->_requestHeader,
		json::serialize(characterData),
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::PostCharacterNumericToken(int characterId, const json::value characterData, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/token";

	reqquest.Post(url,
		this->_requestHeader,
		json::serialize(characterData),
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

#pragma endregion
#pragma region "Items"

bool WebHandler::GetCharacterItems(int characterId, int inventoryType, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/items/" + std::to_wstring(inventoryType);

	reqquest.Get(url,
		this->_requestHeader,
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::PostCharacterItems(int characterId, const json::value items, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/items";

	reqquest.Post(url,
		this->_requestHeader,
		json::serialize(items),
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

#pragma endregion
#pragma region "Skills"

bool WebHandler::GetCharacterSkills(int characterId, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/skills";

	reqquest.Get(url,
		this->_requestHeader,
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::PostCharacterSkills(int characterId, const json::value items, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/skills";

	reqquest.Post(url,
		this->_requestHeader,
		json::serialize(items),
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

#pragma endregion
#pragma region "HotBar"

bool WebHandler::GetCharacterHotBar(int characterId, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/hotbar";

	reqquest.Get(url,
		this->_requestHeader,
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::PostCharacterHotBar(int characterId, const json::value items, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/hotbar";

	reqquest.Post(url,
		this->_requestHeader,
		json::serialize(items),
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

#pragma endregion
#pragma region "Buffs"

bool WebHandler::GetCharacterBuffs(int characterId, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/buffs";

	reqquest.Get(url,
		this->_requestHeader,
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::PostCharacterBuffs(int characterId, const json::value items, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/buffs";

	reqquest.Post(url,
		this->_requestHeader,
		json::serialize(items),
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

#pragma endregion
#pragma region "Titles"

bool WebHandler::GetCharacterTitles(int characterId, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/titles";

	reqquest.Get(url,
		this->_requestHeader,
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

bool WebHandler::PostCharacterTitles(int characterId, const json::value items, json::value* response) {
	HttpRequest reqquest(this->_domain, this->_port, this->_https);
	HttpResponse httpResponse;

	std::wstring url = L"/characters/" + std::to_wstring(characterId) + L"/titles";

	reqquest.Post(url,
		this->_requestHeader,
		json::serialize(items),
		httpResponse);

	return this->ProcessResponse(httpResponse, response);
}

#pragma endregion


