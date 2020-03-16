#include "HelloMultiplexer.hpp"
#include <iostream>
#include <thread>
#include <stdlib.h>

const unsigned DEFAULT_SERVER_PORT = 8000;

int main(int argc, char *argv[])
{
    // spawn the server
    std::cout << "spawning server\n";
    HelloMultiplexer server(DEFAULT_SERVER_PORT);
    std::thread t(&HelloMultiplexer::run, server);
    std::cout << "spawned server\n";
    t.join(); // wait for server to finish
    return 0;
}