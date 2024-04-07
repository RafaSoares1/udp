#include "udp_socket.h"

using namespace bisect::demo;
using namespace bisect::reactor;

namespace
{
    int create_udp_socket()
    {
        return socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    }

    int bind_to_port(int fd, uint16_t port)
    {
        sockaddr_in addr{};
        addr.sin_family      = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port        = htons(port);

        if(bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        {
            return -1;
        }

        return 0;
    }

    // Opens the socket and returns the fd.
    int open_socket(uint16_t port)
    {
        const auto fd = create_udp_socket();
        if(fd < 0) return fd;

        if(bind_to_port(fd, port) < 0) return -1;

        if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == -1) // set socket to non-blocking mode, first step to achieve an asynchronously connection
        {
            return -1;
        }

        return fd;
    }
} // namespace


std::unique_ptr<udp_socket_t> udp_socket_t::create(uint16_t port)
{
    auto instance = std::unique_ptr<udp_socket_t>(new udp_socket_t(port));
        if (!instance->is_valid()) {
            // Construction failed, return nullptr.
            return nullptr;
        }
        return instance;
}

udp_socket_t::udp_socket_t(uint16_t port) : fd_(open_socket(port))
{
}

udp_socket_t::~udp_socket_t()
{
    if(fd_ != -1)
        close(fd_);
}
bool udp_socket_t::is_valid() const noexcept
{
    return fd_ != -1;
}

int udp_socket_t::get_fd() noexcept
{
    return fd_;
}

void udp_socket_t::handle_read() noexcept
{
    printf("socket %d is ready for read\n", fd_);

    char buffer[1500];
    for(;;)
    {
        const auto r = read(fd_, buffer, sizeof(buffer) - 1);
        if(r < 0)
        {
            if(errno != EWOULDBLOCK)
            {
                printf("error reading from socket: %d\n", errno);
            }
            return;
        }
        buffer[r] = '\0';
        printf("read %ld bytes from %d\n", r, fd_);
        printf("Buffer after handle read is: %s\n", buffer);
    }
}

ssize_t udp_socket_t::send_to(const void* data, size_t length, const sockaddr_in& dest_addr)
{
    if(fd_ == -1)
        return -1;
    
    ssize_t sentBytes = sendto(fd_, data, length, 0, reinterpret_cast<const sockaddr*>(&dest_addr), sizeof(dest_addr));

    if(sentBytes == -1)
        return -1;
    return sentBytes;
}
