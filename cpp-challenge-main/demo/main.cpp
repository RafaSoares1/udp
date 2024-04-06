#include <memory>
#include <iostream>
#include "udp_socket.h"
#include "../lib/include/bisect/reactor/handler.h"
#include "../lib/include/bisect/reactor/reactor.h"
using namespace bisect::demo;
using namespace bisect::reactor;

namespace
{
    void wait_for_key(const char* message)
    {
        printf("\n%s\n", message);
        char c;
        std::cin >> c;
    }
} // namespace

int main(int /* argc */, char** /* argv */)
{
    auto s1 = std::make_shared<udp_socket_t>(5000);
    if(!s1->is_valid()) return 1;

    auto s2 = std::make_shared<udp_socket_t>(6000);
    if(!s2->is_valid()) return 1;


    reactor_t reactor;

    reactor.register_handler(s1);
    reactor.register_handler(s2);

    wait_for_key("Press a key and enter to unregister one of the sockets");
    reactor.unregister_handler(s2->get_fd());

    wait_for_key("Press a key and enter to exit");
    return 0;
}
