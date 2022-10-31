#include <cmath>

#include "Player.h"
#include "Encryptation.h"
#include "PacketHandler.h"
#include "DateFunctions.h"
#include "InitialCharacters.h"
#include "WebHandler.h"
#include <iostream>

using namespace std;
using namespace packets;

namespace json = boost::json;

bool AccountData::FromJSON(boost::json::value data)
{
	try {
		this->sessionId = boost::json::value_to<int>(data.at("sessionID"));
		this->accountId = boost::json::value_to<int>(data.at("accountID"));
		this->accountNation = boost::json::value_to<int>(data.at("accountNation"));

		this->accountUsername = json::value_to<std::string>(data.at("accountUsername"));
	}
	catch (std::exception e) {
		Logger::Write(e.what(), Error);
		return false;
	}

	return true;
}


void Player::Start(SOCKET socket, sockaddr addr, WORD index)
{
	this->_socket = socket;
	this->_addr = addr;

	this->index = index;

	this->isActive = true;
	this->isFreeToUse = false;

	this->playingStatus = WaitingLogin;

	this->account = new AccountData();
	this->characterController = new CharacterController();

	Logger::Write(Logger::Format("client [%d] connected with address: %s", this->index, this->GetClientAddress().c_str()), ConnectionsTrafic);

	_routine = std::thread(&Player::HandleNetworkRecv, this);
	_routine.detach();
}

void Player::Destroy() 
{
	this->isActive = false;

	try {
		this->characterController->UnspawnCharacter(false, DeleteDisconnect);
		this->SaveAccount();

		closesocket(this->_socket);
	}
	catch (exception e) {
		Logger::Write(e.what(), Error);
	}

	this->IsIstantiated = false;
}


void Player::Disconnect() {
	this->Destroy();
}


bool Player::SaveAccount() {
	if (!this->IsIstantiated) {
		return true;
	}

	return (this->characterController->SaveCharacter());
}


std::string Player::GetClientAddress() {
	struct sockaddr_in* addrIn = (struct sockaddr_in*)&this->_addr;
	struct in_addr ipAddr = addrIn->sin_addr;

	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);

	return std::string(str);
}

bool Player::ReceiveNetworkData() {
	char buffer[8000];
	int receivedBytes = 0;
	int initialOffset = 0;

	ZeroMemory(&buffer, 8000);

	receivedBytes = recv(this->_socket, buffer, sizeof(buffer), 0);

	if (receivedBytes <= 0) {
		Logger::Write(Error, "socket reading error on: %s", this->GetClientAddress().c_str());
		return false;
	}

	if (receivedBytes < 12) {
		this->_packetCount++;
		return true;
	}

	if (this->_packetCount == 0 && receivedBytes > 60) {
		initialOffset = 4;
		receivedBytes -= 4;
		this->_packetCount++;
	}

	return PacketHandler::PacketControl(this, buffer, receivedBytes, initialOffset);
}

void Player::HandleNetworkRecv() {
	while (this->isActive) {
		if (this->_socket == INVALID_SOCKET) {
			this->isActive = false;
			continue;
		}

		try {
			if (!this->ReceiveNetworkData()) {
				this->isActive = false;
			}
		}
		catch (exception e) {
			Logger::Write(Error, "[Player::HandleNetworkRecv] error: %s", e.what());
			this->isActive = false;
		}

		Sleep(1);
	}

	Logger::Write(Logger::Format("client [%d] disconnected with address: [%s]", this->index, this->GetClientAddress().c_str()), ConnectionsTrafic);
	this->Destroy();
	this->isFreeToUse = true;
}

bool Player::SendPacket(LPVOID packet, WORD size) {
	char* buffer = new char[size];
	char* bufferDest = new char[size];

	memcpy(&buffer[0], packet, size);

	if (Encryptation::Encrypt(bufferDest, buffer, size) <= 0) {
		return false;
	}

	if (send(this->_socket, bufferDest, size, 0) <= 0) {
		return false;
	}

	//printf("sent packet size: %d \n", size);

	return true;
}

bool Player::SendSignalData(PacketCode opcode, DWORD data) {
	PacketSignalData Packet;

	ZeroMemory(&Packet, sizeof(Packet));

	Packet.Header.Size = sizeof(Packet);
	Packet.Header.Index = this->index;
	Packet.Header.Code = (PacketCode)opcode;

	Packet.Data = data;

	return this->SendPacket(&Packet, Packet.Header.Size);
}

bool Player::SendSignal(PacketCode opcode) {
	PacketHeader Packet;

	ZeroMemory(&Packet, sizeof(Packet));

	Packet.Size = sizeof(Packet);
	Packet.Index = this->index;
	Packet.Code = opcode;


	return this->SendPacket(&Packet, Packet.Size);
}


#pragma region "Client Message Functions"

void Player::SendClientMessage(std::string message, BYTE color, BYTE type) {
	PacketSendMessage Packet;

	ZeroMemory(&Packet, sizeof(Packet));

	Packet.Header.Size = sizeof(Packet);
	Packet.Header.Code = PacketCode::PacketSendMessage;

	Packet.Color = (MESSAGE_COLOR)color;
	Packet.Type = (MESSAGE_TYPE)type;

	strcpy_s(Packet.Message, message.c_str());

	this->SendPacket(&Packet, Packet.Header.Size);
}

#pragma endregion
#pragma region "Character List Functions"

bool Player::GetCharacterList(packets::CharacterListData *characters) {
	WebHandler webHandler;

	json::value* response = new json::value();

	if (!webHandler.GetCharactersList(this->account->accountId, response)) {
		this->isActive = false;
		return false;
	}

	InitialCharacters* initialCharacters = InitialCharacters::GetInstance();

	try {
		for (auto& character : response->as_array()) {
			Character baseCharacter = Character();

			int slot = json::value_to<int>(character.at("characterSlot"));

			this->_characterList[slot] = character;

			characters[slot].ClassInfo = (CharacterClass)json::value_to<int>(character.at("characterClass"));

			if (!initialCharacters->Get((WORD)characters[slot].ClassInfo, &baseCharacter)) {
				return false;
			}

			long characterID = json::value_to<int>(character.at("characterID"));

			strcpy_s(characters[slot].Name, json::value_to<std::string>(character.at("characterName")).c_str());

			characters[slot].Nation = this->account->accountNation;

			characters[slot].Sizes = baseCharacter.Status.Sizes;

			characters[slot].Attributes = {
				(WORD)(baseCharacter.Status.Attributes.Strength + (character.at("addinitionalStrength").is_null() ? 0
					: json::value_to<int>(character.at("addinitionalStrength")))),
				(WORD)(baseCharacter.Status.Attributes.Dexterity + (character.at("addinitionalAgility").is_null() ? 0
					: json::value_to<int>(character.at("addinitionalAgility")))),
				(WORD)(baseCharacter.Status.Attributes.Intelligence + (character.at("addinitionalIntelligence").is_null() ? 0
					: json::value_to<int>(character.at("addinitionalIntelligence")))),
				(WORD)(baseCharacter.Status.Attributes.Constitution + (character.at("addinitionalConstituion").is_null() ? 0
					: json::value_to<int>(character.at("addinitionalConstituion")))),
				(WORD)(baseCharacter.Status.Attributes.Spirit + (character.at("addinitionalLuck").is_null() ? 0
					: json::value_to<int>(character.at("addinitionalLuck")))),
				(WORD)(baseCharacter.Status.Attributes.Status)
			};

			characters[slot].Level = json::value_to<int>(character.at("characterLevel"));
			characters[slot].Exp = json::value_to<UINT64>(character.at("currentExperience"));

			characters[slot].Gold = json::value_to<UINT64>(character.at("characterGold"));

			characters[slot].NumRegistered = (character.at("numericToken").is_null()) ? 0 : 1;
			characters[slot].NumericError = json::value_to<int>(character.at("numericTokenError"));

			characters[slot].DeleteTime = (DWORD)((character.at("scheduledDeleteOn").is_null()) ? 0 :
				DateFunctions::StringToDate(json::value_to<std::string>(character.at("scheduledDeleteOn"))));

			json::value response = json::value();

			if (!webHandler.GetCharacterItems(characterID, 0, &response)) {
				this->isActive = false;
				return false;
			}

			for (auto& item : response.as_array()) {
				int itemSlot = json::value_to<int>(item.at("inventorySlot"));

				if (itemSlot > 7) {
					continue;
				}

				characters[slot].Equipaments[itemSlot] = json::value_to<int>(item.at("itemAppearanceID"));

				if (itemSlot == 5) {
					characters[slot].ItemEffects[7] = (json::value_to<int>(item.at("itemAmount")) / 0x10);
				}
			}
		}

	}
	catch (std::exception e) {
		Logger::Write(Error, "[Player::GetCharacterList] error: %s", e.what());
		return false;
	}

	return true;
}

void Player::SendCharacterList() {
	PacketCharacterList Packet;

	ZeroMemory(&Packet, sizeof(Packet));

	try {
		Packet.Header.Size = sizeof(Packet);
		Packet.Header.Code = PacketCode::PacketCharacterList;

		if (!this->GetCharacterList(Packet.CharactersData)) {
			Logger::Write(Logger::Format("account [%s] getting characters error", this->account->accountUsername.c_str()), LOG_TYPE::Warnings);
			return;
		}

		Packet.AccountIndex = this->account->accountId;

		this->SendPacket(&Packet, Packet.Header.Size);
	}
	catch (std::exception e) {
		Logger::Write(Error, "[Player::SendCharacterList] error: %s", e.what());
		return;
	}

	this->playingStatus = PlayerStatus::CharList;
}

bool Player::CharacterHasNumericToken(int characterSlot) {
	bool result = false;

	try {
		result = (this->_characterList[characterSlot].at("numericToken").is_null()) ? false : true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[Player::CharacterHasNumericToken] error: %s", e.what());
		return result;
	}

	return result;
}

bool Player::CheckNumericToken(int characterSlot, char* numericToken) {
	if (!this->CharacterHasNumericToken(characterSlot)) {
		return true;
	}

	try {
		std::string currentNumeric = json::value_to<std::string>(this->_characterList[characterSlot].at("numericToken"));

		if (strcmp(currentNumeric.c_str(), numericToken) == 0) {
			return true;
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[Player::CheckNumericToken] error: %s", e.what());
		return false;
	}

	return false;
}

bool Player::UpdateNumericToken(int characterSlot, char* numericToken, bool errorIncrement) {
	long characterId = 0;
	int numericErrors = 0;

	try {
		characterId = json::value_to<int>(this->_characterList[characterSlot].at("characterID"));
		numericErrors = json::value_to<int>(this->_characterList[characterSlot].at("numericTokenError"));
	}
	catch (std::exception e) {
		Logger::Write(Error, "[Player::UpdateNumericToken] error: %s", e.what());
		return false;
	}

	json::value tokenData = json::value();

	json::object& data = tokenData.emplace_object();

	if (errorIncrement) {
		numericErrors++;
	}
	else {
		numericErrors = 0;
	}

	data.emplace("CharacterID", characterId);

	if (numericToken != NULL) {
		std::string _numeric = "";

		for (int i = 0; i < 4; i++) {
			_numeric += numericToken[i];
		}

		data.emplace("NumericToken", _numeric);
	}

	data.emplace("NumericErrors", numericErrors);

	WebHandler webHandler;

	json::value response = json::value();

	if (!webHandler.PostCharacterNumericToken(characterId, tokenData, &response)) {
		this->SendClientMessage("error on saving character token request");
		return false;
	}

	return true;
}

#pragma endregion