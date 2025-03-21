/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:25:44 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/21 23:45:08 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Webserv.hpp"
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

void	Webserv::run() {
	while (RUNNING) {
		if ((this->_nbrEvents = poll(_pollfds.data(), _pollfds.size(), POLL_TIMEOUT) < 0)) {
			throw std::runtime_error("poll failed");
		}
	}
}

Server	Webserv::getServerByFd(const int clientFd) const {
	std::map<int, int>::const_iterator it = _clientToServer.find(clientFd);
	if (it == _clientToServer.end()) {
		std::runtime_error("server not found");
	}
	return (_servers[it->second]);
}
