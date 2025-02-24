/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:56:06 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/24 12:40:33 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Server.hpp"
#include "./../../headers/debug.hpp" //!

Server::Server(void)
{		
	this->limitClientBodySize = FtPars::limitBodySize;
	this->port = FtPars::port;
	this->serverName = "127.0.0.1";
}

static void	fillServerData(std::string& line, Server& srv) {
	std::string str;
	if (!line.compare(0, 11, "server_name")) {
		str = line.substr(line.find_first_of('=') + 1, line.length());
		str = FtPars::strTrim(str, " \"");
		srv.setserverName(str);
	} else if (!line.compare(0, 4, "host")) {
		std::string str = line.substr(line.find_first_of('=') + 1, line.length());
		str = FtPars::strTrim(str, " \"");
		srv.setHost(str);
	} else if (!line.compare(0, 4, "port")) {
		std::string str = line.substr(line.find_first_of('=') + 1, line.length());
		str = FtPars::strTrim(str, " \"");
		srv.setPort(std::atoi(str.c_str()));
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
	this->limitClientBodySize = FtPars::limitBodySize;
	this->port = FtPars::port;
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
