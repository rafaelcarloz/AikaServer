#include <iostream>    
#include <stdio.h>

#include "ServerInstance.h"

string currentDir = "";

void SetConsoleSpecs() {
	SMALL_RECT Rect;

	Rect.Right = 120;
	Rect.Bottom = 20;
	Rect.Left = 0;
	Rect.Top = 0;

	SetConsoleTitleA("MainServer :: Arcan");
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &Rect);
}

void InitializeServers(ServerInstance* instance) {
	json* servers = new json();

	if (!instance->webHandler.GetServers(servers)) {
		Logger::Write("No servers found!", Status);
		return;
	}

	for (auto& server : *servers) {
		string serverData = server.dump();
		std::replace(serverData.begin(), serverData.end(), '\"', '\'');

		string command = "start "+ currentDir + "\\GameServer.exe " + serverData ;
		system(command.c_str());
	}
}

int main(int argc, char* argv[]) {
	ServerInstance* serverInstance = ServerInstance::GetInstance();

	std::string cur_dir(argv[0]);
	int pos = cur_dir.find_last_of("/\\");

	currentDir = cur_dir.substr(0, pos);

	InitializeServers(serverInstance);

	if (serverInstance->Startup()) {
		Logger::Write("Server started succesfully!", Status);
	}
	else {
		Logger::Write("Server startup error!", Error);
		return 0;
	}

	SetConsoleSpecs();

	while (serverInstance->isActive) {
		Sleep(100);
	}

	getchar();

	return 0;
}