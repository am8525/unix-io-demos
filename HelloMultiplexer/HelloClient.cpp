#include "HelloClient.hpp"
#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void HelloClient::run()
{
    printf("here\n");
    int fd = 0;
    //create a socket
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Client error creating socket, returning.\n");
        return;
    }

    // setup server info
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    //connect to the server
    printf("Client %i connecting...\n", client_id);
    if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Connection to server failed, returning.\n");
        perror("");
        return;
    }
    printf("Client %i connected.\n", client_id);

    //send the looped hello message
    int count = total_ms / cooldown_ms;
    char msg[100];
    sprintf(msg, "Hello from client %u.\n", client_id);
    for (int i = 0; i < count; ++i)
    {
        printf("client %i: sending message to server...\n", client_id);
        send(fd, msg, strlen(msg), 0);
        printf("client %i: msg sent.\n", client_id);
        std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms));
    }

    sprintf(msg, "Goodbye from client %u.\n", client_id);
    printf("client %i: saying goodbye to server...\n", client_id);
    send(fd, msg, strlen(msg), 0);
    printf("client %i: goodbye sent.\n", client_id);
    //interaction done
}