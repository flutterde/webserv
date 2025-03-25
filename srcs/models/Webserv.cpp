/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:25:44 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/25 22:20:45 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Webserv.hpp"
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <ostream>
#include <stdexcept>
#include <sys/poll.h>

Webserv::Webserv() {
}

Webserv::~Webserv() {
	_closeClients();
}

void	Webserv::_closeClients() {
	for (size_t i = 0; i < _pollfds.size(); i++) {
		close(_pollfds[i].fd);
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

void	Webserv::_init() {
	std::cout << "init............" << std::endl;
	std::cout << "The Servers count: " << _servers.size() << std::endl;
}

void	Webserv::run() {
	_init();
	while (RUNNING) {
		if ((this->_nbrEvents = poll(_pollfds.data(), _pollfds.size(), POLL_TIMEOUT)) < 0)
			throw std::runtime_error("poll exception error");
		for (size_t i = 0; i < this->_pollfds.size() && this->_nbrEvents > 0; ++i) {
			if (this->_pollfds[i].revents & POLLIN) {
				this->_nbrEvents--;
				if (isServerSocket(_pollfds[i].fd))
					return ;
					// acceptNewConnection
			}
		}
		sleep(1);//! remove this
		std::cout << "running.." << std::endl;
	}
}

Server	Webserv::getServerByFd(const int clientFd) const {
	for (size_t i = 0; i < _servers.size(); ++i)
		if (_servers[i].getSocket() == clientFd)
			return (_servers[i]);
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

void	Webserv::acceptNewConnection(int fd)
{
	try {
		Server	srv = this->getServerByFd(fd);
		// accept new client
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
