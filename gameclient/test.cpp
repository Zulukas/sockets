#include "client.h"

#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    string address = "127.0.0.1";
    int portNumber = 6789;

    if (argc >= 3)
    {
        address = argv[1];
        portNumber = atoi(argv[2]);
    }

    ClientSocket* mySocket;

    for(;;)
    {
    	try
    	{
    		mySocket = new ClientSocket(address, portNumber);
    		// mySocket.clientConnect();
	    	string message;

	    	cout << "Enter a message: ";
	    	getline(cin, message);

	    	mySocket->writeToServer(message);
	    	message = mySocket->readFromServer();
	    	mySocket->clientDisconnect();

	    	cout << "Server response: " << message << endl;

	    	delete mySocket;

	    	if (message == "DISCONNECTED")
	    	{
	    		return 0;
	    	}
    	}
    	catch (string ex)
    	{
    		cout << ex << endl;
    		return 1;
    	}
    }

    // mySocket.writeToServer(message);
    // mySocket.readFromServer();
    return 0;
}