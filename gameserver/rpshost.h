#include "game.h"

class RPSHost
{
public:
	RPSHost();

	string handle(const string& msg);
private:
	char player1;
	char player2;

	bool p1registered;
	bool p2registered;
};