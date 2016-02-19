/***********************************************************************
* Program:
*    Lab Sockets
*    Brother Jones, CS460
* Author:
*    Kevin Andres
************************************************************************/

#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <iostream>
#include <string>

using namespace std;

#define BUFF_LEN 1024

/******************************************************************************
 * RPSServer Class;  contains the server socket and two client sockets.
 *****************************************************************************/
class RPSServer
{
public:
	RPSServer(int portNumber) throw (const string);	//Default = 6789
	~RPSServer();

	string readFromPlayer1() throw (const string);
	string readFromPlayer2() throw (const string);
	void writeToPlayer1(const string msg) throw (const string);
	void writeToPlayer2(const string msg) throw (const string);

private:
	int serverFD,			//Server File Descriptor
		playerFD_one,		//Player 1 File Descriptor
		playerFD_two,		//Player 2 File Descriptor
		portNumber;			//Port Number

	socklen_t player_one_length,
			  player_two_length;

	struct sockaddr_in  serverAddress,
						player_one_address,
						player_two_address;

	char buffer[BUFF_LEN];
};

/******************************************************************************
 * RPSConstructor takes a port number as a parameter.
 *****************************************************************************/
RPSServer::RPSServer(int portNumber = 6789) throw (const string)
{
	cout << "SERVER: Initializing Server Socket...\n";

	//Initialize the socket
	serverFD = socket(AF_INET, SOCK_STREAM, 0);

	if (serverFD < 0)
	{
		throw string("ERROR: Socket failed to open.");
	}

	//Clear the server address buffer
	bzero((char*) &serverAddress, sizeof(serverAddress));

	//Set the port number of this server
	this->portNumber = portNumber;

	//Set the server address object
	serverAddress.sin_family = AF_INET;				//IPv4
	serverAddress.sin_addr.s_addr = INADDR_ANY;		//The server may be any IP
	serverAddress.sin_port   = htons(portNumber);

	//Bind the socket
	if (bind(serverFD,
		     (struct sockaddr *) &serverAddress,
		     sizeof(serverAddress)) < 0)
	{
		throw string("ERROR: Socket failed to bind.");
	}

	cout << "SERVER: Listening...\n";
	listen(serverFD, 2);

	//////////////////////////////////////////////////////////

	cout << "Waiting for Player 1...\n";

	player_one_length = sizeof(player_one_address);

	playerFD_one = accept(serverFD,
		                (struct sockaddr*) &player_one_address,
		                &player_one_length);

	if (playerFD_one < 0)
	{
		throw string("Error: failed to accept client.");
	}

	cout << "SERVER: Player 1 registered.\n";
	writeToPlayer1("Player 1");

	//////////////////////////////////////////////////////////

	cout << "Waiting for Player 2...\n";

	player_two_length = sizeof(player_two_address);

	playerFD_two = accept(serverFD,
		                (struct sockaddr*) &player_two_address,
		                &player_two_length);

	if (playerFD_two < 0)
	{
		throw string("Error: failed to accept client.");
	}

	cout << "SERVER: Player 2 registered.\n";
	writeToPlayer2("Player 2");
}

/******************************************************************************
 * RPSServer destructor
 *****************************************************************************/
RPSServer::~RPSServer()
{
	close(serverFD);
	close(playerFD_one);
	close(playerFD_two);
}

/******************************************************************************
 * Reads from the 1st socket
 *****************************************************************************/
string RPSServer::readFromPlayer1() throw (const string)
{
	cout << "SERVER: Reading from Player 1...\n";
	//Clear the buffer
	bzero(buffer, BUFF_LEN);

	//Read from the socket to the buffer
	int n = read(playerFD_one, buffer, BUFF_LEN);

	if (n < 0)
		throw string("Error: failed to read from Player 1.");

	cout << "SERVER: Player 1 - Message received " << string(buffer) << endl;

	string reply = string(buffer);
	bzero(buffer, BUFF_LEN);	//Clear the buffer when we're done.

	return reply;
}

/******************************************************************************
 * Reads from the 2nd socket
 *****************************************************************************/
string RPSServer::readFromPlayer2() throw (const string)
{
	cout << "SERVER: Reading from Player 2...\n";
	//Clear the buffer
	bzero(buffer, BUFF_LEN);

	//Read from the socket to the buffer
	int n = read(playerFD_two, buffer, BUFF_LEN);

	if (n < 0)
		throw string("Error: failed to read from Player 2.");

	cout << "SERVER: Player 2 - Message received " << string(buffer) << endl;

	string reply = string(buffer);
	bzero(buffer, BUFF_LEN);	//Clear the buffer when we're done.

	return reply;
}

/******************************************************************************
 * Writes a message to the 1st socket
 *****************************************************************************/
void RPSServer::writeToPlayer1(const string msg) throw (const string)
{
	cout << "SERVER: Writing '" << msg << "' to Player 1.\n";

	//Respond to the client
	int n = write(playerFD_one, msg.c_str(), msg.length() + 1);

	if (n < 0)
		throw string("ERROR: failed to write to Player 1 socket.");

	cout << "SERVER: Player 1 - Message sent." << endl;
}

/******************************************************************************
 * Writes a message to the 2nd socket
 *****************************************************************************/
void RPSServer::writeToPlayer2(const string msg) throw (const string)
{
	cout << "SERVER: Writing '" << msg << "' to Player 2.\n";
	//Respond to the client
	int n = write(playerFD_two, msg.c_str(), msg.length() + 1);

	if (n < 0)
		throw string("ERROR: failed to write to Player 2 socket.");

	cout << "SERVER: Player 2 - Message sent." << endl;
}

/******************************************************************************
 * Determines the winner between player 1 and player 2.
 *****************************************************************************/
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

/******************************************************************************
 * Main function; contains the bulk of the game
 *****************************************************************************/
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
