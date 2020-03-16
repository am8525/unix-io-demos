#include "HelloClient.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <stdlib.h>

const unsigned DEFAULT_CLIENT_COUNT = 3;
const unsigned DEFAULT_SERVER_PORT = 8000;

int main(int argc, char *argv[])
{
    // spawn the clients
    std::vector<std::thread> client_pool;
    uint16_t ms_total = DEFAULT_CLIENT_COUNT * 3000;
    for (int i = 1; i <= DEFAULT_CLIENT_COUNT; ++i)
    {
        uint16_t ms_cooldown = 1000 * i + 1000;
        uint16_t id = i;
        std::cout << "spawning client " << i << "\n";
        HelloClient client(id, ms_total, ms_cooldown, DEFAULT_SERVER_PORT);
        std::thread t(&HelloClient::run, client);
        client_pool.push_back(std::move(t));
        std::cout << "spawned client " << i << "\n";
    }

    for (int i = 0; i < DEFAULT_CLIENT_COUNT; ++i)
    { // wait for client threads to finish
        client_pool[i].join();
    }

    return 0;
}