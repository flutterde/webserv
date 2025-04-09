/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:25:44 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/08 14:55:19 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Webserv.hpp"
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>

Webserv::Webserv() {
}

Webserv::~Webserv() {

}


void	Webserv::_closeClient(int fd)
{
	std::cout << COL_YELLOW << "Closing client fd: " << fd << END_COL << std::endl;
	// remove from pollfds
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		if (_pollfds[i].fd == fd) {
			close(fd);
			std::vector<struct pollfd>::iterator tmp = _pollfds.begin() + i;
			std::cout << "Removing fd: " << tmp->fd << std::endl;
			_pollfds.erase(tmp);
			_requests.erase(fd);
			break;
		}
	}
	// remove from requests
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
				printTime(); std::cout << COL_BLUE << " Events nbr: " << this->_nbrEvents << ":" << _pollfds[i].fd << END_COL << std::endl;
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

bool	Webserv::_isRequestComplete(const std::string& request) const {
	size_t headersEnd = request.find(END_OF_HEADERS);
	if (headersEnd == std::string::npos)
		return (false);
	size_t contentLength = request.find("Content-Length: ");
	if (contentLength == std::string::npos)
		return (true);
	size_t vStart = request.find(" ", contentLength + 15);
	size_t vEnd = request.find("\r\n", vStart);
	std::string value = request.substr(vStart, vEnd - vStart);
	int	lengthNbr = std::atoi(value.c_str());
	return (request.size() >= headersEnd + 4 + lengthNbr);
}

bool	Webserv::isServerSocket(int fd) const
{
	for (size_t i = 0; i < _servers.size(); ++i)
		if (_servers[i].getSocket() == fd)
			return (true);
	return (false);
}

// void	Webserv::setNonBlocking(int fd) {
// 	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
// 		throw std::runtime_error("Set non blocking failed");
// }

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
}
void	Webserv::handleClientRequest(int pollIdx, int fd)
{
	(void)pollIdx;
	std::string	htmlExample = "<html><body><h1> <center> Welcome to 1337 Webserv </center></h1></body></html>";
	std::string response = "HTTP/1.1 200 OK\r\n"
							"Content-Type: text/html\r\n"
							"Content-Length: " + FtPars::toString(htmlExample.size()) + "\r\n"
							"\r\n" + htmlExample;
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
	std::cout << "Received request: \n" << buffer << std::endl; //! remove this
	std::cout << COL_RED << " --------------------------------- " << END_COL << std::endl; //! remove this
	this->_requests[fd].request += buffer;
	if (this->_isRequestComplete(this->_requests[fd].request)) {
		//!
		std::cout << "Request complete: " << this->_requests[fd].request << std::endl;
		send(fd, response.c_str(), response.size(), 0); //! check send length
		this->_closeClient(fd);
	}
}
