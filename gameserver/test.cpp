#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "server.h"
#include <string>
#include <iostream>
using namespace std;

//Globals are bad, so I made it a pointer to a local variable :-)
ServerSocket* serverPtr;

void ctrlc_handler(int junk)
{
	if (serverPtr != NULL)
	{
		// serverPtr->serverDisconnect();
		try
		{
			delete serverPtr;
			cout << "\nServer successfully deleted after CTRL-C catch.\n";
		}
		catch (string ex)
		{}
	}
}

int main()
{
	ServerSocket *server = NULL;
	serverPtr = server;

	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = ctrlc_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);

	server = new ServerSocket(6789);
	serverPtr = server;

	for(;;)
	{
		try
		{
			server->serverConnect();			
			string reply = server->serverListen();

			if (reply == "DISCONNECT")
			{
				cout << "SERVER: Client requesting to disconnect.";	
				server->serverRespond("DISCONNECTED");
				server->serverDisconnect();
				continue;
				// delete server;
				// return 0;
			}

			cout << "The client says: " << reply << endl;
			
			server->serverRespond("I received your message: " + reply);
		}
		catch (string ex)
		{
			cout << ex << endl;
			delete server;
			return 1;
		}
	}

	return 0;
}