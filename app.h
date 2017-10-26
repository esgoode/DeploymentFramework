#ifndef APP_H
#define APP_H

#include <iostream>
#include <sched.h>
#include <vector>
#include <string>
#include <map>

class app
{
public:
    app();
    ~app();
    void start();
    static const std::vector< std::string > builtincmds;
    int parallel_execution( std::string command_string );
    int execute( std::vector< std::string >& str );

private:
    // private methods
    

    int executebuiltin( std::vector< std::string >& command );

    // private fields
    int virtual_memory_limit;
};

#endif // APP_H
