#include "rpshost.h"

#include <sstream>
#include <vector>
using std::stringstream;
using std::vector;

RPSHost::RPSHost() : player1('.'), player2('.'), p1registered(false), p2registered(false)
{
}

string RPSHost::handle(const string& msg)
{
	string reply;

	vector<string> parts;
	stringstream ss(msg);

	string part;
	while (ss >> part)
	{
		parts.push_back(part);
	}

	if (parts[0] == "REGISTER")
	{
		if (!p1registered)
		{
			p1registered = true;
			reply = "P1";
		}
		if (!p2registered)
		{
			p2registered = true;
			reply = "P2";
		}
		else
		{
			reply = "FULL";
		}	
	}

	if (parts.length() == 2)
	{
		//Player disconnects
		if (parts[1] == "DISCONNECT")
		{
			if (parts[0] == "P1")
			{
				p1registered = false;
			}
			else if (parts[0] == "P2")
			{
				p2registered = false;
			}
			else
			{
				return "INVALID PLAYER COMMAND"
			}

			reply = "DISCONNECTED";
		}
	}

	if (parts.length() == 3)
	{
		if (parts[1] == "ACTION")
		{
			if ( (parts[0] != "P1" && parts[0] != "P2") || 
				 (parts[2] != "r" && parts[2] != "p" && parts[2] != "s"))
			{
				return "INVALID PLAYER COMMAND";
			}

			if (parts[0] == "P1")
			{
				if (player1 == '.') //The player can make an action
					player1 = parts[2][0];
			}
		}
	}

	// if (parts[0] == "P1" || parts[0] == "P2")
	// {
	// 	if (parts[1] == "DISCONNECT")
	// 	{
	// 		reply = "DISCONNECTED";

	// 	}
	// }
	// else
	// {
	// 	reply = "INVALID PLAYER";
	// }
}