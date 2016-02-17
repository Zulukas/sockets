#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::cin;
using std::endl;

#define BUFFER_SIZE 1024

class ClientSocket
{
public:
    ClientSocket(string serverString, int portNumber) throw (const string);
    ~ClientSocket();

    void clientConnect() throw (const string);
    void clientDisconnect() throw (const string);
    void writeToServer(const string message) throw (const string);
    string readFromServer() throw (const string);

    string getPlayer() { return player; }

private:
    int sockfd,
        portno,
        n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[BUFFER_SIZE];

    string player;
};

#endif