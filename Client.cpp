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
    ClientSocket(string serverString, int portNumber) throw (const string)
    {
        portno = portNumber;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) 
            throw string("ERROR opening socket");
        server = gethostbyname(serverString.c_str());
        if (server == NULL) 
        {
            throw string("ERROR, no such host.");            
        }

        bzero((char *) &serv_addr, sizeof(serv_addr));
        
        serv_addr.sin_family = AF_INET;
        
        bcopy((char *)server->h_addr, 
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        
        serv_addr.sin_port = htons(portno);
    }

    void writeToServer(const string message) throw (const string)
    {
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
            throw string("ERROR connecting");

        n = write(sockfd, message.c_str(), message.length());
        
        if (n < 0) 
             throw string("ERROR writing to socket");
        
        bzero(buffer,256);
    }

    string readFromServer() throw (const string)
    {
        n = read(sockfd,buffer,255);
        if (n < 0) 
             throw string("ERROR reading from socket");
        printf("%s\n",buffer);
        close(sockfd);

        return string("All good!");
    }

private:
    int sockfd,
        portno,
        n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[BUFFER_SIZE];
};

int main(int argc, char *argv[])
{
    string address = "127.0.0.1";
    int portNumber = 6789;

    if (argc >= 3)
    {
        address = argv[1];
        portNumber = atoi(argv[2]);
    }

    ClientSocket mySocket(address, portNumber);

    cout << "Enter a message: ";
    string message;
    // cin.getline(message);
    getline(cin, message);

    mySocket.writeToServer(message);
    mySocket.readFromServer();
    return 0;
}