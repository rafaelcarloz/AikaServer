#include "UseItemHandler.h"
#include "Player.h"
#include "ServerInstance.h"
#include "MapData.h"


void UseItemHandler::UseInventoryItem(PPlayer player, BYTE slot, WORD useData) {
	_player = player;
	_item = &player->character.Inventory[slot];

	_slot = slot;
	_useData = useData;

	if (_item->Index == 0) {
		return;
	}

	if (_item->Amount == 0) {
		_item->Amount = 1;
	}

	_itemData = ItemList::Get(_item->Index);

	this->_state = luaL_newstate();
	luaL_openlibs(this->_state);

	this->RunScript(_itemData.ItemType, ScriptConsumeFunction);

	lua_close(this->_state);

	if (_itemConsumed) {
		player->inventoryController.DecreaseAmount(_slot, _amountConsumed);
	}

	player->statusController.Recalculate();
}

bool UseItemHandler::RunScript(WORD scriptId, ScriptType scriptType) {
	UseItemHandler handler;
	luabridge::getGlobalNamespace(this->_state)
		.beginClass<UseItemHandler>("UseItemHandler")
		.addFunction("OpenChest", &UseItemHandler::UseItemOpenChest)
		.addFunction("AddCharacterItem", &UseItemHandler::AddCharacterItem)
		.addFunction("GetAvailableSlots", &UseItemHandler::GetIventoryAvailableSlots)
		.addFunction("SendMessage", &UseItemHandler::SendClientMessage)
		.addFunction("GetCharacterClass", &UseItemHandler::GetCharacterClass)
		.addFunction("AddCharacterHealth", &UseItemHandler::AddCharacterHealth)
		.addFunction("AddCharacterMana", &UseItemHandler::AddCharacterMana)
		.addFunction("AddCharacterBuff", &UseItemHandler::AddCharacterBuff)
		.addFunction("GetServerNation", &UseItemHandler::GetServerNation)
		.addFunction("GetCharacterNation", &UseItemHandler::GetCharacterNation)
		.addFunction("TeleportScroll", &UseItemHandler::TeleportScroll)
		.endClass();
	
	
	std::string fileName = ItemList::GetInstance()->GetUseItem(scriptId, scriptType);

	if (fileName == "") {
		return false;
	}

	int r = luaL_dofile(this->_state, fileName.c_str()) != LUA_OK;

	if (r != LUA_OK) {
		std::string errormessage = lua_tostring(this->_state, -1);
		Logger::Write(Error, "[UseItemHandler::LoadScript] type: %d error: %s  ",(BYTE)scriptType, errormessage.c_str());
		return false;
	}

	try {
		auto runScript = luabridge::getGlobal(this->_state, "run");
		runScript(this);

		if (!this->_itemConsumed) {
			lua_getglobal(this->_state, "bConsumeItem");
			this->_itemConsumed = lua_toboolean(this->_state, -1);
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[UseItemHandler::RunScript] error: %s  ",e.what());
		return false;
	}
	
	return true;
}


bool UseItemHandler::UseItemOpenChest(lua_State* L) {
	if (this->_itemData.UseEffect <= 0) {
		return false;
	}

	return this->RunScript(this->_itemData.UseEffect, ScriptConsumeEffect);
}


bool UseItemHandler::AddCharacterItem(lua_State* L) {
	uint32_t itemId = lua_tointeger(L, -2);
	uint32_t amount = lua_tointeger(L, -1);

	if (itemId <= 0 || amount <= 0) {
		return false;
	}

	if (_player->inventoryController.GetAvailableSlotsCount() <= 0) {
		_player->SendClientMessage("inventory full!");
		return false;
	}

	if (_player->inventoryController.PutItem(itemId, amount) == INVALID_SLOT) {
		return false;
	}

	return true;
}

bool UseItemHandler::AddCharacterHealth(lua_State* L) {
	_player->character.Status.Life.CurHP += _itemData.UseEffect;

	return true;
}

bool UseItemHandler::AddCharacterMana(lua_State* L) {
	_player->character.Status.Life.CurMP += _itemData.UseEffect;

	return true;
}

bool UseItemHandler::AddCharacterBuff(lua_State* L) {
	return true;
}


BYTE UseItemHandler::GetIventoryAvailableSlots(lua_State* L) {
	return this->_player->inventoryController.GetAvailableSlotsCount();
}

bool UseItemHandler::SendClientMessage(lua_State* L) {
	std::string message = lua_tostring(L, -1);

	this->_player->SendClientMessage(message);

	return true;
}

BYTE UseItemHandler::GetCharacterClass(lua_State* L) {
	return (BYTE)((BYTE)this->_player->character.ClassInfo / 10);
}

BYTE UseItemHandler::GetServerNation(lua_State* L) {
	return ServerInstance::GetInstance()->GetNationData()->nationId;
}

BYTE UseItemHandler::GetCharacterNation(lua_State* L) {
	return this->_player->account->accountNation;
}

void UseItemHandler::TeleportScroll(lua_State* L) {
	uint16_t teleportPosition = _useData;

	ScrollTeleportPosition scrollTeleport = MapData::GetTeleport(teleportPosition);

	_player->characterController->Teleport(scrollTeleport.ToPosition());
}