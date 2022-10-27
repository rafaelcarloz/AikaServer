#include "ClientConnection.h"
#include "PacketHandler.h"

using namespace std;

void ClientConnection::Start(const SOCKET socket, const sockaddr addr) {
	this->_socket = socket;
	this->_addr = addr;

	this->GetClientAddress();

	this->isActive = true;

	Logger::Write("client connected with address: " + this->remoteAddress, ConnectionsTrafic);

	thread routine = std::thread(&ClientConnection::HandleNetworkRecv, this);
	routine.detach();
}

void ClientConnection::Destroy() {
	try {
		closesocket(this->_socket);
	}
	catch (exception e) {
		Logger::Write(e.what(), Error);
	}
}

void ClientConnection::GetClientAddress() {
	struct sockaddr_in* addrIn = (struct sockaddr_in*)&this->_addr;
	struct in_addr ipAddr = addrIn->sin_addr;

	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);

	this->remoteAddress = string(str);
}

bool ClientConnection::ReceiveNetworkData() {
	char buffer[8000];
	int receivedBytes = 0;
	int initialOffset = 0;

	ZeroMemory(&buffer, 8000);

	receivedBytes = recv(this->_socket, buffer, sizeof(buffer), 0);

	if (receivedBytes <= 0) {
		return false;
	}

	if (receivedBytes < 12) {
		this->_packetCount++;
		return true;
	}

	if (this->_packetCount == 0 && receivedBytes > 1116) {
		initialOffset = 4;
		receivedBytes -= 4;
		this->_packetCount++;
	}

	PacketHandler packetHandler;

	return packetHandler.PacketControl(*this, buffer, receivedBytes, initialOffset);
}

void ClientConnection::HandleNetworkRecv() {
	while (this->isActive)
	{
		if (this->_socket == INVALID_SOCKET) {
			this->isActive = false;
			continue;
		}

		try {
			if (!this->ReceiveNetworkData()) {
				this->isActive = false;
				Logger::Write("socket reading error on: " + this->remoteAddress, Error);
			}
		}
		catch (exception e) {
			Logger::Write(e.what(), Error);
			this->Destroy();

			return;
		}

		Sleep(100);
	}

	Logger::Write("client disconnected with address: " + this->remoteAddress, ConnectionsTrafic);
	this->Destroy();
}

bool ClientConnection::SendPacket(LPVOID packet, WORD size) {
	char* buffer = new char[size];
	char* bufferDest = new char[size];

	memcpy(&buffer[0], packet, size);

	if (Encryptation::Encrypt(bufferDest, buffer, size) <= 0) {
		return false;
	}

	if (send(this->_socket, bufferDest, size, 0) <= 0) {
		return false;
	}

	return true;
}