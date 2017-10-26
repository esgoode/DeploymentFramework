#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include <string>

class ServerClient {
private:
    // private instance variable representing server port to connect to
    static const unsigned short SERVER_PORT = 4444;
    const int RCVBUFSIZE = 2048;

    static void signal_handler(int sig);
    std::string getChildStatus(int pid);

public:
    // constructor
    // instantiates client by instantiating constants and TCPSocket, connecting
    // the socket; prints shell welcome message
    ServerClient();

    // destructor
    // destroys client instance and prints exit message to shell
    ~ServerClient();

    // start
    // start function that sends input to server with TCP and waits for result
    void start();
};

#endif // CLIENT_H