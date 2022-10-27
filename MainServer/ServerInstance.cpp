#include "ServerInstance.h"

ServerInstance* ServerInstance::_instance{ nullptr };
std::mutex ServerInstance::_mutex;

ServerInstance* ServerInstance::GetInstance()
{
	std::lock_guard<std::mutex> lock(_mutex);

	if (_instance == nullptr)
	{
		_instance = new ServerInstance();
	}
	return _instance;
}

bool ServerInstance::LoadConfiguration() {
	try {
		std::ifstream f("MainServer.json");
		json configurationData = json::parse(f);

		string address = configurationData["serverSettings"]["address"];
		WORD port = std::stoi(configurationData["serverSettings"]["port"].dump());

		this->version = configurationData["serverSettings"]["version"];
		this->maxConnections = configurationData["serverSettings"]["maxConnections"];

		this->serverSocket = ServerSocket(address, port);
	}
	catch (exception e) {
		Logger::Write(e.what(), LOG_TYPE::Error);
		return false;
	}

	return true;
}

bool ServerInstance::Startup() {
	if (!this->LoadConfiguration()) {
		return false;
	}

	if (!this->serverSocket.StartSocket()) {
		return false;
	}

	this->isActive = true;

	this->_connectionsThread = std::thread(&ServerInstance::HandleConnections, this);

	return true;
}

void ServerInstance::Close() {
	this->isActive = false;

	this->serverSocket.CloseSocket();
}

void ServerInstance::HandleConnections() { 
	while (this->isActive)
	{
		SOCKET clientSocket = INVALID_SOCKET;
		sockaddr clientAddr{};

		if (this->serverSocket.AcceptConnection(&clientSocket, &clientAddr)) {
			ClientConnection clientConnection = ClientConnection();

			if (clientSocket == INVALID_SOCKET) {
				Logger::Write("received invalid socket!", Error);
				continue;
			}

			this->clients.push_back(clientConnection);

			this->clients.back().Start(clientSocket, clientAddr);
		}
		else {
			Logger::Write("connection accepting error!", Error);
		}

		this->HandleInactiveConnections();

		Sleep(100);
	}
}

void ServerInstance::HandleInactiveConnections() {
	this->clients.remove_if([](ClientConnection client) { return (client.isActive == false); });
}