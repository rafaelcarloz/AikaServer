#include <ctime>

#include "PacketHandler.h"
#include "ServerInstance.h"

#include "..\GameServer\Encryptation.h"

using namespace packets;

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

	switch ((int)Header.Code) {

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

		boost::json::value *response = new boost::json::value();

		if (!webHandler.GetSessionDataByToken(username, token, response)) {
			Logger::Write(Logger::Format("account [%s] invalid token", username.c_str()), Warnings);
			client.isActive = false;
			return false;
		}

		ResponseLoginPacket PacketSend;

		ZeroMemory(&PacketSend, sizeof(PacketSend));

		PacketSend.Header.Size = sizeof(PacketSend);
		PacketSend.Header.Code = (PacketCode)0x82;
		PacketSend.Index = boost::json::value_to<WORD>(response->at("accountID"));
		PacketSend.Nation = boost::json::value_to<WORD>(response->at("accountNation"));
		PacketSend.Time = GetTickCount();

		if (!client.SendPacket(&PacketSend, PacketSend.Header.Size)) {
			Logger::Write(Logger::Format("account [%s] send packet error.", username.c_str()), LOG_TYPE::Warnings);

			return false;
		}

		Logger::Write(Logger::Format("account [%s] logged with token [%s]", username.c_str(), token.c_str()), LOG_TYPE::Packets);
	}
	catch (std::exception& e)
	{
		Logger::Write(e.what(), Error);
	}

	client.isActive = false;

	return true;
}