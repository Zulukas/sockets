#include "rpsserver.h"

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

RPSServer::~RPSServer()
{
	close(serverFD);
	close(playerFD_one);
	close(playerFD_two);
}

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

void RPSServer::writeToPlayer1(const string msg) throw (const string)
{
	cout << "SERVER: Writing '" << msg << "' to Player 1.\n";

	//Respond to the client
	int n = write(playerFD_one, msg.c_str(), msg.length() + 1);

	if (n < 0)
		throw string("ERROR: failed to write to Player 1 socket.");

	cout << "SERVER: Player 1 - Message sent." << endl;
}

void RPSServer::writeToPlayer2(const string msg) throw (const string)
{
	cout << "SERVER: Writing '" << msg << "' to Player 2.\n";
	//Respond to the client
	int n = write(playerFD_two, msg.c_str(), msg.length() + 1);

	if (n < 0)
		throw string("ERROR: failed to write to Player 2 socket.");

	cout << "SERVER: Player 2 - Message sent." << endl;
}
