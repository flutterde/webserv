#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <map>

#define RUNNING 1
#define MAX_EVENTS 64
#define INDEX_FILE "index.html"
#define INDEX_FILE2 "app.html"
#define BUFFER_SIZE 4096

void printError(const std::string &msg) {
    std::cerr << msg << std::endl;
    std::exit(1);
}

// Function to make a socket non-blocking
void setNonBlocking(int sockfd) {
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
        printError("fcntl F_SETFL error");
    }
}

class Server {
    public:
        std::string indexFile;
        int socketFd;
        int port;
        struct sockaddr_in serverAddr;
        
        Server() : socketFd(-1), port(0) {}
        
        void init(int serverPort, const std::string& indexFile) {
            port = serverPort;
            this->indexFile = indexFile;
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

//! WebServer class
class WebServer {
    private:
        std::vector<Server*> servers;
        std::vector<struct pollfd> pollfds;
        std::vector<int> clientFds;
        // Map to track which client belongs to which server
        std::map<int, int> clientToServerMap;
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
        
        void addServer(int port, const std::string& indexFile) {
            Server *server = new Server();
            server->init(port, indexFile);
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
            
            // Add mapping of client to server
            clientToServerMap[clientFd] = serverIdx;
            
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
        
        // Helper function to close and remove client
        void closeAndRemoveClient(int clientFd, int pollIdx) {
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
            
            // Remove from the client-server map
            clientToServerMap.erase(clientFd);
        }
        
        // Send a 404 Not Found response
        void sendNotFoundResponse(int clientFd) {
            std::string content = "<html><body><h1>404 Not Found</h1><p>The requested resource could not be found.</p></body></html>";
            
            std::stringstream response;
            response << "HTTP/1.1 404 Not Found\r\n";
            response << "Content-Type: text/html\r\n";
            response << "Content-Length: " << content.size() << "\r\n";
            response << "Connection: close\r\n";
            response << "\r\n";
            response << content;
            
            std::string responseStr = response.str();
            send(clientFd, responseStr.c_str(), responseStr.size(), 0);
        }
        
        // Send a 405 Method Not Allowed response
        void sendMethodNotAllowedResponse(int clientFd) {
            std::string content = "<html><body><h1>405 Method Not Allowed</h1><p>The requested method is not supported.</p></body></html>";
            
            std::stringstream response;
            response << "HTTP/1.1 405 Method Not Allowed\r\n";
            response << "Content-Type: text/html\r\n";
            response << "Content-Length: " << content.size() << "\r\n";
            response << "Allow: GET, POST\r\n";
            response << "Connection: close\r\n";
            response << "\r\n";
            response << content;
            
            std::string responseStr = response.str();
            send(clientFd, responseStr.c_str(), responseStr.size(), 0);
        }
        
        // Handle GET requests
        void handleGetRequest(int clientFd, const std::string& path, Server* srv) {
            std::string filePath;
            
            if (path == "/" || path == "/index.html") {
                filePath = srv->indexFile;
            } else {
                // Remove leading slash and handle other files
                filePath = path.substr(1);
            }
            
            std::string content = readFile(filePath);
            if (content.empty()) {
                sendNotFoundResponse(clientFd);
                return;
            }
            
            // Determine content type based on file extension
            std::string contentType = "text/html";
            size_t dotPos = filePath.find_last_of('.');
            if (dotPos != std::string::npos) {
                std::string extension = filePath.substr(dotPos + 1);
                if (extension == "css") contentType = "text/css";
                else if (extension == "js") contentType = "application/javascript";
                else if (extension == "json") contentType = "application/json";
                else if (extension == "png") contentType = "image/png";
                else if (extension == "jpg" || extension == "jpeg") contentType = "image/jpeg";
                else if (extension == "gif") contentType = "image/gif";
            }
            
            // Send response
            std::stringstream response;
            response << "HTTP/1.1 200 OK\r\n";
            response << "Content-Type: " << contentType << "\r\n";
            response << "Content-Length: " << content.size() << "\r\n";
            response << "Connection: close\r\n";
            response << "\r\n";
            response << content;
            
            std::string responseStr = response.str();
            send(clientFd, responseStr.c_str(), responseStr.size(), 0);
        }
        
        // Handle POST requests
        void handlePostRequest(int clientFd, const std::string& path, const std::string& body, Server* srv) {
            // Example implementation - you can customize based on your needs
            std::cout << "Handling POST request to " << path << std::endl;
            
            // Here you can process the POST data, store it, etc.
            // For this example, we'll just echo back the received data
            
            std::stringstream response;
            response << "HTTP/1.1 200 OK\r\n";
            response << "Content-Type: application/json\r\n";
            response << "Content-Length: " << body.size() << "\r\n";
            response << "Connection: close\r\n";
            response << "\r\n";
            response << body;  // Echo back the received body
            
            std::string responseStr = response.str();
            send(clientFd, responseStr.c_str(), responseStr.size(), 0);
        }
        
        void handleClient(int pollIdx) {
            int clientFd = pollfds[pollIdx].fd;
            
            // Get the server index this client belongs to
            int serverIdx = clientToServerMap[clientFd];
            
            Server *srv = servers[serverIdx];
            std::cout << "Handling client on server port: " << srv->port << std::endl;
            
            char buffer[BUFFER_SIZE];
            ssize_t bytesRead = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);

            if (bytesRead <= 0) {
                if (bytesRead == 0) {
                    std::cout << "Client disconnected" << std::endl;
                } else {
                    std::cerr << "Error reading from client: " << strerror(errno) << std::endl;
                }
                closeAndRemoveClient(clientFd, pollIdx);
                return;
            }
            buffer[bytesRead] = '\0';
            
            // Parse the HTTP request
            std::string request(buffer);
            std::string method, path, httpVersion;
            std::map<std::string, std::string> headers;
            std::string body;
            
            // Parse request line
            std::istringstream requestStream(request);
            std::string requestLine;
            std::getline(requestStream, requestLine);
            std::istringstream requestLineStream(requestLine);
            requestLineStream >> method >> path >> httpVersion;
            
            // Parse headers
            std::string headerLine;
            while (std::getline(requestStream, headerLine) && headerLine != "\r") {
                size_t colonPos = headerLine.find(':');
                if (colonPos != std::string::npos) {
                    std::string headerName = headerLine.substr(0, colonPos);
                    std::string headerValue = headerLine.substr(colonPos + 1);
                    
                    // Trim whitespace
                    headerValue.erase(0, headerValue.find_first_not_of(" \t"));
                    headerValue.erase(headerValue.find_last_not_of("\r\n") + 1);
                    
                    headers[headerName] = headerValue;
                }
            }
            
            // Extract body if present
            if (headers.find("Content-Length") != headers.end()) {
                int contentLength = std::stoi(headers["Content-Length"]);
                if (contentLength > 0) {
                    // Find the start of the body
                    size_t bodyStart = request.find("\r\n\r\n");
                    if (bodyStart != std::string::npos) {
                        body = request.substr(bodyStart + 4);
                    }
                }
            }
            
            // Log request details
            std::cout << "Method: " << method << std::endl;
            std::cout << "Path: " << path << std::endl;
            std::cout << "HTTP Version: " << httpVersion << std::endl;
            
            std::cout << "Headers:" << std::endl;
            for (const auto& header : headers) {
                std::cout << "  " << header.first << ": " << header.second << std::endl;
            }
            
            if (!body.empty()) {
                std::cout << "Body: " << body << std::endl;
            }
            
            // Handle the request based on method
            if (method == "GET") {
                handleGetRequest(clientFd, path, srv);
            } else if (method == "POST") {
                handlePostRequest(clientFd, path, body, srv);
            } else {
                sendMethodNotAllowedResponse(clientFd);
            }
            
            // After handling the request, close the connection
            closeAndRemoveClient(clientFd, pollIdx);
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
            std::cout << "Web server running with " << servers.size() << " servers" << std::endl;
            
            while (running) {
                int numEvents = poll(pollfds.data(), pollfds.size(), 0);
                
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
    webserver.addServer(8080, INDEX_FILE);
    webserver.addServer(8081, INDEX_FILE2);
    webserver.addServer(8082, INDEX_FILE);
    webserver.addServer(8083, INDEX_FILE2);
    
    std::cout << "Webserver running on ports 8080, 8081, 8082, and 8083" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    
    webserver.run();
    
    return 0;
}
