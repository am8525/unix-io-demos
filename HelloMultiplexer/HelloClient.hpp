#ifndef HELLOCLIENT_HPP
#define HELLOCLIENT_HPP
#include <stdlib.h>

/**
 * pesters the server with a series of simple plaintext messages.
 */
class HelloClient
{
public:
    HelloClient(uint16_t id, uint16_t total, u_int16_t cooldown, uint32_t p) : client_id(id), total_ms(total), cooldown_ms(cooldown), server_port(p)
    {
    }
    void run();

private:
    uint16_t client_id;
    uint16_t total_ms;
    uint16_t cooldown_ms;
    uint32_t server_port;
};
#endif // HELLOCLIENT_HPP