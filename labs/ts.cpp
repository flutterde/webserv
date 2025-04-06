/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:25:44 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/06 19:02:46 by ochouati         ###   ########.fr       */
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

#define  RESPONSE_HTML "HTTP/1.1 200 OK\r\nContent-Length: 98\r\n\r\n<!DOCTYPE html><html><head><title>200 NNN Found</title></head><body><h1>Welcome</h1></body></html>"

Webserv::Webserv() {
}

Webserv::~Webserv() {

}

void setNonBlocking(int sockfd) {
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << COL_RED << "Error setting non-blocking mode: " << strerror(errno) << END_COL << std::endl;
    }
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

void	Webserv::_closeClient(int fd)
{
}


void	Webserv::_init() {
	std::cout << COL_GREEN << "Webserv is running..." << END_COL << std::endl;
	for (size_t i = 0; i < _servers.size(); ++i) {
		_servers[i].initServer();
		_pollfds.push_back((struct pollfd){_servers[i].getSocket(), POLLIN, 0});
		std::cout << COL_GREEN << "Server " << i + 1 << " is running on port " << _servers[i].getPort() << END_COL << std::endl;
	}
}

void	Webserv::run() {
	this->_init();
	while (1) {
		//!  1. poll
		if ((this->_nbrEvents = poll(_pollfds.data(), _pollfds.size(), 0)))
			throw std::runtime_error("poll error");
		//!  2. check for new connections (Process events)
		for (size_t i = 0; i < _pollfds.size() && _nbrEvents > 0; ++i) {
			if (_pollfds[i].revents & POLLIN) {
				_nbrEvents--;
				if (isServerSocket(_pollfds[i].fd)) {
					acceptNewConnection(_pollfds[i].fd);
				}
			}
		}

		//! 3. check for client requests
	}
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
		if (_servers[i].getSocket() == fd) {
			std::cout << COL_YELLOW << "Server socket found: " << fd << END_COL << std::endl;
			return (true);
		}
	return (false);
}

void	Webserv::acceptNewConnection(int srvfd)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	std::cout << COL_YELLOW << "Accepting new connection on fd: " << srvfd << END_COL << std::endl;
	int clientFd = accept(srvfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
	if (clientFd < 0) {
		std::cerr << COL_RED << "Error accepting new connection: " << strerror(errno) << END_COL << std::endl;
		return;
	}
	setNonBlocking(clientFd);
}
void	Webserv::handleClientRequest(int pollIdx, int fd)
{
}

