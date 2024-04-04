#pragma once

#include "bisect/reactor/handler.h"
#include <memory>

namespace bisect::reactor
{
    /// Runs asynchronously and notifies registered handlers when their
    /// file descriptor is ready for reading.
    class reactor_t
    {
      public:
        /// Launches a thread that monitors the file descriptors of all currently registered handlers.
        /// That thread should call `handle_read` on all the handlers whose fd are ready for read.
        reactor_t();
        ~reactor_t();

        /// @brief Registers a new handler. New handlers may be registered at any time, concurrently,
        /// and should be monitored immediately.
        /// @param handler the abstract owner of the file descriptor.
        void register_handler(std::shared_ptr<handler_t> handler);

        /// @brief Unegisters an handler. This may be called at any time, concurrently,
        /// and the file descriptor should be removed from the monitored set before the function returns.
        void unregister_handler(int fd);
    };
} // namespace bisect::reactor
