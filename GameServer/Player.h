#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Player.fwd.h"
#include "CharacterController.h"

#include "ServerSocket.h"

#include <thread>

#include "Logger.h"
#include "GameEntity.h"

#include "Packets.h"

#include <boost/json.hpp>

class AccountData {
public:
	long sessionId;
	long accountId;
	int accountNation;
	std::string accountUsername;

	bool FromJSON(boost::json::value data);
};

class Player : public GameEntity {
private:
	SOCKET _socket;
	sockaddr _addr;

	int _packetCount;
	std::thread _routine;

	bool isSaving = false;

	bool ReceiveNetworkData();
	
	bool GetCharacterList(packets::CharacterListData* characters);
public:
	bool isActive;
	bool isFreeToUse = true;

	PlayerStatus playingStatus;

	std::string GetClientAddress();

	AccountData* account;
	boost::json::value _characterList[3];

	CharacterController* characterController;
	InventoryController inventoryController;

	void Start(SOCKET socket, sockaddr addr, WORD index);
	void Destroy();

	void Disconnect();

	bool SaveAccount();

	void HandleNetworkRecv();
	bool SendPacket(LPVOID packet, WORD size);
	bool SendSignalData(WORD opcode, DWORD data);
	bool SendSignal(WORD opcode);

	void SendClientMessage(std::string message, BYTE color = 0, BYTE type = 16);

	void SendCharacterList();
	bool CharacterHasNumericToken(int characterSlot);
	bool CheckNumericToken(int characterSlot, char* numericToken);
	bool UpdateNumericToken(int characterSlot, char* numericToken, bool errorIncrement = false);
};

#endif
