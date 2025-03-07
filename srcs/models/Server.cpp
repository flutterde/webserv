/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:56:06 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/26 09:52:54 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Server.hpp"
#include "./../../headers/debug.hpp" //!

Server::Server(void)//! why ?
{		
	this->limitClientBodySize = FT_LIMIT_BODY_SIZE;
	this->port = FT_PORT;
	this->serverName = "127.0.0.1";
	this->serverBind = -1;
	this->serverSocket = -1;
}

static std::string&	validateAndTrim(std::string& str) {
	try
	{
		str = str.substr(str.find_first_of('=') + 1, str.length());
		str = FtPars::strTrim(str, " 	");
		FtPars::isValidPattern(str);
		str = FtPars::strTrim(str, "\"");
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
	return (str);
}

static void	fillServerData(std::string& line, Server& srv) {
	std::string str;
	str = line;
	if (!line.compare(0, 11, "server_name")) {
		validateAndTrim(str);
		srv.setserverName(str);
	} else if (!line.compare(0, 4, "host")) {
		validateAndTrim(str);
		FtPars::isValidIP4(str);
		srv.setHost(str);
	} else if (!line.compare(0, 4, "port")) {
		validateAndTrim(str);
		srv.setPort(std::atoi(str.c_str()));
	} else if (!line.compare(0, 20, "client_max_body_size")) {
		validateAndTrim(str);
		srv.setLimitClientBodySize(std::atoi(str.c_str()));
	} else if (!line.compare(0, 14, "error_page_404")) {
		validateAndTrim(str);
		srv.setErrorPage404(str);
	} else if (!line.compare(0, 14, "error_page_500")) {
		validateAndTrim(str);
		srv.setErrorPage500(str);
	}else if (!line.compare(0, 15, "allowed_methods")) {
		std::cout << "-----> allowed_methods: " << str << std::endl;
		validateAndTrim(str);
		// srv.getMethods() = FtPars::parseMethods(srv.getAllowedMethods(), str);
		srv.setMethods(FtPars::parseMethods(srv.getAllowedMethods(), str));
	} else if (!line.compare(0, 7, "indexes")) {
		validateAndTrim(str);
		FtPars::setServerIndexes(srv, str);
	} else if (!line.compare(0, 9, "autoindex")) {
		validateAndTrim(str);
		FtPars::autoIndexHandler(srv, str);
	}
}

static void	setServer(std::vector<std::string>& arr, size_t& idx, Server& srv)
{
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
	this->limitClientBodySize = FT_LIMIT_BODY_SIZE;
	this->port = FT_PORT;
	this->allowedMethods["GET"] = true;
	this->allowedMethods["POST"] = false;
	this->allowedMethods["DELETE"] = false;
	this->indexes["index.html"] = false;
	this->autoIndex = false;
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
	return (this->errorPage500);
}

std::map<std::string, bool>	Server::getAllowedMethods(void) const {
	return (this->allowedMethods);
}

std::map<std::string, bool>	Server::getIndexes(void) const
{
	return (this->indexes);
}

bool	Server::getAutoIndex(void) const
{
	return (this->autoIndex);
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
	this->errorPage500 = val;
}

void	Server::setIndex(std::string& key, bool val)
{
	this->indexes[key] = val;
}

void	Server::setMethods(std::map<std::string, bool> mp)
{
	this->allowedMethods = mp;
}

void	Server::setAutoIndex(bool val)
{
	this->autoIndex = val;
}


void	Server::initServer(void)
{

}
