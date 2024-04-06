#include "../include/bisect/reactor/reactor.h"
using namespace bisect::reactor;

reactor_t::reactor_t() : stop_signal(false)
{
    reactor_thread = std::thread(&reactor_t::monitor_handlers, this);
}

reactor_t::~reactor_t() {
    // Signal the monitoring thread to stop and join it
    stop_signal = true;
    if (reactor_thread.joinable()) {
        reactor_thread.join();
    }
}

void reactor_t::register_handler(std::shared_ptr<handler_t> handler) {
    std::lock_guard<std::mutex> guard(handlers_mutex);
    handlers[handler->get_fd()] = handler;
}

void reactor_t::unregister_handler(int fd) {
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

        // Acquire lock to safely copy the handlers
        {
            std::lock_guard<std::mutex> guard(handlers_mutex);
            handlers_copy = handlers;
        }

        // Prepare the set of file descriptors
        for (const auto& [fd, handler] : handlers_copy) {
            FD_SET(fd, &read_fds);
            if (fd > max_fd) {
                max_fd = fd;
            }
        }

        // Timeout structure (optional, here for demonstration, results in a blocking select)
        struct timeval timeout;
        timeout.tv_sec = 1; // 1 second timeout
        timeout.tv_usec = 0; // 0 milliseconds

        // The call to select() will block until there is some activity on one of the file descriptors in the set, or until timeout
        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);

        if (activity < 0 && errno != EINTR) {
            std::cerr << "select error" << std::endl;
        }

        // Now, check which file descriptors are ready
        for (const auto& [fd, handler] : handlers_copy) {
            if (FD_ISSET(fd, &read_fds)) {
                // If FD_ISSET returns true, the fd is ready for reading
                handler->handle_read();
            }
        }
        
        // No need to sleep here since select is inherently blocking until there's activity
        // or the timeout is reached.
    }
}