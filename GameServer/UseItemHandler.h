#pragma once

#ifndef _USE_ITEM_HANDLER_H_
#define _USE_ITEM_HANDLER_H_

#include "Player.fwd.h"
#include "ItemList.h"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#pragma comment(lib, "lua.lib")

enum ScriptType : BYTE {
	ScriptConsumeFunction,
	ScriptConsumeEffect
};

class UseItemHandler {
private:
	PPlayer _player;

	BYTE _slot;
	WORD _useData;

	PItem _item;
	ItemFromList _itemData;

	bool _itemConsumed = false;
	WORD _amountConsumed = 1;

	lua_State* _state;

	bool RunScript(WORD scriptId, ScriptType scriptType);

	bool UseItemOpenChest(lua_State* L);


	bool AddCharacterItem(lua_State* L);
	bool AddCharacterHealth(lua_State* L);
	bool AddCharacterMana(lua_State* L);
	bool AddCharacterBuff(lua_State* L);

	BYTE GetIventoryAvailableSlots(lua_State* L);
	bool SendClientMessage(lua_State* L);
	BYTE GetCharacterClass(lua_State* L);
	BYTE GetServerNation(lua_State* L);
	BYTE GetCharacterNation(lua_State* L);

	void TeleportScroll(lua_State* L);
public:
	void UseInventoryItem(PPlayer player, BYTE slot, WORD useData);
};

#endif
