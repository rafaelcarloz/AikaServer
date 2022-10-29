#include "ServerInstance.h"
#include "DataLoader.h"
#include "DateFunctions.h"
#include "Player.h" 
#include "ServerSocket.h"
#include <Winsock2.h>

using namespace std;
namespace json = boost::json;

std::atomic<ServerInstance*> ServerInstance::_instance{ nullptr };
std::mutex ServerInstance::_mutex;

BOOL __stdcall ConsoleHandler(DWORD dwCtrlType) {
	if (dwCtrlType == CTRL_CLOSE_EVENT) {
		ServerInstance* serverInstance = ServerInstance::GetInstance();

		serverInstance->Close();

		return true;
	}
}

ServerInstance* ServerInstance::GetInstance()
{
	ServerInstance* tmp = _instance.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);

	if (tmp == nullptr) {
		std::lock_guard<std::mutex> lock(_mutex);

		tmp = _instance.load(std::memory_order_relaxed);
		if (tmp == nullptr) {
			tmp = new ServerInstance();
			std::atomic_thread_fence(std::memory_order_release);
			_instance.store(tmp, std::memory_order_relaxed);
		}
	}

	return tmp;
}

bool ServerInstance::LoadConfiguration() {
	try {
		std::ifstream f("GameServer.json");
		std::string input(std::istreambuf_iterator<char>(f), {});
		f.close();

		json::value configurationData = json::parse(input);

		this->version = json::value_to<int>(configurationData.at("serverSettings").at("version"));
		this->maxConnections = json::value_to<int>(configurationData.at("serverSettings").at("maxConnections"));
	}
	catch (exception e) {
		Logger::Write(e.what(), LOG_TYPE::Error);
		return false;
	}

	return true;
}

bool ServerInstance::Startup(json::value serverInfo) {
	this->serverSocket = ServerSocket(json::value_to<string>(serverInfo.at("serverAddress")), 
			(WORD)json::value_to<int>(serverInfo.at("serverPort")));

	this->serverType = json::value_to<int>(serverInfo.at("serverType"));

	if (!this->LoadConfiguration()) {
		return false;
	}

	DataLoader dataLoader;

	if (!dataLoader.Startup(this)) {
		return false;
	}

	if (!this->serverSocket.StartSocket()) {
		return false;
	}

	this->isActive = true;
	SetConsoleCtrlHandler(&ConsoleHandler, true);

	this->SetNationData(json::value_to<int>(serverInfo.at("nationID")));

	this->entityHandler = new EntityHandler();

	this->_connectionsThread = std::thread(&ServerInstance::HandleConnections, this);
	this->_connectionsThread.detach();

	this->StartRoutines();

	return true;
}

void ServerInstance::Close() {
	Logger::Write(Status, "Shutting down server...");

	this->entityHandler->DisconnectPlayers();

	this->isActive = false;

	this->serverSocket.CloseSocket();
}

void ServerInstance::HandleConnections() {
	while (this->isActive) {
		SOCKET clientSocket = INVALID_SOCKET;
		sockaddr clientAddr{};

		if (this->serverSocket.AcceptConnection(&clientSocket, &clientAddr)) {
			if (clientSocket == INVALID_SOCKET) {
				Logger::Write("received invalid socket!", Error);
				continue;
			}

			UINT32 entityId = INVALID_ENTITY;

			this->entityHandler->AddEntity(new Player(), EntityPlayer, &entityId);

			PPlayer player;

			if (!this->entityHandler->GetPlayerById(entityId, player)) {
				Logger::Write("error retriving added client!", Error);
				continue;
			}

			player->Start(clientSocket, clientAddr, entityId);
		}
		else {
			Logger::Write("connection accepting error!", Error);
		}

		this->HandleInactiveConnections();

		Sleep(100);
	}
}

void ServerInstance::HandleInactiveConnections() {
	auto entities = this->entityHandler->GetEntities();

	for (auto& entity : entities) {
		if (EntityHandler::GetEntityType(entity.first) != EntityPlayer) {
			continue;
		}

		PPlayer player;

		if (!this->entityHandler->GetPlayerById(entity.first, player)) {
			Logger::Write(Error, "[ServerInstance::HandleInactiveConnections] get player error");
			continue;
		}

		if (player->isFreeToUse == false) {
			continue;
		}

		this->entityHandler->RemoveEntity(entity.first);
	}
}


bool ServerInstance::SetNationData(int nationId) {
	string serverTypeName = (this->serverType == 1) ? "PvP" : "PvE";

	json::value* nationData = new json::value();
	
	if (!this->webHandler.GetNation(nationId, nationData)) {
		Logger::Write("failed to get nation data", Status);
		return false;
	}

	this->nation = new Nation();
	this->nation->nationId = json::value_to<int>(nationData->at("nationID"));
	this->nation->nationName = json::value_to<std::string>(nationData->at("nationName"));
	this->nation->nationRank = json::value_to<int>(nationData->at("nationRank"));
	this->nation->marshalGuildID = json::value_to<int>(nationData->at("marshalGuildID"));
	this->nation->taxCitizens = json::value_to<int>(nationData->at("taxCitizens"));
	this->nation->taxVisitors = json::value_to<int>(nationData->at("taxVisitors"));
	this->nation->taxSettlement = json::value_to<int>(nationData->at("taxSettlement"));
	this->nation->nationAlliedID = json::value_to<int>(nationData->at("nationAlliedID"));
	this->nation->nationAlliedDate = DateFunctions::StringToDate(json::value_to<std::string>(nationData->at("nationAlliedDate")));
	this->nation->nationGold = json::value_to<UINT64>(nationData->at("nationGold"));

	string consoleTitle = Logger::Format("GameServer :: %s-%s", this->nation->nationName.c_str(), serverTypeName.c_str());
	SetConsoleTitleA(consoleTitle.c_str());

	return true;
}


bool ServerInstance::SendPacketToClient(WORD clientId, LPVOID packet, WORD packetSize) {
	PPlayer client;

	if (!this->entityHandler->GetPlayerById(clientId, client)) {
		return false;
	}

	if (client->isActive == false) {
		return false;
	}

	return client->SendPacket(packet, packetSize);
}



Nation* ServerInstance::GetNationData() {
	return this->nation;
}

#pragma region "Server Threads"

bool ServerInstance::StartRoutines() {
	this->_threadUpdatedVisible = std::thread(&ServerInstance::RoutineUpdateVisibleEntities, this);
	this->_threadUpdatedVisible.detach();

	this->_threadUpdateBuffsAndStatus = std::thread(&ServerInstance::RoutineUpdateBuffsAndStatus, this);
	this->_threadUpdateBuffsAndStatus.detach();

	return true;
}


void ServerInstance::RoutineUpdateVisibleEntities() {
	while (this->isActive)
	{
		auto entities = this->entityHandler->GetEntities();

		for (auto& entity : entities) {
			if (EntityHandler::GetEntityType(entity.first) != EntityPlayer) {
				continue;
			}

			PPlayer player;

			if (!this->entityHandler->GetPlayerById(entity.first, player)) {
				Logger::Write(Error, "[ServerInstance::RoutineUpdateVisibleEntities] get player error");
				continue;
			}

			if (player->IsIstantiated != true || player->index <= 0) {
				continue;
			}

			player->visibleController->PerformUpdate();
		}

		Sleep(10);
	}
}

void ServerInstance::RoutineUpdateBuffsAndStatus() {
	while (this->isActive)
	{
		auto entities = this->entityHandler->GetEntities();

		for (auto& entity : entities) {
			if (!entity.second->IsIstantiated) {
				continue;
			}

			entity.second->statusController.DoRegenerationTick();
		}

		Sleep(100);
	}
}


#pragma endregion