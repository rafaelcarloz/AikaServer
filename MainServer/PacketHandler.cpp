#include <ctime>

#include "PacketHandler.h"
#include "ServerInstance.h"

#include "..\GameServer\Encryptation.h"

using json = nlohmann::json;

bool PacketHandler::PacketControl(ClientConnection client, char* buffer, int receivedBytes, int initialOffset) {
	if (receivedBytes == 0 || receivedBytes < sizeof(PacketHeader)) {
		return false;
	}

	if (initialOffset > 0) {
		memmove(&buffer[0], &buffer[initialOffset], receivedBytes);
	}

	if (Encryptation::Decrypt(buffer, receivedBytes) <= 0) {
		return false;
	}

	auto& Header = reinterpret_cast<PacketHeader&>(buffer[0]);

	switch (Header.Code) {

	case 0x81:
		PacketHandler::LoginHandler(client, buffer);
		break;
	default:
		break;
	}


	return true;
}

bool PacketHandler::LoginHandler(ClientConnection client, char* buffer) {
	auto& Packet = reinterpret_cast<CheckTokenPacket&>(buffer[0]);

	std::string username(Packet.Username);
	std::string token(Packet.Token);

	try
	{
		WebHandler webHandler;

		json* response = new json();

		if (!webHandler.GetSessionDataByToken(username, token, response)) {
			Logger::Write(Logger::Format("account [%s] invalid token", username.c_str()), Warnings);
			client.isActive = false;
			return false;
		}

		ResponseLoginPacket PacketSend;

		ZeroMemory(&PacketSend, sizeof(PacketSend));

		PacketSend.Header.Size = sizeof(PacketSend);
		PacketSend.Header.Code = 0x82;
		PacketSend.Index = std::atoi((*response)["accountID"].dump().c_str());
		PacketSend.Nation = std::atoi((*response)["accountNation"].dump().c_str());
		PacketSend.Time = GetTickCount();

		if (!client.SendPacket(&PacketSend, PacketSend.Header.Size)) {
			Logger::Write(Logger::Format("account [%s] send packet error.", username.c_str()), LOG_TYPE::Warnings);

			return false;
		}

		Logger::Write(Logger::Format("account [%s] logged with token [%s]", username.c_str(), token.c_str()), LOG_TYPE::Packets);
	}
	catch (json::exception& e)
	{
		Logger::Write(e.what(), Error);
	}

	client.isActive = false;

	return true;
}