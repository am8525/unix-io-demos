#ifndef HELLOMULTIPLEXER_HPP
#define HELLOMULTIPLEXER_HPP

#include <vector>
#include <stdlib.h>

/**
 * listens for, receives, and echos messages on a set of file descriptors.
 */
class HelloMultiplexer
{
    const uint16_t MAX_CLIENT_BACKLOG = 5;

public:
    HelloMultiplexer(uint16_t p) : received_client(false), server_port(p), clients_left(0), sockets()
    {
    }
    void run();

private:
    bool received_client;
    uint16_t server_port;
    uint16_t clients_left;
    std::vector<int> sockets; // first will be accepting fd, rest will handle client msgs.
};
#endif // HELLOMULTIPLEXER_HPP