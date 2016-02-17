#include "client.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main(int argc, char *argv[])
{
    string address = "127.0.0.1";   //Default server location set to localhost
    int portNumber = 6789;          //on port 6789

    if (argc >= 3)                  //Given command line arguments, use the provided info.
    {
        address = argv[1];          //This plays nicely with the definitions in /etc/hosts
        portNumber = atoi(argv[2]);
    }

    ClientSocket mySocket(address, portNumber); //Establish the socket
    // cout << mySocket.readFromServer();

    bool timeToMakeAction = true;               //Simple state indicator    

    for(;;)     //FORever loop; get the pun? Haha.
    {
    	try
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

	    	// if (message == "DISCONNECTED")
	    	// {
	    	// 	return 0;
	    	// }
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