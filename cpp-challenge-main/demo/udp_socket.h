#pragma once

#include "../lib/include/bisect/reactor/handler.h"
#include <cinttypes>
#include <sys/types.h>
#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <errno.h>

namespace bisect::demo
{
    class udp_socket_t : public reactor::handler_t
    {
      public:
        //Factory Method
        static std::unique_ptr<udp_socket_t> create(uint16_t port);
        /// NOTE:
        /// A FEW THINGS ARE MISSING HERE
        
        /// @brief Destructor to close socket, avoid resource leaks.
        ~udp_socket_t();
        
        /// @brief send data to a specific address
        /// @param data pointer to data do be sent
        /// @param length data in bytes
        /// @param dest_addr destination address and port
        /// @note No error handling, so there is no feedback if construction fails.
        ssize_t send_to(const void* data, size_t length, const sockaddr_in& dest_addr);

        /// @brief Checks if the socket is valid.
        /// @return true if the socket is valid, false otherwise.
        bool is_valid() const noexcept;

        int get_fd() noexcept override;
        

        void handle_read() noexcept override;

      private:
        /// @brief the socket's file descriptor.
        const int fd_;

        /// @brief Creates a sockets that listens on all interfaces and binds to `port`.
        /// @param port the port to bind to.
        /// @note No error handling, so there is no feedback if construction fails.
        udp_socket_t(uint16_t port);
    };
} // namespace bisect::demo
