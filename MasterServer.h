#ifndef MASTERSERVER_H
#define MASTERSERVER_H

#include <vector>
#include <string>
#include <map>
#include "PracticalSocket.h"

using namespace std;

class MasterServer {
private:
    // private instance variable representing server port to connect to
    static const unsigned short SERVER_PORT = 4444;
    //each client computer
    struct client{
        TCPSocket* socket;
        bool alive;
        std::map< int, std::string > children;
    };

    //list of clients
    vector<client> clients;

    int numClients = 0;

public:
    // constructor
    // constructs server instance
    MasterServer();

    // destructor
    // destroys server instance
    ~MasterServer();

    //heartbeat function
    bool testConnections();

    // start
    // start function that begins a server's TCP connection, opening for clients
    void start();
};

#endif // SERVER_H