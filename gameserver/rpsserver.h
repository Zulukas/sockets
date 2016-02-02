#ifndef RPS_SERVER_H
#define RPS_SERVER_H

#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <iostream>

#define BUFF_LEN 1024

using std::string;
using std::cout;
using std::endl;

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

#endif