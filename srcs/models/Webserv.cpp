/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:25:44 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/24 23:51:57 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Webserv.hpp"
#include <cstddef>
#include <cstdlib>
#include <stdexcept>

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

Webserv::Webserv(readConfig& config) {
	this->_config = config;
}

void	Webserv::_init() {
	std::cout << "init............" << std::endl;
}

void	Webserv::run() {
	_init();
	while (RUNNING) {
		// if ((this->_nbrEvents = poll(_pollfds.data(), _pollfds.size(), POLL_TIMEOUT) < 0)) {
		// 	throw std::runtime_error("poll failed");
		// }
		// for (size_t i = 0; i < this->_pollfds.size(); ++i) {
		// 	if (this->_pollfds[i].revents & POLLIN) {
		// 		// read from client
		// 	}
		// }
		sleep(2);
		std::cout << "running.." << std::endl;
	}
}

Server	Webserv::getServerByFd(const int clientFd) const {
	std::map<int, int>::const_iterator it = _clientToServer.find(clientFd);
	if (it == _clientToServer.end()) {
		std::runtime_error("server not found");
	}
	return (_servers[it->second]);
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
