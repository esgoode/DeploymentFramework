#include "ServerClient.h"
#include "PracticalSocket.h"
#include "utilities.h"
#include "app.h"

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <sys/poll.h>
#include <pthread.h>
#include <fstream>
#include <sys/time.h>
#include <sys/resource.h>

// constructor
// instantiates client
ServerClient::ServerClient() {
    std::cout << "Client created";
}

// destructor
// destroys client instance and prints shell exit message
ServerClient::~ServerClient() {
    std::cout << "Client destroyed";
}

// start
// start function that receives input to server with TCP and waits for result
void ServerClient::start() {
    try {
        //establish connection with Master Server
        TCPSocket sock("127.0.0.1", SERVER_PORT);

        app *mApp = new app();

        //buffer for communciation
        char gen_buffer[1024];

        //infinite loop to receive socket sends
        while(true) {
            // create a general buffer for receiving commands from client socket

            // create a pointer to a new app for executing commands


            // receive command from server and convert to string
            sock.recv(gen_buffer, sizeof(gen_buffer));
            std::vector<std::string> commandV;

            std::string command(gen_buffer);
            tokenize_string(command, commandV, " ");

            //sock.recv(buffer, RCVBUFSIZE);

            //respond to test
            if(gen_buffer[0] == 't' && gen_buffer[3] == 't')
                sock.send("ok", strlen("ok"));
            else if(gen_buffer[0] == 'p' && gen_buffer[1] == 'w' && gen_buffer[2] == 'd'){


                //returns current directory
                getcwd(gen_buffer, sizeof(gen_buffer));
                sock.send(gen_buffer, sizeof(gen_buffer));
            } else if(command.compare("") == 0){
                //do nothing
            } else if(gen_buffer[0] = 'p' && gen_buffer[1] == 'i' && gen_buffer[2] == 'd'){
                int pid = atoi(commandV[1].c_str());

                //check childs status whether running or exited
                string reply = getChildStatus(pid);

                //send back to master
                strncpy(gen_buffer, reply.c_str(), sizeof(gen_buffer));
                gen_buffer[sizeof(gen_buffer) - 1] = 0;
                sock.send(gen_buffer, sizeof(gen_buffer));
                
            } else if (commandV[0] == "kill"){

                //kill process with SIGINT
                int pid = atoi(commandV[1].c_str());
                kill(pid, SIGINT);
            } else if (commandV[0] == "sKill") {

                //kill process with SIGKILL
                int pid = atoi(commandV[1].c_str());
                kill(pid, SIGKILL);
            } else if (commandV[0] == "usage"){

                //struct that stores usage values
                struct rusage rstruct;
                string reply = "";

                if (getrusage(RUSAGE_CHILDREN, &rstruct) != 0) {
                    reply += "error getting usage\n";
                } else {

                    //get usage values
                    long sec_cpu = (long)rstruct.ru_utime.tv_sec;
                    long micro_cpu = rstruct.ru_utime.tv_usec * 1000000;
                    long cpu = sec_cpu + micro_cpu;
                    long memory = rstruct.ru_minflt;

                    reply += "CPU: " + std::to_string(cpu) + " Memory: " + std::to_string(memory) + "\n";
                }

                //return to masster
                strncpy(gen_buffer, reply.c_str(), sizeof(gen_buffer));
                gen_buffer[sizeof(gen_buffer) - 1] = 0;
                sock.send(gen_buffer, sizeof(gen_buffer));

            }else {

                //serves as way to execute command and remove file
                int pid = mApp->execute(commandV);                   // execute command

                string reply = std::to_string(pid) + getChildStatus(pid);


                //memset(gen_buffer, 0, sizeof(gen_buffer)); // reset buffer to empty
                strncpy(gen_buffer, reply.c_str(), sizeof(gen_buffer));
                gen_buffer[sizeof(gen_buffer) - 1] = 0;
                sock.send(gen_buffer, sizeof(gen_buffer));
            }

            memset(gen_buffer, 0, sizeof(gen_buffer)); // reset buffer to empty
        }
    } catch(SocketException &e){
        std::cout << "Client failed to connect";
        exit(0);
    }

}

//getChildStatus
//calls waitpid and gets status of process
std::string ServerClient::getChildStatus(int pid){
    int status;

    //signal handler
    int return_pid = waitpid(pid, &status, WNOHANG);

    string reply = "";

    if (return_pid <= -1) {
        /* error */
        reply += " : error\n";
    } else if (return_pid == 0) {
        /* child is still running */
        reply += " : running\n";
    } else {
        /* child is finished. exit status in   status */
        if ( WIFEXITED( status ) )
        {
            int exitcode = WEXITSTATUS( status );
            std::string msg =
                    ": Child " +
                    ( ( exitcode )
                      ? "Self Terminated With Exit Code " + std::to_string( exitcode )
                      : "Exited Successfully" ) +
                    "\n";
            reply  += " " + msg;
        }
        else if ( WIFSIGNALED( status ) )
        {
            int exitsig = WTERMSIG( status );
            reply += " : Child Terminated Due to Signal " + std::to_string(exitsig) + "\n";
        }
    }

    return reply;
}
