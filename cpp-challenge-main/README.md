# C++ Challenge

## 1. UDP socket

Consider the class [`udp_socket_t`](demo/udp_socket.h).

**1.1**

There is a comment that implies that the interface and the implementation is not complete. What is missing there?

**1.2**

C++ constructors don't allow returning errors. How would you avoid having to check that the socket is valid using the `udp_socket_t::is_valid()` function and rather signal immediately that the construction failed, without using exceptions?

_Hint: would a static function that returns an instance of `udp_socket_t` help?_

## 2. Reactor

Consider the class [`reactor_t`](lib/include/bisect/reactor/reactor.h).

As you can see, [the implementation file](lib/src/reactor.cpp) is empty. Implement the class according to the specification in the header file.
