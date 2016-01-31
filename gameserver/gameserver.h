#pragma once

#include "server.h"

#include <time.h>

#include <string>
#include <algorithm>
using std::string;

class GameServer
{
public:
	GameServer(int portNumber);

	void loop();
private:
	Server server;
	Game* game;
};