#include <boost/json/src.hpp>
#include <boost/json/memory_resource.hpp>

#include "ServerInstance.h"

namespace json = boost::json;

int main(int argc, char* argv[]){
	std::string serverData;

	if (argc < 2){
		serverData = R"({"serverAddress":"127.0.0.1","serverPort":8822,"nationID":1, "serverType": 1})";
	}
	else {
		serverData = argv[1];
	}

	std::replace(serverData.begin(), serverData.end(), '\'', '\"');

	json::value serverInfo = json::parse(serverData);

	ServerInstance* serverInstance = ServerInstance::GetInstance();

	if (serverInstance->Startup(serverInfo)) {
		Logger::Write("Server started succesfully!", Status);
	}
	else {
		Logger::Write("Server startup error!", Error);
		return 0;
	}

	while (serverInstance->isActive) {
		Sleep(100);
	}

	return 0;
}