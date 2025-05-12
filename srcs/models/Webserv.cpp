/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:25:44 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/12 10:34:34 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Webserv.hpp"
#include <iostream>
#include <string>
#include <sys/poll.h>


Webserv::Webserv() {
}

Webserv::~Webserv() {
	delete notFound;
	notFound = NULL;
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
	if (this->_servers.empty())
		throw std::runtime_error("No servers found");
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		this->_servers[i].initServer();
		this->_pollfds.push_back((struct pollfd){this->_servers[i].getSocket(), POLLIN, 0});
	}
}

void	Webserv::run() {
	this->_init();
	while (RUNNING) {
		if ((this->_nbrEvents = poll(_pollfds.data(), _pollfds.size(), 0)) < 0) {
			std::cerr << COL_RED << "Error while polling: " << END_COL << std::endl;
			continue;
		}
		for (size_t i = 0; i < _pollfds.size() && this->_nbrEvents > 0; ++i) {
			if (_pollfds[i].revents & (POLLERR | POLLHUP)) {
				if (!isServerSocket(_pollfds[i].fd))
					this->_closeClient(_pollfds[i].fd);
				--this->_nbrEvents;
				continue;
			}
			if (_pollfds[i].revents & POLLIN) {
				--this->_nbrEvents;
				if (isServerSocket(_pollfds[i].fd))
					this->acceptNewConnection(_pollfds[i].fd);
				else {
					this->handleClientRequest(i, _pollfds[i].fd);
				}
				printTime(); std::cout << COL_BLUE << " Events nbr: " << this->_nbrEvents << ":" << _pollfds[i].fd << END_COL << std::endl;
			}
			if (_pollfds[i].revents & POLLOUT) {
				//! Writting to client should be here
				//? 1. Check if client still exists in _requests
				//? 2. check if the request progress is complete
				//? 3. send response
				//? 4. close client
				std::cout << COL_BLUE << "Sending response to client..." << END_COL << std::endl;
				this->sendResponse(_pollfds[i].fd);
			}
		}
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
}

//! Handle Client Request
//! 1. Read request from socket to buffer
//! 2. Append buffer to request
//! 3. Check if request is complete
void	Webserv::handleClientRequest(int pollIdx, int fd)
{
	(void)pollIdx;
	//! delete this
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
	std::cout << COL_RED << " --------------------------------- " << END_COL << std::endl; //! remove this
	std::cout << "Received request: \n" << "buffer" << std::endl; //! remove this
	this->_requests[fd].request += buffer;
	std::cout << COL_RED << " --------------------------------- " << END_COL << std::endl; //! remove this
	std::map<int, ClientData>::iterator it = this->_requests.find(fd);
	if (it == this->_requests.end()) {
		std::cerr << "Error: client not found" << std::endl;
		return;
	}
	if (FtPars::getCurrentTimeMs() - it->second.startTime > (it->second.server->getTimeout() * 1000)) //!
	{
		std::cout << "Client timeout" << std::endl;
		this->_closeClient(fd);
		return;
	}
	if (it->second.bodyReded != -1) {
		it->second.bodyReded += bytesRead;
		// std::cout << COL_GREEN << "Body readed: " << it->second.bodyReded << END_COL << std::endl;
	}
	if (this->_isRequestComplete(it->second)) {
		printWarning("Request Ready......................>>>>");
		it->second.progress = READY;
		this->enablePOLLOUT(fd);
	}
}

void	Webserv::sendResponse(int fd) //?! Complete the request, you have to send headers, body and file
{
	mapIt it = this->_requests.find(fd);
	if (it == this->_requests.end()) {
		std::cerr << "Error: client not found" << std::endl;
		return;
	}
	if (it->second.progress == READY) {
		this->handleRequest(it->second);
	}
	std::cout << " ************>>>>>>>>>>>>>>>>>> Sending response to client..." << std::endl;
}

