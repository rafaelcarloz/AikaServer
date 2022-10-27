#include "Nation.h"
#include "Logger.h"
#include "DateFunctions.h"
#include <iostream>

#include "ServerInstance.h"

using namespace std;
namespace json = boost::json;

bool Nation::FromJSON(boost::json::value* nationData, Nation* nation) {
	nation->nationId = json::value_to<int>(nationData->at("nationID"));
	nation->nationName = json::value_to<std::string>(nationData->at("nationName"));
	nation->nationRank = json::value_to<int>(nationData->at("nationRank"));
	nation->marshalGuildID = json::value_to<int>(nationData->at("marshalGuildID"));
	nation->taxCitizens = json::value_to<int>(nationData->at("taxCitizens"));
	nation->taxVisitors = json::value_to<int>(nationData->at("taxVisitors"));
	nation->taxSettlement = json::value_to<int>(nationData->at("taxSettlement"));
	nation->nationAlliedID = json::value_to<int>(nationData->at("nationAlliedID"));
	nation->nationAlliedDate = DateFunctions::StringToDate(json::value_to<std::string>(nationData->at("nationAlliedDate")));
	nation->nationGold = json::value_to<UINT64>(nationData->at("nationGold"));

	return true;
}

Nation Nation::GetData() {
	ServerInstance* instance = ServerInstance::GetInstance();

	return *instance->nation;
}