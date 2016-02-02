#include "rpsserver.h"

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

bool switcher = true;

char handleInput(const string msg)
{
	//Parse the input, and determine the action.
	srand(time(NULL));

	int num = rand() % 3;

	switch(num)
	{
		case 0:
			return 'r';
		case 1:
			return 'p';
		case 2:
			return 's';
	}

	return 'r';
}

string determineWinner(const char p1, const char p2)
{
	//Determine the winner
	if (p1 == p2)
	{
		return "0";
	}

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

int main(int argc, char** argv)
{
	int portNumber = 6789;

	if (argc >= 2)
	{
		//Convert portnumber to int
	}

	RPSServer server(portNumber);

	char p1, p2;

	for(;;)
	{
		try
		{
			//Get input from player 1
			string reply = server.readFromPlayer1();

			p1 = handleInput(reply);

			server.writeToPlayer1("ACCEPTED - your move is " + p1);

			//Get input from player 2
			reply = server.readFromPlayer2();

			p2 = handleInput(reply);

			server.writeToPlayer2("ACCEPTED - your move is " + p2);

			//Assert that the players are ready...
			// server.readFromPlayer1();
			// server.readFromPlayer2();

			string winner = determineWinner(p1, p2);

			server.writeToPlayer1(winner);
			server.writeToPlayer2(winner);
		}
		catch (string ex)
		{
			cout << ex << endl;
			return 1;
		}
	}
}