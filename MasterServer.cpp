#include "MasterServer.h"
#include "app.h"
#include "PracticalSocket.h"
#include "utilities.h"
#include <iostream>
#include <system_error>
#include <unistd.h>
#include <string>
#include <list>
#include <cstring>

// constructor
// constructs server instance
MasterServer::MasterServer() {
    std::cerr << "===============================================" << std::endl;
    std::cerr << "Welcome to the Deployment M Node " << std::endl;
    std::cerr << "===============================================" << std::endl;
}

// destructor
// destroys server instance
MasterServer::~MasterServer() {
    std::cerr << "===============================================" << std::endl;
    std::cerr << "Closing M Node " << std::endl;
    std::cerr << "===============================================" << std::endl;
}

//testConnections()
//servs the heartbeat function, that sends test and receives ok from each node
//if client doesn't respond twice in a row, marked dead.
bool MasterServer::testConnections(){
    char buff[4];

    for (int i = 0; i < numClients; i++){
        client c = clients[i];
        if(clients[i].alive) {
            buff[0] = 't';
            buff[1] = 'e';
            buff[2] = 's';
            buff[3] = 't';
            c.socket->send(buff, sizeof(buff));
            c.socket->recv(buff, sizeof(buff));
            clients[i].alive = buff[0] == 'o' && buff[1] == 'k';
        } else {
            std::cout << "Client " << i + 1 << ": Disconnected" << endl;
        }
    }

}


// start
// start function that begins a server's TCP connection, opening for clients
void MasterServer::start() {
    try {
        // Open the connection to listen for clients
        TCPServerSocket server_socket(SERVER_PORT);

        while (true) {
            // accept client connections
            std::cout << "Server is waiting for a client to connect." << std::endl;
            TCPSocket *client_socket = server_socket.accept();

            //push new client to vector clients
            client c;
            c.socket = client_socket;
            c.alive = true;
            clients.push_back(c);
            numClients++;

            std::cout << "Client " << numClients << " has connected." << std::endl;
            testConnections();

            std::string command = "";

            //buffer for sockets
            char buffer[2048];


            while(command != "exit") {
                //testClients to send and receive heartbeat
                testConnections();

                //get user input
                std::cout << "Enter command (h for help): ";

                getline(std::cin, command);

                std::vector<std::string> commandV;

                tokenize_string(command, commandV, " ");

                for(int cNum = 0; cNum < numClients; cNum++) {

                    if(clients[cNum].alive) {
                        //copy designated executable to worker
                        //we need to get directory of client
                        if (commandV[0] == "copy") {

                            //get directory of worker node fix parse error
                            std::string pwd = "pwd";
                            std::vector <std::string> adjDir;

                            strncpy(buffer, pwd.c_str(), sizeof(buffer));
                            buffer[sizeof(buffer) - 1] = 0;


                            clients[cNum].socket->send(buffer, sizeof(buffer));     //request current directory of client
                            clients[cNum].socket->recv(buffer, sizeof(buffer));

                            std::string directory(buffer);

                            //reset buffer
                            memset(buffer, 0, sizeof(buffer));

                            //get current directory and setup scp call
                            getcwd(buffer, sizeof(buffer));
                            std::string cdirectory(buffer);
                            std::string copy = "scp " + cdirectory + "/" + commandV[1] + " vagrant@localhost:" + directory;

                            //cal scp
                            system(copy.c_str());
                        } else if (commandV[0] == "list") {

                                //if client is alive let user know
                                if (clients[cNum].alive) {
                                    cout << "Client " << std::to_string(cNum + 1) << " is available" << endl;
                                }

                        } else if (commandV[0] == "pid"){
                            map<int, std::string>::iterator it;

                            //for each process check with client and update status
                            for ( it = clients[cNum].children.begin(); it != clients[cNum].children.end(); it++ )
                            {
                                if(it->second == " : running\n" || it->second == ": running\n") {
                                    std::string pid = std::to_string(it->first);  // string (key)
                                    std::string send = "pid " + pid;

                                    //set buffer
                                    strncpy(buffer, send.c_str(), sizeof(buffer));
                                    buffer[sizeof(buffer) - 1] = 0;

                                    clients[cNum].socket->send(buffer, sizeof(buffer));

                                    memset(buffer, 0, sizeof(buffer));

                                    clients[cNum].socket->recv(buffer, sizeof(buffer));
                                    std::string status(buffer);

                                    it->second = status;
                                }

                                //output status
                                std::cout << std::to_string(it->first) << it->second;
                            }
                        } else if(commandV[0] == "kill" || commandV[0] == "sKill"){
                            //adjust buffer
                            int pid = atoi(commandV[1].c_str());

                            //pid is in this client, kill it
                            if(clients[cNum].children.count(pid) == 1) {
                                strncpy(buffer, command.c_str(), sizeof(buffer));
                                buffer[sizeof(buffer) - 1] = 0;

                                clients[cNum].socket->send(buffer, sizeof(buffer));
                            }
                        } else if(commandV[0] == "usage"){
                            strncpy(buffer, command.c_str(), sizeof(buffer));
                            buffer[sizeof(buffer) - 1] = 0;

                            clients[cNum].socket->send(buffer, sizeof(buffer));

                            memset(buffer, 0, sizeof(buffer));

                            clients[cNum].socket->recv(buffer, sizeof(buffer));
                            std::string status(buffer);
                            std::cout << "Client " + std::to_string(cNum + 1) + " " + status;

                        } else if (commandV[0] == "exit"){
                            //do nothing
                        } else if(commandV[0] == "h"){
                            std::cout << "Welcome to deployment project. The commands are as follows" << endl;
                            std::cout << "copy \t\t\t= 'copy' followed by desired file" << endl;
                            std::cout << "list clients \t= 'list'" << endl;
                            std::cout << "list PIDs \t\t= 'pid'" << endl;
                            std::cout << "SIGKILL \t\t= 'sKill' followed by desired process" << endl;
                            std::cout << "SIGINT \t\t\t= 'kill' followed by desired process" << endl;
                            std::cout << "exit \t\t\t= 'exit' (type exit to add additional clients)" << endl;
                            std::cout << "execute \t\t= './' followed by filename" << endl;
                            std::cout << "remove \t\t\t= 'rm' followed by filename" << endl;


                        } else {
                            strncpy(buffer, command.c_str(), sizeof(buffer));
                            buffer[sizeof(buffer) - 1] = 0;

                            clients[cNum].socket->send(buffer, sizeof(buffer));

                            memset(buffer, 0, sizeof(buffer));

                            clients[cNum].socket->recv(buffer, sizeof(buffer));
                            std::string status(buffer);
                            std::cout << status;

                            //map to status
                            int pid = atoi(status.substr(0, 5).c_str());
                            clients[cNum].children[pid] = status.substr(5);
                        }

                        memset(buffer, 0, sizeof(buffer));

                    }
                }


            }


        }
    } catch (SocketException) {
        std::cout << "Unable to start server. Press \"Enter\" to exit " << endl;
        std::string done = "";
        getline(std::cin, done);
        exit(0);
    }
}


