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
#include "./../../headers/Webserv.hpp"
#include "./../../headers/debug.hpp" //!
#include <cstddef>
#include <sys/fcntl.h>


//?
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>  // For getaddrinfo

Server::Server(void)//! why ?
{		
	this->limitClientBodySize = FT_LIMIT_BODY_SIZE;
	this->port = FT_PORT;
	this->serverName = "127.0.0.1";
	this->serverBind = -1;
	this->serverSocket = -1;
	this->rootPath = DEFAULT_ROOT_PATH;
	this->clientBodyTempPath = BODY_TEMP_PATH;
}

Server::~Server(void)
{
	if (this->serverSocket != -1)
		close(this->serverSocket);
	if (this->serverBind != -1)
		close(this->serverBind);
}

Server::Server(const Server& srv, uint32_t port) //! 
{
	this->host = srv.host;
	this->port = port;
	this->serverName = srv.serverName;
	this->limitClientBodySize = srv.limitClientBodySize;
	this->errorPage404 = srv.errorPage404;
	this->errorPage500 = srv.errorPage500;
	this->allowedMethods = srv.allowedMethods;
	this->indexes = srv.indexes;
	this->autoIndex = srv.autoIndex;
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
		if (!srv.getserverName().empty())
			throw std::runtime_error("server_name already set");
		validateAndTrim(str);
		srv.setserverName(str);
	} else if (!line.compare(0, 4, "host")) {
		if (!srv.getHost().empty())
			throw std::runtime_error("host already set");
		validateAndTrim(str);
		FtPars::isValidIP4(str);
		srv.setHost(str);
	} else if (!line.compare(0, 4, "port")) {
		validateAndTrim(str);
		FtPars::serverPortsHandler(srv, str);
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
		validateAndTrim(str);
		srv.setMethods(FtPars::parseMethods(srv.getAllowedMethods(), str));
	} else if (!line.compare(0, 7, "indexes")) {
		validateAndTrim(str);
		FtPars::setServerIndexes(srv, str);
	} else if (!line.compare(0, 9, "autoindex")) {
		validateAndTrim(str);
		FtPars::autoIndexHandler(srv, str);
	} else if (!line.compare(0, 14, "upload_enabled")) {
		validateAndTrim(str);
		FtPars::enableUploadsHandler(srv, str);
	} else if (!line.compare(0, 13, "location_root")) {
		validateAndTrim(str);
		srv.setRootPath(str);
	} else if (!line.compare(0, 9, "redirects")) {
		std::cout << "-----> redirects: " << str << std::endl;
		validateAndTrim(str);
		FtPars::handleRedirects(srv, str);
	} else if (!line.compare(0, 21, "client_body_temp_path")) {
		validateAndTrim(str);
		srv.setClientBodyTempPath(str);
	} else if (!line.compare(0, 3, "cgi")) {
		validateAndTrim(str);
		FtPars::handleCGIs(srv, str);
	} else if (!line.compare(0, 14, "client_timeout")) {
		validateAndTrim(str);
		srv.setTimeout(std::atoi(str.c_str()));
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
	this->enableUploads = false;
	// this->uploadPath = "uploads";
	setServer(arr, idx, *this);
}

// GETTERS

uint32_t	Server::getPort(void)	const
{
	return (this->port);
}

size_t		Server::getTimeout(void)	const
{
	return (this->timeout);
}

std::string	Server::getHost(void)	const
{
	return (this->host);
}

std::string	Server::getserverName(void)	const
{
	return (this->serverName);
}

std::string	Server::getRootPath(void)	const
{
	return (this->rootPath);
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

const std::vector<uint32_t>&	Server::getPorts(void) const
{
	return (this->ports);
}

bool	Server::getEnableUploads(void) const
{
	return (this->enableUploads);
}

const std::string&	Server::getClientBodyTempPath(void) const
{
	return (this->clientBodyTempPath);
}

int	Server::getSocket() const
{
	return (this->serverSocket);
}

const std::map<std::string, std::string>&	Server::getRedirects(void)	const
{
	return (this->redirects);
}

const std::string&	Server::getCGI(std::string& val)	const
{
	std::map<std::string, std::string>::const_iterator it = this->cgis.find(val);
	if (it != this->cgis.end())
		return (it->second);
	return (*(notFound));
}

const std::map<std::string, std::string>&	Server::getCGIs()	const
{
	return (this->cgis);
}

// SETTERS

void	Server::setPort(uint32_t val)
{
	this->port = val;
}

void	Server::setTimeout(size_t val)
{
	this->timeout = val;
}

void	Server::setHost(std::string& val)
{
	this->host = val;
}

void	Server::setClientBodyTempPath(std::string& val)
{
	this->clientBodyTempPath = val;
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
void	Server::setPorts(uint32_t val)
{
	this->ports.push_back(val);
}

void	Server::setEnableUploads(bool val)
{
	this->enableUploads = val;
}

void	Server::setRootPath(std::string& val)
{
	this->rootPath = val;
}

void	Server::setRedirects(const std::string& key, const std::string& val)
{
	this->redirects[key] = val;
}

void	Server::setCGI(std::string& key, std::string& val)
{
	this->cgis[key] = val;
}

// INET FUNCTIONS

void	Server::initServer(void)
{
	this->ftSocket();
	this->setSocketOptions();
	this->ftBind();
	this->ftListen();
	this->setNonBlocking(this->serverSocket);
}

void	Server::ftSocket(void)
{
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket < 0)
		throw std::runtime_error("Socket creation failed");
}

void	Server::setSocketOptions(void)
{
	int opt = 1;
	if (this->serverSocket < 0 || setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Set socket options REUSEADDR failed");
	if (this->serverSocket < 0 || setsockopt(this->serverSocket, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Set socket options REUSEADDR failed");
}


void	Server::ftBind(void)
{
	struct sockaddr_in addr;

	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->port);
	std::cout << "Host: " << this->host << " len: " << this->host.size() << std::endl;
	addr.sin_addr.s_addr = inet_addr(this->host.c_str());
	if ((this->serverBind = bind(this->serverSocket, (struct sockaddr *)&addr, sizeof(addr))) < 0)
		throw std::runtime_error("Bind failed");
}


void	Server::ftListen(void)
{
	if ((this->serverListenFd = listen(this->serverSocket, LISTEN_BACKLOG)) < 0)
		throw std::runtime_error("Listen failed");
}

void	Server::setNonBlocking(int fd) //! Duplicate code in Webserv.cpp 
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Set non blocking failed");
}
