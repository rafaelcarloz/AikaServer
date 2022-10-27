#include <stdio.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

int main() {
	std::string serverData = string(R"({"serverAddress":"127.0.0.1","serverPort":8822,"serverType":1,"nationID":3})");
	json serverInfo = json::parse(serverData);
}