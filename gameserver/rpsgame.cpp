#include "rpsserver.h"

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

string determineWinner(const char p1, const char p2)
{
	//Determine the winner
	if (p1 == p2)
	{
		return "0";
	}

	if (p1 != 's' && p1 != 'p' && p1 != 'r')
		return "2";
	if (p2 != 's' && p2 != 'p' && p2 != 'r')
		return "1";

	switch (p1)
	{
		case 'r':
			if (p2 == 'p')
			{
				return "2";
			}
			else
			{
				return "1";
			}
		case 'p':
			if (p2 == 's')
			{
				return "2";
			}
			else
			{
				return "1";
			}
		case 's':
			if (p2 == 'r')
			{
				return "2";
			}
			else
			{
				return "1";
			}
	}
}

// void initServer(RPSServer &server, int portNumber)
// {
// 	server = RPSServer(portNumber);
// }

int main(int argc, char** argv)
{
	int portNumber = 6789;

	if (argc >= 2)
	{
		//Convert portnumber to int
		portNumber = atoi(argv[1]);
	}

	RPSServer server(portNumber);

	char p1, p2;

	for(;;)
	{
		try
		{
			//Get input from player 1
			string reply = server.readFromPlayer1();

			if (reply == "quit")
			{
				return 0;
			}

			p1 = reply[0];

			string acceptedMsg = "ACCEPTED - your move is ";
			acceptedMsg += p1;

			server.writeToPlayer1("Player 1: " + acceptedMsg);

			//Get input from player 2
			reply = server.readFromPlayer2();

			if (reply == "quit")
			{
				return 0;
			}

			p2 = reply[0];

			acceptedMsg = "ACCEPTED - your move is ";
			acceptedMsg += p2;

			server.writeToPlayer2("Player 2: " + acceptedMsg);

			string winner = determineWinner(p1, p2);

			//Assert that the players are ready...
			server.readFromPlayer1();
			server.writeToPlayer1("Player 1: " + winner);
			server.readFromPlayer2();
			server.writeToPlayer2("Player 2: " + winner);
		}
		catch (string ex)
		{
			cout << ex << endl;
			return 1;
		}
	}
}
