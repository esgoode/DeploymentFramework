[![Build Status](https://travis-ci.com/CS3281-vu/spring2017-assignment-3-cs281-test.svg?token=pyUEeBLkG7FqiYPhyfxp&branch=master)](https://travis-ci.com/CS3281-vu/spring2017-assignment-3-cs281-test)

# FINISHED (EXTENSION UNTIL MAY 4th)

This project's goal was to build a deployment framework that had a master node which communicated with and sent commands to a number of client nodes. We were succesful in building this project.

# How To Run

To run this project:


    1. cmake CMakeLists.txt
    
    2. make
    
    3. 2 executable files (cShell and mShell) will now be in the bin directory
    
    4. Move the client executable cShell to another node or part of workstation
    
    5. execute mShell (this will start master node)
    
    6. execute cShell (note mShell will not start until cShell connects)
    
    
Note: You need to set up propper ssh between locations of cShell and mShell. This was described in piazza. This allows the copy command.

# How To Test 

These are by no means the only way to test these two programs, but this is one way to test.
  1. Copy cShell into a number of places
  2. Run mShell (note when the first client connects it will ask you to enter a command, to connect another client before entering another command simply type exit
  3. Now the mShell will prompt for the user to enter commands
  4. Execute each command in a variety of ways testing that they work.
  
    a. Welcome to deployment project. The commands are as follows"
    
    b.  copy = 'copy' followed by desired file"
    
    c.  list clients \t= 'list'
    
    d.  list PIDs = 'pid'
    
    e.  SIGKILL = 'sKill' followed by desired process
    
    f.  SIGINT = 'kill' followed by desired process
    
    g.  exit = 'exit' (type exit to add additional clients)
    
    h.  execute = './' followed by filename
    
    i.  remove = 'rm' followed by filename
    
    
For Further Details Please See Further Documentation
  

