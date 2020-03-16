# Demo 1: The hello multiplexer.

This demo employs the UNIX select() function to concurrently handle read operations 
on a set of file descriptors. 

The main file descriptor (i.e. sockets[0]) listens for and accepts new connections, whereas
all other file descriptors receive messages from connected clients.

to run this demo on Mac OSX or Linux, run the following commands in terminal #1:\n
g++ -o server.out ServerMain.cpp HelloMultiplexer.cpp -std=c++11 \n
./server.out

and these commands in terminal #2:\n
g++ -o client.out ClientMain.cpp HelloClient.cpp -std=c++11 \n
./client.out
