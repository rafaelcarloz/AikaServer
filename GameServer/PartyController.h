#pragma once

#ifndef _PARTY_CONTROLLER_H_
#define _PARTY_CONTROLLER_H_

#include <vector>

#include "Player.h"

class Party {
private:
	std::vector<PPlayer> members;
public:
	void Initialize(PPlayer leader);

	bool AddMember(PPlayer player);
	bool RemoveMember(PPlayer player);
};

typedef Party* PParty;

class Raid {

};

typedef Raid* PRaid;

class PartyController {
private:
	PParty _party;
	PRaid _raid;
public:

};


#endif