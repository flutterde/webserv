/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:25:44 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/23 20:17:13 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Webserv.hpp"
#include <string>


Webserv::Webserv() {
}

Webserv::~Webserv() {

}

Webserv::Webserv(readConfig& config, char **env) {
	
	if (!env || !*env)
		throw std::runtime_error("no env provided");
	this->_config = &config;
	this->_servers = _config->getServers();
	int i = -1;
	while (env[++i]) {
		this->_envs.push_back(*env);
	}
}

void	Webserv::_init() {
	std::cout << "initing............" << std::endl;
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		// init server //! add this
		this->_servers[i].initServer();
		// printServer(this->_servers[i]); //! remove this
		// ... 
		std::cout << "server socket: " << this->_servers[i].getSocket() << std::endl;
		this->_pollfds.push_back((struct pollfd){this->_servers[i].getSocket(), POLLIN, 0});
	}
}

void	Webserv::run() {
	this->_init();
	while (RUNNING) {
		if ((this->_nbrEvents = poll(_pollfds.data(), _pollfds.size(), -1)) < 0)
			throw std::runtime_error("poll exception error");
		for (size_t i = 0; i < _pollfds.size() && this->_nbrEvents > 0; ++i) {
			// std::cout << "NBR OF ENV: " << this->_nbrEvents << std::endl;
			if (_pollfds[i].revents & POLLIN) {
				--this->_nbrEvents;
				if (isServerSocket(_pollfds[i].fd)) {
					// acceptNewConnection
					this->acceptNewConnection(_pollfds[i].fd);
				} else {
					// handle client request
					this->handleClientRequest(i, _pollfds[i].fd);
				}
				// printTime(); std::cout << COL_BLUE << " Events nbr: " << this->_nbrEvents << ":" << _pollfds[i].fd << END_COL << std::endl;
			}
			// std::cout << "pollfd: " << _pollfds[i].fd << std::endl;
		}
		// exit(0);
		// sleep(1);//! remove this
		//  std::cout << "running.." << _pollfds.size() << std::endl; //! remove this
	}
}

Server*	Webserv::getServerByFd(int fd) {
	for (size_t i = 0; i < _servers.size(); ++i)
		if (_servers[i].getSocket() == fd)
			return (&(_servers[i]));
	throw std::runtime_error("Error while handling new connection");
}

bool	Webserv::_isRequestComplete(ClientData& client) {
	if (!client.isHeaderComplete) {
		this->isHeaderComplete(client);
	}
	this->setRequestType(client);
	this->setContentLength(client);
	this->setBoundary(client);
	//! Validate request
	//! ...
	return (this->isRequestComplete(client));
}

bool	Webserv::isServerSocket(int fd) const
{
	for (size_t i = 0; i < _servers.size(); ++i)
		if (_servers[i].getSocket() == fd)
			return (true);
	return (false);
}

void	Webserv::acceptNewConnection(int fd)
{
	std::cout << "(" << fd << ")"<< " accepting new connection..." << std::endl;
	ClientData	newClient;
	try {
		Server	*srv = this->getServerByFd(fd);
		std::cout << "server port: " << srv->getPort() << "Server name: " << srv->getserverName() << std::endl;
		struct sockaddr_in clientAddress;
		socklen_t clientAddressSize = sizeof(clientAddress);
		int clientFd = accept(fd, (struct sockaddr *)&clientAddress, &clientAddressSize);
		std::cout << "client fd: " << clientFd << std::endl; //! remove this
		if (clientFd < 0) //? Should really exit here?
			throw std::runtime_error("Error while accepting new connection");
		// this->setNonBlocking(clientFd);
		Server::setNonBlocking(clientFd);
		this->_pollfds.push_back((struct pollfd){clientFd, POLLIN, 0});
		newClient.fd = clientFd;
		newClient.server = srv;
		this->_requests[clientFd] = newClient;
		//! delete this at the end
		char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
        std::cout << "New connection from " << clientIP << ":" << ntohs(clientAddress.sin_port) 
                    << " on server port " << srv->getPort() << std::endl;
		//! end delete
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	// this->requestCount++;
}

//! Handle Client Request
//! 1. Read request from socket to buffer
//! 2. Append buffer to request
//! 3. Check if request is complete
void	Webserv::handleClientRequest(int pollIdx, int fd)
{
	(void)pollIdx;
	//! delete this

	//! end delete
	char buffer[READ_SIZE];
	ssize_t	bytesRead = recv(fd, buffer, READ_SIZE - 1, 0);
	if (bytesRead <= 0) { //! check this
		if (bytesRead == 0) {
			std::cout << "Client disconnected" << std::endl;
		} else {
			std::cerr << "Error while reading from client" << std::endl;
		}
		this->_closeClient(fd);
		return;
	}
	buffer[bytesRead] = '\0';
	// std::cout << COL_RED << " --------------------------------- " << END_COL << std::endl; //! remove this
	// std::cout << buffer << std::endl; //! remove this
	// std::cout << COL_RED << " --------------------------------- " << END_COL << std::endl; //! remove this
	// this->_requests[fd].request += buffer;
	this->_requests[fd].request.append(buffer, bytesRead);
	std::map<int, ClientData>::iterator it = this->_requests.find(fd);
	if (it == this->_requests.end()) {
		std::cerr << "Error: client not found" << std::endl;
		return;
	}
	if (it->second.bodyReded != -1) {
		it->second.bodyReded += bytesRead;
	}
	// if (this->_isRequestComplete(this->_requests[fd].request)) {
	// 	//!
	// 	std::cout << "Request complete: " << this->_requests[fd].request << std::endl;
	// 	send(fd, response.c_str(), response.size(), 0); //! check send length
	// 	this->_closeClient(fd);
	// }
	if (!this->isRequestValid(it->second)) {
		std::cout << "400 Request not valid" << std::endl;
		std::string response = "HTTP/1.1 400 Bad Request\r\n"
							"Content-Length: 0\r\n"
							"\r\n";
		send(fd, response.c_str(), response.size(), 0);
		this->_closeClient(fd);
		return;
	}
	else if (this->_isRequestComplete(it->second))
		this->handleRequest(it->second);
	
}
