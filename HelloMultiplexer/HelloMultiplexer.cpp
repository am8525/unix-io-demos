#include "HelloMultiplexer.hpp"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/select.h>

void HelloMultiplexer::run()
{
    //create socket
    int fd = 0;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Server error creating socket, quitting.\n");
        exit(-1);
    }

    //setup address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port = htons(server_port);

    //bind socket to address
    if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Server error binding socket, quitting.\n");
        exit(-1);
    }
    int client_fd = 0;
    struct sockaddr_in client_addr;
    listen(fd, MAX_CLIENT_BACKLOG);
    socklen_t addrlen = sizeof(client_addr);
    client_fd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
    if (client_fd < 0)
    {
        printf("Error accepting first client, quitting.\n");
        perror("");
        exit(-1);
    }
    sockets.push_back(fd);
    sockets.push_back(client_fd);
    clients_left++;
    printf("Client %i connected.\n", clients_left);
    fd_set rfds;
    char msg_buffer[256];
    memset(&msg_buffer, 0, 256);
    while (clients_left > 0)
    { // begin the select() loop.
        /**
         * First fd will listen (non-blocking),
         * while the rest will simply receive 
         * client messages.
         * 
         * if the activity is on the first fd,
         * must accept and create a new fd,
         * incr client count and push_back(client_fd).
         * 
         * else: if the message is "Hello", simply echo.
         * otherwise, remove the fd from our vector set
         * and decrement clients_left.
         */
        fcntl(sockets[0], F_SETFL, O_NONBLOCK);
        listen(sockets[0], MAX_CLIENT_BACKLOG);
        int ret = 0;
        FD_ZERO(&rfds);
        for (auto fd : sockets)
            FD_SET(fd, &rfds);
        if ((ret = select(FD_SETSIZE, &rfds, nullptr, nullptr, nullptr)) < 0)
        {
            printf("Server error on select(), quitting.\n");
            exit(-1);
        }
        printf("select returned %i\n", ret);
        for (int i = 0; i < sockets.size(); ++i)
        {
            if (FD_ISSET(sockets[i], &rfds))
            {
                FD_CLR(sockets[i], &rfds);
                if (i == 0)
                { // new connect request
                    client_fd = accept(sockets[i], (struct sockaddr *)&client_addr, &addrlen);
                    if (client_fd < 0)
                    {
                        printf("Error accepting client, quitting.\n");
                        perror("");
                        exit(-1);
                    }
                    sockets.push_back(client_fd);
                    clients_left++;
                    printf("Client %i connected.\n", clients_left);
                }
                else
                { // incoming msg
                    memset(&msg_buffer, 0, 256);
                    if (read(sockets[i], msg_buffer, sizeof(msg_buffer)) < 0)
                    {
                        printf("Server error reading msg, quitting.\n");
                        exit(-1);
                    }
                    //if it is a Goodbye message, decr clients_left & remove socket.
                    printf("%s", msg_buffer);
                    if (msg_buffer[0] == 'G')
                    {
                        clients_left--;
                        sockets.erase(sockets.begin() + i);
                    }
                }
            }
        } // end select() for loop
    }     // end while loop
}