#include "../include/bisect/reactor/reactor.h"
using namespace bisect::reactor;

reactor_t::reactor_t() : stop_signal(false)
{
    reactor_thread = std::thread(&reactor_t::monitor_handlers, this);
}

reactor_t::~reactor_t()
{
    // Signal the monitoring thread to stop and join it
    stop_signal = true;
    if (reactor_thread.joinable()) {
        reactor_thread.join();
    }
}

void reactor_t::register_handler(std::shared_ptr<handler_t> handler)
{
    std::lock_guard<std::mutex> guard(handlers_mutex);
    handlers[handler->get_fd()] = handler;
}

void reactor_t::unregister_handler(int fd)
{
    std::lock_guard<std::mutex> guard(handlers_mutex);
	printf("\nFD: %d disconected. \n", fd);
    handlers.erase(fd);
}

void reactor_t::monitor_handlers()
{
    while (!stop_signal) {
        fd_set read_fds;
        FD_ZERO(&read_fds);

        int max_fd = 0;

        std::unordered_map<int, std::shared_ptr<handler_t>> handlers_copy;

        //lock to safely copy the handlers
        {
            std::lock_guard<std::mutex> guard(handlers_mutex);
            handlers_copy = handlers;
        }

        for (const auto& [fd, handler] : handlers_copy)
        {
            FD_SET(fd, &read_fds);
            if (fd > max_fd) {
                max_fd = fd; // find max_fd is specific to the select(). first parameter of select requires the highest
                            // numbered FD. +1 to ensure that all FDs between 0 and max_fd inclusive are monitored
            }
        }
        // Timeout structure to set up timeout period for the select()
        struct timeval timeout;
        timeout.tv_sec = 1; // 1 second timeout
        timeout.tv_usec = 0; // 0 milliseconds (no milliseconds added)

        // The call to select() will block until there is some activity on one of the file descriptors in the set, or until timeout
        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);

        //std::cout << "Activity: " << activity << std::endl;

        if (activity < 0 && errno != EINTR)
            std::cerr << "select error" << std::endl;

        //what file descriptors is ready
        for (const auto& [fd, handler] : handlers_copy)
        {
            if (FD_ISSET(fd, &read_fds))
            {
                //std::cout << "FD: " << fd << "will enter the handle_read." << std::endl;
                // If FD_ISSET returns true, the fd is ready for reading
                handler->handle_read();
            }
        }
    }
}