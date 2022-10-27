#pragma once

#ifndef _SERVER_INSTANCE_H_
#define _SERVER_INSTANCE_H_

#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>

#include <boost/json.hpp>

#include "ServerSocket.h"

#include "WebHandler.h"
#include "EntityHandler.h"
#include "Nation.h"

class ServerInstance {
private:
	std::thread _connectionsThread;

	std::thread _threadUpdatedVisible;

	ServerSocket serverSocket;

	static std::atomic<ServerInstance*> _instance;
	static std::mutex _mutex;

	bool LoadConfiguration();
	bool SetNationData(int nationId);
protected:
	ServerInstance() { }
	~ServerInstance() {}
public:
	bool isActive = false;

	int version = 100;
	int maxConnections = 10;
	int serverType = 1;

	
	Nation* nation;

	WebHandler webHandler;
	EntityHandler* entityHandler;

	ServerInstance(ServerInstance& other) = delete;
	void operator=(const ServerInstance&) = delete;

	static ServerInstance* GetInstance();

	bool Startup(boost::json::value serverInfo);
	void Close();

	void HandleConnections();
	void HandleInactiveConnections();

	bool SendPacketToClient(WORD clientId, LPVOID packet, WORD packetSize);
	bool SendPacketToServer();
	bool SendPacketToAll();

	Nation* GetNationData();


	bool StartRoutines();
	void RoutineUpdateVisibleEntities();
};



#endif
