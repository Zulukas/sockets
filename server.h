#pragma once

#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <iostream>


using std::string;
using std::cout;
using std::endl;

#define BUFFER_LENGTH 1024

class ServerSocket
{
public:
	ServerSocket(int portNumber) throw (const string);
	~ServerSocket();

	string serverListen() throw (const string);
	void serverRespond(const string message) throw (const string);

	void serverConnect() throw (const string);
	void serverDisconnect() throw (const string);

private:
	int socketFD,		//Server Socket
		newSocketFD,	//Client Socket
		portNumber,		//Port being used by server
		n;

	socklen_t clientLength;
	char buffer[BUFFER_LENGTH];

	struct sockaddr_in serverAddress, clientAddress;
};