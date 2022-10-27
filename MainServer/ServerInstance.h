#pragma once

#include "..\GameServer\ServerSocket.h"
#include <nlohmann/json.hpp>

#include <iostream>
#include <iterator>
#include <list>
#include <thread>
#include <fstream>
#include <mutex>

#include "..\GameServer\WebHandler.h"

#include "ClientConnection.h"


using namespace std;
using json = nlohmann::json;

class ServerInstance {
private:
	std::thread _connectionsThread;

	ServerSocket serverSocket;

	static ServerInstance* _instance;
	static std::mutex _mutex;

	bool LoadConfiguration();

protected:
	ServerInstance(){ }
	~ServerInstance() {}
public:
	bool isActive = false;

	int version = 100;
	int maxConnections = 10;

	std::list<ClientConnection> clients;

	WebHandler webHandler;

	ServerInstance(ServerInstance& other) = delete;
	void operator=(const ServerInstance&) = delete;

	static ServerInstance* GetInstance();

	bool Startup();
	void Close();

	void HandleConnections();
	void HandleInactiveConnections();
};
