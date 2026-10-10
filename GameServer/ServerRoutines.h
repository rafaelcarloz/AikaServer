#pragma once

#ifndef _SERVER_ROUTINES_H_
#define _SERVER_ROUTINES_H_

#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>

#include "ServerInstance.h"

class ServerRoutines {
private:
	std::thread _connectionsThread;

	std::thread _threadUpdatedVisible;
	std::thread _threadUpdateBuffsAndStatus;
	std::thread _threadMobActions;
	std::thread _threadUpdateQuests;

public:
	ServerInstance* serverInstance;

	bool StartRoutines();
	void RoutineUpdateVisibleEntities();
	void RoutineUpdateBuffsAndStatus();
	void RoutineUpdateMobActions();
	void RoutineUpdateQuests();
};



#endif
