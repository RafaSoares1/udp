#include "udp_socket.h"
#include <cstring>
#include <arpa/inet.h> // For inet_pton
#include <iostream>

using namespace std;

int main()
{
    auto udpSocket = bisect::demo::udp_socket_t::create(8080);

    if (!udpSocket) {
        cout << "Failed to open UDP socket." << endl;
        return 1;
    }

	sockaddr_in destAddr;
	destAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &destAddr.sin_addr);
	destAddr.sin_port = htons(8080);

	const char* message = "Hello UDP!632541";
	size_t messageLen = strlen(message) + 1;

	ssize_t bytesSent = udpSocket->send_to(message, messageLen, destAddr);
	if (bytesSent < 0)
	{
		std::cerr << "Failed to send message." << std::endl;
		return 2;
	} 
	else
	{
		std::cout << "Sent " << bytesSent << " bytes." << std::endl; //send working well
	}

	/// This will show that everything works correctly.
	/// UDP socket can immediately read back. Correctly configured for sending and receiving data.
    udpSocket->handle_read();


	return 0;
}
