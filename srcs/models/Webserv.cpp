/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:25:44 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/24 16:24:36 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Webserv.hpp"
#include "../../headers/header.hpp"
#include "../../headers/Response.hpp"
#include "../../headers/Request.hpp"
#include <stdexcept>

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
	std::cout << COL_BLUE << "initing.." << END_COL << std::endl;
	if (this->_servers.empty())
		throw std::runtime_error("No servers found");
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		this->_servers[i].initServer();
		this->_pollfds.push_back((struct pollfd){this->_servers[i].getSocket(), POLLIN, 0});
	}
}

void	Webserv::_loop() {
	
}

void	Webserv::run() {
	this->_init();
	while (RUNNING) {
		if ((this->_nbrEvents = poll(_pollfds.data(), _pollfds.size(), POLL_TIMEOUT)) < 0) continue;
		for (size_t i = 0; i < _pollfds.size() && this->_nbrEvents > 0; ++i) {
			try {
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
					else
						this->handleClientRequest(_pollfds[i].fd);
				}
				if (_pollfds[i].revents & POLLOUT)
					this->sendResponse(_pollfds[i].fd);
			} catch (std::exception& e) {
				(void)e;
			}
		}
		this->timeoutHandler();
	}
}

Server*	Webserv::getServerByFd(int fd) {
	for (size_t i = 0; i < _servers.size(); ++i)
		if (_servers[i].getSocket() == fd)
			return (&(_servers[i]));
	throw std::runtime_error("Error while handling new connection");
}

bool	Webserv::_isRequestComplete(ClientData& client) {
	if (client.progress == READY)
		return (true);
	if (!client.isHeaderComplete) {
		this->isHeaderComplete(client);
	}
	this->setRequestType(client);
	this->setContentLength(client);
	this->setBoundary(client);
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
	ClientData	newClient;
	try {
		Server	*srv = this->getServerByFd(fd);
		struct sockaddr_in clientAddress;
		socklen_t clientAddressSize = sizeof(clientAddress);
		int clientFd = accept(fd, (struct sockaddr *)&clientAddress, &clientAddressSize);
		if (clientFd < 0)
			throw std::runtime_error("Error while accepting new connection");
		Server::setNonBlocking(clientFd);
		this->_pollfds.push_back((struct pollfd){clientFd, POLLIN, 0});
		newClient.fd = clientFd;
		newClient.server = srv;
		this->_requests[clientFd] = newClient;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void	Webserv::handleClientRequest(int fd)
{
	char buffer[READ_SIZE];
	ssize_t	bytesRead = recv(fd, buffer, READ_SIZE - 1, 0);
	if (bytesRead <= 0) {
		this->_closeClient(fd);
		throw std::runtime_error("Error while reading from client");
	}
	buffer[bytesRead] = '\0';
	this->_requests[fd].request.append(buffer, bytesRead);
	std::map<int, ClientData>::iterator it = this->_requests.find(fd);
	if (it == this->_requests.end()) return;
	if (it->second.bodyReded != -1)
		it->second.bodyReded += bytesRead;
	if (this->_isRequestComplete(it->second))
		this->prepareClientResponse(it->second);
}

void	Webserv::prepareClientResponse(ClientData& client)
{
	// std::cout << " PPP, the client is: " << client.fd << std::endl;
	try {
		Request req(client.headers.append(client.request), client);
		client.status = 200000;
		if (!client.resp)
			client.resp = new Response(client, req);
		client.progress = READY;
		this->enablePOLLOUT(client.fd);
	}
	catch(std::exception& e) {
		std::cerr << COL_RED << "Error while preparing response: " << e.what() << END_COL << std::endl;
		this->_closeClient(client.fd);
		std::cout << COL_RED << " *-*-*-*-*-*-*-*-*-*-* " << END_COL << std::endl;
		throw std::runtime_error("Error while preparing response");
	}
}


void	Webserv::sendResponse(int fd)
{
	mapIt it = this->_requests.find(fd);
	if (it == this->_requests.end())
		return;
	if (it->second.progress == READY)
		this->handleRequest(it->second);
}

void Webserv::timeoutHandler(void)
{
    mapIt it = _requests.begin();
    while (it != _requests.end()) {
        if (FtPars::getCurrentTimeMs() - it->second.startTime > (it->second.server->getTimeout() * 1000)) {
            std::cout << "Client fd " << it->first << " timed out" << std::endl;
            int clientFd = it->first;
            ++it;
            this->_closeClient(clientFd);
        } else {
            ++it;
        }
    }
}


