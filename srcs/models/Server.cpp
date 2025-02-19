/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:56:06 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/19 14:51:27 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Server.hpp"
#include "./../../headers/debug.hpp" //!

Server::Server(void)
{		
	this->limitClientBodySize = 800000;
	this->port = 8080;
	this->serverName = "localhost";
}

static void	fillServerData(std::string& line, Server& srv) {
	std::cout << " *********** + \n";
	if (!line.compare(0, 11, "server_name")) {
		std::string str = line.substr(line.find_first_of('=') + 1, line.length());
		std::cout << "the server Name: " << str << std::endl;
		srv.setserverName(str);
		std::cout << "Get server Name: " << srv.getserverName() << std::endl;
	}
}

static void	setServer(std::vector<std::string>& arr, size_t& idx, Server& srv)
{
	(void)srv;
	idx++;
	for (size_t i = idx; i < arr.size(); ++i) {
		if (FtPars::isNewServer(arr[i])) {
			break;
		}
		fillServerData(arr[i], srv);
	}
	idx--;
}

Server::Server(std::vector<std::string>& arr, size_t& idx)
{
	(void) this->errorPage404;
	(void) this->errorPage500;
	(void) this->host;
	(void) this->limitClientBodySize;
	(void) this->port;
	(void) this->serverName;
	(void) arr;
	(void) idx;
	// printing(arr[idx]);
	std::cout << "Setting new server.. \n";
	setServer(arr, idx, *this);
}

uint32_t	Server::getPort(void)	const
{
	return (this->port);
}

std::string	Server::getHost(void)	const
{
	return (this->host);
}

std::string	Server::getserverName(void)	const
{
	return (this->serverName);
}

uint32_t	Server::getLimitClientBodySize(void)	const
{
	return (this->limitClientBodySize);
}

std::string	Server::getErrorPage404(void)	const
{
	return (this->errorPage404);
}

std::string	Server::getErrorPage500(void)	const
{
	return (this->errorPage404);
}

void	Server::setPort(uint32_t val)
{
	this->port = val;
}

void	Server::setHost(std::string& val)
{
	this->host = val;
}

void	Server::setserverName(std::string& val)
{
	this->serverName = val;
}

void	Server::setLimitClientBodySize(uint32_t val)
{
	this->limitClientBodySize = val;
}

void	Server::setErrorPage404(std::string& val)
{
	this->errorPage404 = val;
}

void	Server::setErrorPage500(std::string& val)
{
	this->errorPage404 = val;
}
