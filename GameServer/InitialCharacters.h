#pragma once

#ifndef _INITIAL_CHARACTERS_H_
#define _INITIAL_CHARACTERS_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdio.h>
#include <map>

#include <boost/json.hpp>

#include "PlayerData.h"

class InitialCharacters {
private:
	static InitialCharacters* _instance;

	std::map<WORD, Character> _characters;

	Character FromJSON(boost::json::value characterData);
protected:
	InitialCharacters() { }
	~InitialCharacters() {}
public:
	InitialCharacters(InitialCharacters& other) = delete;
	void operator=(const InitialCharacters&) = delete;

	static InitialCharacters* GetInstance();

	bool Add(WORD characterClass, boost::json::value characterData);
	bool Get(WORD characterClass, Character* character);
	Character Get(WORD characterClass);
};


#endif
