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

#define RUNNING 1
#define MAX_EVENTS 10
#define INDEX_FILE "index.html"

void printError(const std::string &msg) {
	std::cerr << msg << std::endl;
	std::exit(1);
}

class Server {
	public:
		int socketFd;
		int bindFd;
		int listenFd;
		struct sockaddr_in serverAddr;
		std::vector<struct pollfd> pollfds;
		std::vector<int> clientFds;
		void init(int port);
		void stop();
		Server() {}
		~Server() {
			stop();
			std::cout << "Server stopped" << std::endl;
		}
};

void Server::init(int port) {
	if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printError("Error creating socket");
		std::exit(1);
	}
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	if ((bindFd = bind(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr))) < 0) {
		printError("Error binding socket");
		std::exit(1);
	}
	if ((listenFd = listen(socketFd, 10)) < 0) {
		printError("Error listening on socket");
		std::exit(1);
	}
}

void Server::stop() {
	close(socketFd);
	close(bindFd);
	close(listenFd);
}


int main() {
	std::cout << "Starting server..." << std::endl;
	std::vector<Server*> servers;
	Server *server = new Server();
	server->init(8080);
	servers.push_back(server);
	std::cout << "Server 1 started" << std::endl;
	server = new Server();
	server->init(8081);
	servers.push_back(server);
	std::cout << "Server 2 started" << std::endl;
	while (RUNNING) {
		for (size_t i = 0; i < servers.size(); ++i) {
			
		}
	}
	
}