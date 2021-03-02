# BattleShip

## Description

This project is under development; it currently is fully functional but
limited to play on the same unix system as it uses Unix sockets for it's
IPC; development is now being done on converting it to working through TCP/IP
sockets, so that it can be played across separate systems.

It consists of a multithreaded server program which handles connections
and commands from multiple client programs, and can handle these client
connections simultaneously.

It can be used by different users on a shared Unix system on the terminal
to play a version of multiplayer battleship. Players take turns
firing at the next person in the chain of players like so:

A -> B -> C -> D -> A -> B -> C (etc.)

## Compiling And Testing Instructions:

**System Prerequisites**

This program is intended to be run on a Linux system.
It was developed and tested on the following:


OS: Arch Linux x86_64

Kernel: 5.10.15-arch1-1

Shell: bash 5.1.4

Thread model: posix

gcc version 10.2.0

GNU Make 4.3


**Testing Instructions**

To test, run:

   `make clean`

   `make all`

   `make test`


**Explanation of Test**

`make clean`
- This will cleanup all executable, object, etc. files generated during compilation,
  if make all and make test have already been executed previously.

`make all`
- This will compile the battleship_server and battleship_client executables and
  their dependencies.

`make test`
- This will execute ./battleship_server in the background, then call
  ./battleship_client multiple times, with a variety of command line arguments.
  For this assessment I chose to use the IPC model, and implemented this with
  unix sockets, and multithreaded my main node (./battleship_server), so that
  it is able to accept multiple client requests simultaneously. I ensure
  atomicity of operations and proper synchronization using pthread_mutex_locks.

  ./battleship_server is the master node executable which is capable of accepting
  multiple connections from clients simultaneously. ./battleship_client takes
  command line arguments that specify an operation specified by a player 
  for the server to process. The output of make test consists of the commands
  which execute the executables that were compiled by make all to
  demonstrate the functionality of this project: then, when the majority of the
  basic functionality the executables are capable of are gone through, 
  the log_file that is written to every time a client operation takes place on 
  the server node is then displayed with the cat command on the terminal,
  so the tester can see the result of the commands.

  After the basic operation of the executables are demonstrated, a SIGINT
  is sent to the master node to stop it. The master node then executes
  its interrupt routine for the SIGINT signal and the server/game is done.

  Example sequence:

  ./battleship_server &

  will bind to the unix socket ./srv_socket and listen for incoming requests
  from client sockets. When an incoming client request comes in, a new thread
  is dispatched to process the client request. This master process is thus
  capable of processing multiple client connections/requests at once.

  ./battleship_client -i LUKS -j

  will bind to the unix socket ./cli_socket_LUKS and connect with the server
  socket ./srv_socket. It represents the request from the player represented
  by the unique identifier LUKS to join the game. Because no -x or -y args
  are specified, the player LUKS will be inserted into the circular linked
  list of players and a random x,y solution will be generated for him.

  ./battleship_client -i JOHN -j -x 5 -y 9

  Represents the request from the player represented by the unique identifier
  JOHN to join the game. The optional -x and -y cmdline args are specified,
  so the player JOHN will be inserted into the circular linked list of players
  and the x,y solution he specified will be used for the location on his grid that
  if hit, will cause him to lose. Now that there are the minimum number of players (2), 
  the master node will process BOMB requests from clients. This fact is logged to the
  logfile.

  ./battleship_client -i JOHN -b -x 2 -y 10

  Represents the request from the player JOHN to bomb the person he is firing at
  (LUKS) at the coordinates x = 2 and y = 10, however he joined just after LUKS, so
  there will be a statement written to the log_file that it is not his turn, and so on.

  To call make test twice, first call make clean, then make all, then make test.

  For an example of what the output of `make test` should look like, please see
  the file make_test_output.txt.

  **If it doesn't look like this, the program hasn't been compiled/tested properly.**

   battleship_server uses the socket path "./srv_socket" for the server socket, and
   battleship_client uses the socket path "./cli_socket_[unique identifier]" for the client socket.

## Notes on Development:

   To achieve synchronization using multiple threads initialized by multiple client connections,
   which the master node is capable of handling simultaneously, I created a while loop in my main
   function directly following the listen() call which infinitely loops on a call to accept(), which
   accepts incoming requests from clients. When a client connects, a new thread is created, and that
   thread is dispatched to the function client_handler with a custom struct containing the server and
   client file handles to deal with that client's request.
   
   The game stays open, in a lobby like format where new players can join to challenge the winner of the
   last round. To change this so the server program was to be terminated on its own when an initial
   winner was declared, I would likely use a signal sent from the child to the parent process or better
   yet, semaphores so that all excess heap memory left in main() could be freed as easily as possible.

   For storing the client's unique identifiers and who they're linked with I my own circular
   doubly linked list implementation in the files linkedlist.c and linkedlist.h. While it wouldn't
   have been difficult to make it so that a joining user could specify which neighbours to be
   inserted between, the joining user is simply inserted as the "head" node position in the circular linked
   list of players.

   To ensure atomicity of operations, a mutex_lock was necessary to protect calls
   to fprintf when writing to the log_file, protect operations dealing with the global circular
   doubly linked list, file handlers from being changed by incoming threads before
   the last thread to go through could be sent to client_handler() and have its file handlers
   parsed, etc.
