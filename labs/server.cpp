/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 22:31:18 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/19 23:33:21 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h>
#include <sys/poll.h>
#include <vector>
#include <cstring> // For memset
#include <unistd.h>

#define PORT 8081
#define MAX_CLIENTS 10

void printError(const char* message) {
    std::cerr << "====> Error: " << message << std::endl;
    exit(1);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
	struct pollfd fdPoll;
    std::vector<struct pollfd> fds;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printError("Socket creation failed");
    }

    // Set socket options (reuse address)
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        printError("Setsockopt failed");
    }

    // Bind socket
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printError("Bind failed");
    }

    // Listen for connections
    if (listen(sockfd, MAX_CLIENTS) < 0) {
        printError("Listen failed");
    }

    // Add server socket to pollfds
	fdPoll = (struct pollfd){sockfd, POLLIN, 0};
    fds.push_back(fdPoll);

    char buffer[1024];
    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";

    while (true) {
        // Wait for events
        if (poll(fds.data(), fds.size(), -1) < 0) {
            printError("Poll failed");
        }

        // Iterate through file descriptors
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == sockfd) { // New connection
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
                    
                    if (client_fd < 0) {
                        printError("Accept failed");
                    }

                    // Add new client to pollfds
					fdPoll = (struct pollfd){client_fd, POLLIN, 0};
                    fds.push_back(fdPoll);
                    std::cout << "New client connected: " << client_fd << std::endl;
                } else { // Existing client sent data
                    int client_fd = fds[i].fd;
                    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer)-1);
                    
                    if (bytes_read <= 0) { // Client closed connection or error
                        close(client_fd);
                        fds.erase(fds.begin() + i);
                        std::cout << "Client disconnected: " << client_fd << std::endl;
                    } else {
                        // Send response
                        write(client_fd, response, strlen(response));
                        close(client_fd);
                        fds.erase(fds.begin() + i);
                    }
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
