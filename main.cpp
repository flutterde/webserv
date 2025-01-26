/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 15:41:59 by ochouati          #+#    #+#             */
/*   Updated: 2025/01/26 12:59:08 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/header.hpp"
#include "headers/Config.hpp"

#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "config file require!" << std::endl;
		return (1);
	}
	std::string confFile(av[1]);
    std::cout << "-->>> " << confFile << std::endl;
	Config	conf;
	readConfigFile(confFile);
	std::cout << " ------------------------------ " << conf.getRootFolder() << std::endl;
	    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Attach socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(conf.getPort());
    
    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    std::cout << "HTTP server running on port " << conf.getPort() << std::endl;
    
    while (true) {
        // Accept a connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }
        
        // Read the request
        int bytes_read = read(new_socket, conf.buffer, conf.getBodySize());
        if (bytes_read > 0) {
            std::cout << "Received request:\n" << conf.buffer << std::endl;
        }
        
        // Send a basic HTTP response
        std::string response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 49\r\n"
            "\r\n"
            "<html><body><h1>Welcome to C++ HTTP Server</h1></body></html>\n\n";
        send(new_socket, response.c_str(), response.size(), 0);
        
        // Close the connection
        close(new_socket);
    }
    
    close(server_fd);
}
