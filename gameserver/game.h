#pragma once

#include <string>
using std::string;

class Game
{
public:
	virtual string handle(string message) = 0;
};
