# Overview

The main goal of this exercise was to create a UDP server.<br />
**UDP(User Datagram Protocol)** is a communication protocol used across the Internet for especially time-sensitive transmissions like live-streams, video calls and gaming, where speed is more critical than accuracy.

## 1- UDP Socket

The udp_socket_t class defined has the functionality for creating and receiving data over a UDP socket.

### 1.1 There is a comment that implies that the interface and the implementation is not complete. What is missing there?

In my point of view a destructor in this class was missing.<br />
Using `~udp_socket_t()` was useful to close the socket file descriptor and this way we avoid file descriptor leaks.

### 1.2 C++ constructors don't allow returning errors.<br />How would you avoid having to check that the socket is valid using the udp_socket_t::is_valid() function and rather signal immediately that the construction failed, without using exceptions?

To make this work we can implement a `Factory Method`.<br />This static method is used to create an instance of udp_socket_t dynamically.<br />It returns a unique pointer, which is a smart pointer that takes the ownership of the udp_socket_t object.<br />
This method encapsulates the creation of the initial validation process, returning nullptr if the socket setup fails.

### 1.3 Testing the udp_socket_t class

In order to test all these functionalities I created a new file called [TestUDP.cpp](https://github.com/RafaSoares1/bisect/blob/main/cpp-challenge-main/demo/mainUDP.cpp) in order to make sure that this class was working as expected.

In order for this to work something was missing: so I created the `send_to()` method that allows sending data to a specified destination address and port. It uses the `sendto() system call`, transferring the user provided data to the given destination address.

This TestUDP start by creating a UDP socket, by calling the static create method.
Next, we prepare the destination address structure of type sockaddr_in, which is used by the send_to method to send data. 
Then we send a message using the send_to method to the defined destination address.
Finally we call the handle_read(). This method attempts to read data available on the socket in a non-blocking manner and print it, showing that this code can send and receive,with UDP, data to the same machine that’s running the program. 
