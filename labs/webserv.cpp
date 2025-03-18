/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:04:35 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/21 19:15:22 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>

#define PORT 8081
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
# define END_OF_HEADERS "\r\n\r\n"

// Function to send HTTP response with HTML file content
void sendResponse(int client_socket, const char* file_path) {
    FILE* html_file = fopen(file_path, "r");
    if (!html_file) {
        // If file not found, send 404 response
        const char* response = "HTTP/1.1 404 Not Found\r\n"
                              "Content-Type: text/html\r\n"
                              "Connection: close\r\n"
                              "\r\n"
                              "<html><body><h1>404 Not Found</h1></body></html>";
        write(client_socket, response, strlen(response));
        return;
    }
    
    // Get file size
    fseek(html_file, 0, SEEK_END);
    long file_size = ftell(html_file);
    fseek(html_file, 0, SEEK_SET);
    
    // Read file content
    char* file_content = (char*)malloc(file_size + 1);
    if (!file_content) {
        fclose(html_file);
        return;
    }
    
    fread(file_content, 1, file_size, html_file);
    file_content[file_size] = '\0';
    fclose(html_file);
    
    // Create HTTP header
    char header[BUFFER_SIZE];
    sprintf(header, "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/html\r\n"
                   "Content-Length: %ld\r\n"
                   "Connection: keep-alive\r\n"
                   "\r\n", file_size);
    
    // Send header
    write(client_socket, header, strlen(header));
    
    // Send file content
    write(client_socket, file_content, file_size);
    
    free(file_content);
}

// Function to handle HTTP requests
void handleRequest(int client_socket, char* buffer) {
    // Extract the request method and path
    char method[10], path[255], protocol[20];
    sscanf(buffer, "%s %s %s", method, path, protocol);
    
    printf("Request: %s %s %s\n", method, path, protocol);
    
    // We only handle GET requests in this simple server
    if (strcmp(method, "GET") == 0) {
        // If root path requested, serve index.html
        if (strcmp(path, "/") == 0) {
            strcpy(path, "/index.html");
        }
        
        // Construct file path (remove leading '/')
        char file_path[270] = "./";
        strcat(file_path, path + 1);
        
        // Send response with file content
        sendResponse(client_socket, file_path);
    } else {
        // For non-GET requests, send 501 Not Implemented
        const char* response = "HTTP/1.1 501 Not Implemented\r\n"
                              "Content-Type: text/plain\r\n"
                              "Connection: close\r\n"
                              "\r\n"
                              "Only GET requests are supported by this server";
        write(client_socket, response, strlen(response));
    }
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }
    
    // Set socket to reuse address
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        return 1;
    }
    
    // Set socket to non-blocking mode
    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    
    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket to the specified port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        return 1;
    }
    
    printf("Server started on port %d\n", PORT);
    
    // Array of pollfd structures for polling
    struct pollfd fds[MAX_CLIENTS + 1]; // +1 for the server socket
    int nfds = 1;
    
    // Initialize the first pollfd structure with server socket
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    
    // Initialize the rest of the pollfd array
    for (int i = 1; i <= MAX_CLIENTS; i++) {
        fds[i].fd = -1; // -1 indicates available slot
    }
    
    // Main server loop
    while (1) {
        // Wait for events on the sockets
        int activity = poll(fds, nfds, -1);
        
        if (activity < 0) {
            perror("Poll error");
            break;
        }
        
        // Check if there is a new connection on the server socket
        if (fds[0].revents & POLLIN) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            
            int new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
            if (new_socket >= 0) {
                // Set new socket to non-blocking
                fcntl(new_socket, F_SETFL, O_NONBLOCK);
                
                // Find an empty slot in the fds array
                int i;
                for (i = 1; i <= MAX_CLIENTS; i++) {
                    if (fds[i].fd < 0) {
                        fds[i].fd = new_socket;
                        fds[i].events = POLLIN;
                        break;
                    }
                }
                
                // If no empty slot is found, close the connection
                if (i > MAX_CLIENTS) {
                    printf("Too many connections, refusing new client\n");
                    close(new_socket);
                } else {
                    printf("New connection accepted, socket fd is %d\n", new_socket);
                    
                    // Update nfds if necessary
                    if (i >= nfds) {
                        nfds = i + 1;
                    }
                }
            }
        }
        
        // Check client sockets for data
        for (int i = 1; i < nfds; i++) {
            if (fds[i].fd >= 0 && (fds[i].revents & POLLIN)) {
                // Data is available on this socket
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_read = read(fds[i].fd, buffer, BUFFER_SIZE - 1);
                
                if (bytes_read <= 0) {
                    // Error or connection closed
                    if (bytes_read == 0) {
                        printf("Client disconnected, socket fd %d\n", fds[i].fd);
                    } else {
                        perror("Read error");
                    }
                    
                    close(fds[i].fd);
                    fds[i].fd = -1;
                } else {
                    // Process the HTTP request
                    buffer[bytes_read] = '\0';
                    handleRequest(fds[i].fd, buffer);
                    
                    // For simplicity, we'll close the connection after handling the request
                    // In a production server, you might keep it open for persistent connections
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
            }
        }
        
        // Clean up the fds array by moving all used slots to the beginning
        // and adjusting nfds accordingly
        for (int i = 1; i < nfds; i++) {
            if (fds[i].fd == -1) {
                // Find the next non-empty slot
                int j;
                for (j = i + 1; j < nfds; j++) {
                    if (fds[j].fd != -1) {
                        break;
                    }
                }
                
                if (j == nfds) {
                    // No more used slots found
                    nfds = i;
                    break;
                } else {
                    // Move the non-empty slot here
                    fds[i].fd = fds[j].fd;
                    fds[j].fd = -1;
                }
            }
        }
    }
    
    // Close the server socket
    close(server_fd);
    
    return 0;
}