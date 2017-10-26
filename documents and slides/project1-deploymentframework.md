# Project 1: Deployment and Orchestration Infrastructure [to be done in a team of two]

A deployment and orchestration infrastructure is responsible for application installation and lifecycle management of the apps on a collection of remote nodes. Additionally, they provide the capability to shutdown the applications, collect execution logs and remove the installed binaries if requested. Furthermore, they provide the capability to audit performance logs for the installed and running applications. [Apache Brooklyn](https://brooklyn.apache.org/), and [Kubernetes](https://kubernetes.io/) are examples of two modern orchestration frameworks.

In this project you are going to create a choregraphed version of these frameworks. By choreographed we mean that a single master process running on the `master` computer will be responsible for controlling the orchestration process. This master process will communicate with a number of `sub-ordinate` processes that will run on `worker` computers. This communication can be implemented by any remote process communication procedure - for example, sockets, or JAVA RMI or zero MQ client server. In this class we are going to provide you examples of both socket based communication and JAVA Remote Method Invocation (RMI) method, so you can choose to use them in your project.

*Note* - Typical deployment and orchestration frameworks can install and configure multi-process applications, where the success of orchestration requires all  processes to successfully launch to consider that the application has been installed successfully. if there is any error then the whole application is torn down even if some of the  processes installed correctly. However, in this project you do not have to support this functionality i.e. you will install each app as a sigle process unit.

Once the communication lines between master and worker nodes have been established, a master will have to support a number of commands:

- copy the executable file and any  libraries required for the executable file to the worker nodes, where the process has to start.
- delete the executable file and any libraries described in the command. Note that any system library like pthread and libc will not be copied. Only new developer library or any third party library will have to be copied.
- launch the executable on the remote worker node and return the status back to the master node. The log of the `executable` launched on the worker node should be tracked in an app-specific file. This will require you to use the `dup2` command that we will learn about in the class. Launching the application will require you to `fork` and `exec`. 
- Query the list of launched PIDs and their status - whether they are running, sleeping or dead on demand. If the applicaition is dead, you need to provide the reason why the application terminated
- shutdown the application on a remote worker node gracefully. This will require you to generate a `SIGINT` signal for the worker process.
- List the available remote worker nodes.
- shutdown the application on a remote worker node forcefully. This will require you to generate a `SIGKILL` signal for the worker process.
- Ask about the resource usage of all children on a worker node. The resource used that should be tracked are: CPU, and mximum memory usage (resident set). 

## *Hints* 

For this project you will have to write the two programs, master deployment manager and the worker deployment manager. The master deployment manager must provide a shell interface on which you can enter the different commands listed above. You can use your assignment 4  interface for this. The worker deployment manager will need to use the SIGCHLD handler to keep track of the child processes. You did that in assignment 4 as well.  You will need to use the getrusage API for the resource metrics.

## Design specification

- List down the exact commands and their arguments that will be used to interact with the master deployment sever.
- Also describe in english what each command will do and briefly describe how that functionality will be implemented in the worker deployment manager
- For sockets, describe if you will use TCP or UDP. Are you going to use Practical Sockets Class?
- Describe what are the error conditions, how will you know if errors have occured

### Specific points to consider

In the design specification you should consider clarifying the following points.
   - How does the master deployment manager receive the commands to be executed. You can use the assignment 3 's model solution to open a shell from where you read the commands.
   - How does the master deployment manager communicate with the worker deployment managers. Specifically, how does it know when the worker deployment managers are not reachable. 
     - you can open up a TCP socket connection (to be covered in the class on 4/4/) where each worker deployment manager connect to the master. Over this connection the worker should send a periodic "heartbeat" message. you can assume that the worker deployment manager (running on the remote worker node) is not usable if 2 concescutive heartbeat messages are lost. 
     - Thus at any given time you will have a list of available worker nodes in the master deployment manager. This list can be provided in reponse to the command to get the list of available remote worker nodes.
     - you can copy the executable file of the socket to the remote node, if it is not already there. Therefore, given an executable you will have to check if it is present on the remote machine.
     - Then you can implement other commands as required.




# Project Report

In your project report, describe the following:

1. The design of your deployment framework (review the design specification)
2. The design of the user interaction, including the format of the commands a user can specify and what they do.
3. How the deployment framework works, what is the sequence of interaction. How can the user instruct to launch a particular process on a particular node.
4. Describe how you tested your project.
5. Describe how you have implemented more advanced functionalities like resource usage and process state queries
6. Explain how have you implemented code to shutdown the processes on the remote notes forcefully.
7. Explain the important of dup2 and how you used to it to capture the output and error streams of launched processes into a log file and how can you transfer the log file back to the user upon request.
8. explain what security features will you consider implementing if you had more time (note that security features are not in the scope of this project).

Also include a short, 5 slide presentation describing:

1. The high level idea of the project.
2. The overall structure.
3. The challenges.
4. What you learned.
5. The work distribution between you and your partner.
