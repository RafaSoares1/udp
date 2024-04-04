#pragma once

namespace bisect::reactor
{
    class handler_t
    {
      public:
        virtual ~handler_t() = default;

        /// @brief Accessor to the file descriptor.
        /// @return the file descriptor.
        virtual int get_fd() noexcept = 0;

        /// @brief Will be called when the file descriptor is ready for reading.
        virtual void handle_read() noexcept = 0;
    };
} // namespace bisect::reactor
