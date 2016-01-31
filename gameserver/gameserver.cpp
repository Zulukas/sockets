#include "gameserver.h"

GameServer::GameServer(int portNumber) : Server(portNumber) {}

/******************************************************************************
 * The GameServer loop continues accepting from clients
 *****************************************************************************/
void GameServer::loop()
{
	time_t timer;
	double seconds;


	for (;;)
	{
		try
		{
			server.serverConnect();
			string message = server.serverListen();
			string reply;

			std::transform(message.begin(), message.end(), message.begin(), ::toupper);

			//Handle the input from the client
			reply = game->handle(message);

			//Respond
			server.serverRespond(reply);
			
			//If the client requests to disconnect
			if (message == "DISCONNECT")
			{			
				server.serverDisconnect();
			}
		}
		catch (const string ex)
		{
			cout << ex << endl;
			break;
		}
	}
}

// int main()
// {
// 	GameHost gh;
// 	try
// 	{
// 		ServerSocket server(6789);

// 		for (;;)
// 		{
// 			server.serverConnect();
// 			string message = server.serverListen();


// 			// message.erase((message.begin(), message.end(), '\n'), message.end());

// 			if (message[message.length() - 1] == '\n')
// 			{
// 				cout << "NEWLINE\n";
// 			}

// 			cout << "SERVER: Message received: '" << message << "'\n";
// 			server.serverRespond("I received your message.");			
// 		}

// 		// server.serverConnect();

// 		server.serverDisconnect();
// 	}
// 	catch (string ex)
// 	{
// 		cout << ex << endl;
// 	}

// }