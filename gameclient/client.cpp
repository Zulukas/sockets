#include "client.h"


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
}

ClientSocket::~ClientSocket()
{
    close(sockfd);
}

void ClientSocket::clientDisconnect() throw (const string)
{
    close(sockfd);
}

void ClientSocket::writeToServer(const string message) throw (const string)
{
    n = write(sockfd, message.c_str(), message.length());
    
    if (n < 0) 
         throw string("ERROR: unable to write to socket");
    
    bzero(buffer, BUFFER_SIZE);
}

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