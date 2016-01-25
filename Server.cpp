#include "Server.h"

class GameHost
{
public:
	GameHost() : player1_turn(true) {}

	void playerSwitch() { player1_turn = !player1_turn; }

private:
	bool player1_turn;
};

ServerSocket::ServerSocket(int portNumber) throw (const string)
{
	cout << "SERVER: Initializing Server Socket...\n";
	//Initialize the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0);

	if (socketFD < 0)
	{
		throw string("ERROR: Socket failed to open.");
	}

	//Clear the server address buffer
	bzero((char*) &serverAddress, sizeof(serverAddress));

	//Set the port number of this server
	this->portNumber = portNumber;

	//Set the server address object
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port   = htons(portNumber);

	//Bind the socket
	if (bind(socketFD, 
		     (struct sockaddr *) &serverAddress, 
		     sizeof (serverAddress)) < 0)
	{
		throw string("ERROR: Socket failed to bind.");
	}

	cout << "SERVER: Socket is ready.\n";
}

ServerSocket::~ServerSocket()
{
	cout << "SERVER: Closing the socket.";
	close(socketFD);
}

string ServerSocket::serverListen() throw (const string)
{
	cout << "SERVER: Reading...\n";
	//Clear the buffer	
	bzero(buffer, BUFFER_LENGTH);

	//Read from the socket to the buffer
	n = read(newSocketFD, buffer, BUFFER_LENGTH - 1);

	if (n < 0)
		throw string("Error: failed to read from socket.");

	cout << "SERVER: Message received " << string(buffer) << endl;

	return string(buffer);
}

void ServerSocket::serverRespond(const string message) throw (const string)
{
	cout << "SERVER: responding with: '" << message << "'\n";
	//Respond to the client
	n = write(newSocketFD, message.c_str(), message.length());

	if (n < 0)
		throw string("ERROR: failed to write to socket.");

	cout << "SERVER: Message sent." << endl;
}

void ServerSocket::serverConnect() throw (const string)
{
	cout << "SERVER: Listening...\n";
	listen(socketFD, 5);

	clientLength = sizeof(clientAddress);

	newSocketFD= accept(socketFD, (struct sockaddr*) &clientAddress, &clientLength);

	if (newSocketFD < 0)
	{
		throw string("Error: failed to accept client.");
	}

	cout << "SERVER: Accepted " << newSocketFD << endl;	
}

void ServerSocket::serverDisconnect() throw (const string)
{
	cout << "SERVER: Disconnecting " << newSocketFD << endl;
	close(newSocketFD);
}


int main()
{
	GameHost gh;
	try
	{
		ServerSocket server(6789);

		for (;;)
		{
			server.serverConnect();
			string message = server.serverListen();

			// message.erase((message.begin(), message.end(), '\n'), message.end());

			if (message[message.length() - 1] == '\n')
			{
				cout << "NEWLINE\n";
			}

			cout << "SERVER: Message received: '" << message << "'\n";
			server.serverRespond("I received your message.");			
		}

		// server.serverConnect();

		server.serverDisconnect();
	}
	catch (string ex)
	{
		cout << ex << endl;
	}

}