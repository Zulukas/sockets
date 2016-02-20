/***********************************************************************
* Program:
*    Lab RockClient, Rock/Paper/Scissors with Sockets - Client Code
*    Brother Jones, CS 460
* Author:
*    Kevin Andres
* Summary:
*    Game client for rock-paper-scissors
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 1024

/******************************************************************************
 * ClientSocket class, contains information for how to communicate with the 
 * server.
 *****************************************************************************/
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

/******************************************************************************
 * Create a client socket object, telling it where to look for for the server.
 *****************************************************************************/
ClientSocket::ClientSocket(string serverString, int portNumber) throw (const string)
{
    portno = portNumber;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
    {
        throw string("ERROR: unable to open socket");
    }
    
    server = gethostbyname(serverString.c_str());

    if (server == NULL) 
    {
        throw string("ERROR: no such host at " + serverString + ".");            
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    
    serv_addr.sin_port = htons(portno);
    
    int result = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    if (result < 0) 
        throw string("ERROR: unable to connect.  Is the server up?");

    player = readFromServer();
}

/******************************************************************************
 * Destroy the object, closes the connection.
 *****************************************************************************/
ClientSocket::~ClientSocket()
{
    close(sockfd);
}

/******************************************************************************
 * Close connection with the server
 *****************************************************************************/
void ClientSocket::clientDisconnect() throw (const string)
{
    close(sockfd);
}

/******************************************************************************
 * Writes messages to the server
 *****************************************************************************/
void ClientSocket::writeToServer(const string message) throw (const string)
{
    n = write(sockfd, message.c_str(), message.length());
    
    if (n < 0) 
         throw string("ERROR: unable to write to socket");
    
    bzero(buffer, BUFFER_SIZE);
}

/******************************************************************************
 * Reads messages from the server
 *****************************************************************************/
string ClientSocket::readFromServer() throw (const string)
{
    n = read(sockfd, buffer, BUFFER_SIZE);
    if (n < 0) 
        throw string("ERROR: unable to read from socket");
    
    string reply = string(buffer);
    cout << reply << endl;
    bzero(buffer, BUFFER_SIZE);

    return reply;
    
}

/******************************************************************************
 * Main - contains the main game logic.
 *****************************************************************************/
int main(int argc, char *argv[])
{
    string address = "127.0.0.1";   //Default server location set to localhost
    int portNumber = 6789;          //on port 6789

    if (argc >= 3)                  //Given command line arguments, use the provided info.
    {
        address = argv[1];          //This plays nicely with the definitions in /etc/hosts
        portNumber = atoi(argv[2]);
    }

    try
    {
        bool timeToMakeAction = true;               //Simple state indicator
        ClientSocket mySocket(address, portNumber); //Establish the socket

        for(;;)     //FORever loop; get the pun? Haha.
        {
        	string message;

            if (timeToMakeAction)
            {
    	    	cout << mySocket.getPlayer() << ": Enter your move (quit): ";
    	    	getline(cin, message);

                if (message == "quit")
                {
                    mySocket.clientDisconnect();
                    return 0;
                }
                if (message[0] != 'r' && message[0] != 'p' && message[0] != 's')
                {
                    cout << "Invalid move, try again!\n";
                    continue;
                }
                else
                {
                    timeToMakeAction = false;
                    mySocket.writeToServer(message);
                }
            }
            else
            {
                mySocket.writeToServer("WAITING");
                timeToMakeAction = true;
            }


        	message = mySocket.readFromServer();
        	// mySocket.clientDisconnect();

            stringstream ss(message);

            string parts[3];
            ss >> parts[0];
            ss >> parts[1];
            ss >> parts[2];

        	if (parts[2] == "0")
            {
                cout << "Tie!\n";
            }
            if (parts[2] == "1")
            {
                cout << "Player 1 Wins!\n";
            }
            if (parts[2] == "2")
            {
                cout << "Player 2 Wins!\n";
            }
        }
	}
	catch (string ex)
	{
		cout << ex << endl;
		return 1;
	}

    return 0;
}
