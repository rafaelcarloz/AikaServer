#pragma once

#ifndef _NATION_H_
#define _NATION_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <wtypes.h>
#include <stdio.h>
#include <string>

#include <boost/json.hpp>

class Nation {
public:
	int nationId;
    std::string nationName;
    int nationRank;
    int marshalGuildID;
    int taxCitizens;
    int taxVisitors;
    int taxSettlement;
    int nationAlliedID;
    time_t nationAlliedDate;
    UINT64 nationGold;

    static bool FromJSON(boost::json::value* nationData, Nation* nation);
    static Nation GetData();
};

#endif