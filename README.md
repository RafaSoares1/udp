# Overview

The main goal of this exercise was to create a UDP server.<br />
**UDP(User Datagram Protocol)** is a communication protocol used across the Internet for especially time-sensitive transmissions like live-streams, video calls and gaming, where speed is more critical than accuracy.

## 1) UDP Socket

The udp_socket_t class defined has the functionality for creating and receiving data over a UDP socket.

### 1.1) There is a comment that implies that the interface and the implementation is not complete. What is missing there?

In my point of view a destructor in this class was missing.<br />
Using `~udp_socket_t()` was useful to close the socket file descriptor and this way we avoid file descriptor leaks.

### 1.2) C++ constructors don't allow returning errors.<br />How would you avoid having to check that the socket is valid using the udp_socket_t::is_valid() function and rather signal immediately that the construction failed, without using exceptions?

To make this work we can implement a `Factory Method`.<br />This static method is used to create an instance of udp_socket_t dynamically.<br />It returns a unique pointer, which is a smart pointer that takes the ownership of the udp_socket_t object.<br />
This method encapsulates the creation of the initial validation process, returning nullptr if the socket setup fails.

### 1.3) Testing the udp_socket_t class

In order to test all these functionalities I created a new file called [TestUDP.cpp](https://github.com/RafaSoares1/bisect/blob/main/cpp-challenge-main/demo/TestUDP.cpp) in order to make sure that this class was working as expected.

In order for this to work something was missing: so I created the `send_to()` method that allows sending data to a specified destination address and port. It uses the `sendto() system call`, transferring the user provided data to the given destination address.

This TestUDP start by creating a UDP socket, by calling the static create method.
Next, we prepare the destination address structure of type sockaddr_in, which is used by the send_to method to send data. 
Then we send a message using the send_to method to the defined destination address.
Finally we call the handle_read(). This method attempts to read data available on the socket in a non-blocking manner and print it, **showing that this code can send and receive,with UDP, data to the same machine that’s running the program**. 

### Demo of udp_socket_t working:

![2024-04-0818-05-58-ezgif com-video-to-gif-converter](https://github.com/RafaSoares1/bisect/assets/103336451/126deead-c751-44a2-a73d-54041ef869a2)

## 2) Implementing the reactor_t class
This class manages a collection of handlers that are used to read events, such as file descriptors becoming ready to read.<br />
It runs a separate thread that continuously monitors these file descriptors for activity.

`Constuctor:` initializes the stop_signal as false and launches a thread(reactor_thread) that runs the monitor_handlers(). This thread will continuously check the registered FDs for activity.

`Destructor:` signals the monitoring thread to stop by setting stop_signal to true and waits for the thread to finish execution by joining it.

`register_handler():` adds a new handler to the collection(handlers). It uses mutex to ensure a safe thread access.

`unregister_handler():` Removes a handler from the collection based on its file descriptor. It also uses a mutex to ensure thread-safe access.

### 2.1) The monitoring thread: monitor_handlers()
This method continuously monitors the registered Fds.<br />It uses `select()` system call to wait for any of the registered FDs to become ready for reading or until timeout occurs.

1. `Prepare a list of FDs:` sets up fd_set, wich is a list that keeps track of which FDs it should monitor for potential incoming data. Then we make a copy of the registered handlers to avoid locking issues with other threads that might want to register or unregister handlers while its monitoring them.

2. `Find max_fd:` finds the highest FD number in the copied list of handlers. **Select()** function requires it.

3. `Set timeout:` it sets a timeout of 1 second, so select() will wait 1 second for an FD to become ready. If no FD is ready within that time, select finishes waiting.

4. `Handle ready FDs:` after select finishes, it goes through the copied list of handlers to see which FDs are ready(using **FD_ISSET**). For each FD that has data ready to be read, it calls that FDs handle_read method, letting it process the incoming data.<br />
This means our application can handle multiple connections simultaneously, reacting to events as they occur, without one operation blocking the execution of others (**asynchronously connection**)

5. `Repeat:` it goes back to step one and repeats the process, unless its been told to stop via **stop_signal**.

## 3) Main function
With everything set up, let me explain how the [main](https://github.com/RafaSoares1/bisect/blob/main/cpp-challenge-main/demo/main.cpp) function works:

1. `Setup UDP sockets:` two UDP sockets are created and bound to ports 5000(s1) e 6000(s2).

2. `Creating reactor:` reactor_t object is instantiated. The reactor runs in its own thread, continuously monitoring registered handlers for any activity

3. `Registering Handlers:` The UDP sockets s1 and s2 are registered with the reactor handlers. The register_handler() method adds these sockets to the reactors map of FDs to be monitored.

4. `Unregistering handlers:` Before the program exits, it gives the user an option to unregister one of the sockets(s2) by waiting for a key press. Unregister_handler()method removes s2 FD from the reactor monitoring list.

5. `End of the program:` before exiting waits for another key press. When this happens the destructors of reactor_t object, s1 and s2 will be called

## 4) How to test the UDP connection

When the program is running and waiting for a connection to be established, we can use a Netcat command(networking tool that can read and write data across network connections) to send a message.

We can open another terminal and try to check if everything is working as expected by doing the following:

```sh
echo "test message5000" | nc -u 127.0.0.1 5000
```
```sh
echo "test message6000" | nc -u 127.0.0.1 6000
```

![2024-04-0818-51-06-ezgif com-video-to-gif-converter](https://github.com/RafaSoares1/bisect/assets/103336451/90612df4-efca-4f42-8143-1dc909405ee9)
