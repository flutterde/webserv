#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main()
{
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	std::cerr << "socket number: " << socketFd << std::endl;

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	bind(socketFd, (struct sockaddr *)&address, sizeof(address));
	listen(socketFd, 5);
	std::cout << "Server is listening on port " << PORT << std::endl;

	socklen_t addrlen = sizeof(address);
	int new_socket;
	char buffer[204800];
	// Accept a connection
	const char *http_response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: 311\r\n"
		"Connection: close\r\n\r\n"
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"    <meta charset=\"UTF-8\">\n"
		"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		"    <title>Simple Webpage</title>\n"
		"</head>\n"
		"<body>\n"
		"    <h1>Welcome to My Simple Webpage</h1>\n"
		"    <p>This is a basic webpage served from C++.</p>\n"
		"</body>\n"
		"</html>\n";
	while (true)
	{
		new_socket = accept(socketFd, (struct sockaddr *)&address, &addrlen);
		if (new_socket < 0)
		{
			std::cerr << "Accept failed" << std::endl;
			close(socketFd);
			return EXIT_FAILURE;
		}

		std::cout << "Waiting for data..." << std::endl;
		// Read the request
		ssize_t valread = read(new_socket, buffer, sizeof(buffer) - 1);
		if (valread < 0)
		{
			std::cerr << "Read failed" << std::endl;
		}
		else
		{
			buffer[valread] = '\0';
			std::cout << "Received request:\n"
					  << buffer << std::endl;
		}

		// Close the sockets

		// std::cout << http_response;
		send(new_socket, http_response, strlen(http_response), 0);
		close(new_socket);
	}

	close(socketFd);
	return EXIT_SUCCESS;
}