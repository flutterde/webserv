/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:25:44 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/21 18:02:32 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Webserv.hpp"

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
