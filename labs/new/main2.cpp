#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/poll.h>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>

#define RUNNING 1
#define MAX_EVENTS 64
#define INDEX_FILE "index.html"
#define BUFFER_SIZE 4096

void printError(const std::string &msg) {
    std::cerr << msg << std::endl;
    std::exit(1);
}

// Function to make a socket non-blocking
void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        printError("fcntl F_GETFL error");
    }
    
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        printError("fcntl F_SETFL error");
    }
}

class Server {
    public:
        int socketFd;
        int port;
        struct sockaddr_in serverAddr;
        
        Server() : socketFd(-1), port(0) {}
        
        void init(int serverPort) {
            port = serverPort;
            
            if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                printError("Error creating socket");
            }
            
            // Set socket options to reuse address
            int opt = 1;
            if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                printError("Error setting socket options");
            }
            
            memset(&serverAddr, 0, sizeof(serverAddr));
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(port);
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            
            if (bind(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
                printError("Error binding socket on port " + toString(port));
            }
            
            if (listen(socketFd, 10) < 0) {
                printError("Error listening on socket");
            }
            
            // Make the socket non-blocking
            setNonBlocking(socketFd);
            
            std::cout << "Server initialized on port " << port << std::endl;
        }
        
        std::string toString(int number) {
            std::stringstream ss;
            ss << number;
            return ss.str();
        }
        
        ~Server() {
            if (socketFd > 0) {
                close(socketFd);
                std::cout << "Server on port " << port << " stopped" << std::endl;
            }
        }
};

class WebServer {
    private:
        std::vector<Server*> servers;
        std::vector<struct pollfd> pollfds;
        std::vector<int> clientFds;
        bool running;
        
    public:
        WebServer() : running(true) {}
        
        ~WebServer() {
            // Close all client connections
            for (size_t i = 0; i < clientFds.size(); i++) {
                close(clientFds[i]);
            }
            
            // Clean up servers
            for (size_t i = 0; i < servers.size(); i++) {
                delete servers[i];
            }
        }
        
        void addServer(int port) {
            Server *server = new Server();
            server->init(port);
            servers.push_back(server);
            
            // Add server socket to poll list
            struct pollfd pfd;
            pfd.fd = server->socketFd;
            pfd.events = POLLIN;
            pollfds.push_back(pfd);
            
            std::cout << "Server added on port " << port << std::endl;
        }
        
        void acceptConnection(int serverIdx) {
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            
            int serverFd = servers[serverIdx]->socketFd;
            int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
            
            if (clientFd < 0) {
                if (errno != EWOULDBLOCK && errno != EAGAIN) {
                    std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
                }
                return;
            }
            
            // Make client socket non-blocking
            setNonBlocking(clientFd);
            
            // Add client to poll list
            struct pollfd pfd;
            pfd.fd = clientFd;
            pfd.events = POLLIN;
            pollfds.push_back(pfd);
            
            // Add client to client list
            clientFds.push_back(clientFd);
            
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
            std::cout << "New connection from " << clientIP << ":" << ntohs(clientAddr.sin_port) 
                      << " on server port " << servers[serverIdx]->port << std::endl;
        }
        
        std::string readFile(const std::string &filename) {
            std::ifstream file(filename.c_str());
            if (!file.is_open()) {
                return ""; // Return empty string if file can't be opened
            }
            
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }
        
        void handleClient(int pollIdx) {
            int clientFd = pollfds[pollIdx].fd;
            char buffer[BUFFER_SIZE];
            ssize_t bytesRead = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);
            
            if (bytesRead <= 0) {
                if (bytesRead == 0) {
                    std::cout << "Client disconnected" << std::endl;
                } else {
                    std::cerr << "Error reading from client: " << strerror(errno) << std::endl;
                }
                
                // Close socket
                close(clientFd);
                
                // Remove client from poll list
                pollfds.erase(pollfds.begin() + pollIdx);
                
                // Remove from clientFds
                for (size_t i = 0; i < clientFds.size(); i++) {
                    if (clientFds[i] == clientFd) {
                        clientFds.erase(clientFds.begin() + i);
                        break;
                    }
                }
                
                return;
            }
            
            buffer[bytesRead] = '\0';
            
            // Simple HTTP request handling - check if it's a GET request
            if (strncmp(buffer, "GET", 3) == 0) {
                // Read index.html content
                std::string content = readFile(INDEX_FILE);
                if (content.empty()) {
                    content = "<html><body><h1>Error: index.html not found</h1></body></html>";
                }
                
                // Prepare HTTP response
                std::stringstream response;
                response << "HTTP/1.1 200 OK\r\n";
                response << "Content-Type: text/html\r\n";
                response << "Content-Length: " << content.size() << "\r\n";
                response << "Connection: close\r\n";
                response << "\r\n";
                response << content;
                
                // Send response
                std::string responseStr = response.str();
                ssize_t bytesSent = send(clientFd, responseStr.c_str(), responseStr.size(), 0);
                
                if (bytesSent < 0) {
                    std::cerr << "Error sending response: " << strerror(errno) << std::endl;
                }
                
                // Close connection after sending response
                close(clientFd);
                
                // Remove client from poll list
                pollfds.erase(pollfds.begin() + pollIdx);
                
                // Remove from clientFds
                for (size_t i = 0; i < clientFds.size(); i++) {
                    if (clientFds[i] == clientFd) {
                        clientFds.erase(clientFds.begin() + i);
                        break;
                    }
                }
            }
        }
        
        bool isServerSocket(int fd) {
            for (size_t i = 0; i < servers.size(); i++) {
                if (servers[i]->socketFd == fd) {
                    return true;
                }
            }
            return false;
        }
        
        int getServerIndex(int fd) {
            for (size_t i = 0; i < servers.size(); i++) {
                if (servers[i]->socketFd == fd) {
                    return i;
                }
            }
            return -1;
        }
        
        void run() {
            int timeout = 1000; // Poll timeout in milliseconds
            
            std::cout << "Web server running with " << servers.size() << " servers" << std::endl;
            
            while (running) {
                int numEvents = poll(&pollfds[0], pollfds.size(), timeout);
                
                if (numEvents < 0) {
                    std::cerr << "Poll error: " << strerror(errno) << std::endl;
                    break;
                }
                
                // Process events
                for (size_t i = 0; i < pollfds.size() && numEvents > 0; i++) {
                    if (pollfds[i].revents & POLLIN) {
                        numEvents--;
                        
                        if (isServerSocket(pollfds[i].fd)) {
                            // Server socket has activity - accept new connection
                            int serverIdx = getServerIndex(pollfds[i].fd);
                            acceptConnection(serverIdx);
                        } else {
                            // Client socket has activity - handle client
                            handleClient(i);
                        }
                    }
                }
            }
        }
        
        void stop() {
            running = false;
        }
};

int main() {
    std::cout << "Starting webserver..." << std::endl;
    
    WebServer webserver;
    webserver.addServer(8080);
    webserver.addServer(8081);
    // webserver.addServer(8082);
    // webserver.addServer(8083);
    
    std::cout << "Webserver running on ports 8080 and 8081" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    
    webserver.run();
    
    return 0;
}
