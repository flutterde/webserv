/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:40:21 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/12 18:04:08 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/WebservHandler.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/fcntl.h>
#include <sys/socket.h>

WebservHandler::WebservHandler() {
	wServ = this;
}

WebservHandler::~WebservHandler() {
}

void	WebservHandler::_closeClient(int fd)
{
	std::cout << COL_YELLOW << "Closing client fd: " << fd << END_COL << std::endl;
	// remove from pollfds
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		if (_pollfds[i].fd == fd) {
			close(fd);
			std::vector<struct pollfd>::iterator tmp = _pollfds.begin() + i;
			_pollfds.erase(tmp);
			_requests.erase(fd);
			break;
		}
	}
}

void	WebservHandler::setRequestType(ClientData& client)
{
	printWarning("setRequestType....");
	if (client.headers.empty() || client.type != NOT_SET)
		return;
	else if (client.method != "POST")
		client.type = NO_CONTENT;
	else if (client.headers.find("Content-Type: multipart/form-data") != std::string::npos)
		client.type = MULTIPART_FORM;
	else if (client.headers.find("Content-Length:") != std::string::npos)
		client.type = CONTENT_LENGTH;
	else if (client.headers.find("Transfer-Encoding: chunked\r\n") != std::string::npos)
		client.type = CHUNKED;
	else
		client.type = NO_CONTENT;
	std::cout << COL_BLUE << "=> Request Type: " << client.type << END_COL << std::endl;
}

void	WebservHandler::setContentLength(ClientData& client)
{
	printWarning("setContentLength....");
	if (!client.isHeaderComplete || client.contentLen != -1 || client.type == NO_CONTENT)
		return;
	size_t contentLength = client.headers.find("Content-Length: ");
	if (contentLength == std::string::npos)
		return;
	size_t vStart = client.headers.find(" ", contentLength + 15);
	size_t vEnd = client.headers.find("\r\n", vStart);
	std::string value = client.headers.substr(vStart, vEnd - vStart);
	client.contentLen = std::atoi(value.c_str());
	std::cout << "=> Content-Length: " << client.contentLen << std::endl;
}

bool	WebservHandler::isHeaderComplete(ClientData& client)
{
	printWarning("isHeaderComplete....");
	size_t	pos;
	if (client.isHeaderComplete)
		return (true);
	pos = client.request.find("\r\n\r\n");
	if (pos != std::string::npos) {
		client.isHeaderComplete = true;
		client.headers = client.request.substr(0, pos + 4);
		client.request = client.request.substr(pos + 4);
		client.bodyReded = client.request.size();
		client.progress = WORKING;
		this->setMethod(client);
		return (true);
	}
	return (false);
}

bool	WebservHandler::isRequestComplete(ClientData& client)
{
	printWarning("is Request Complete....");
	this->isRequestValid(client);
	if (!client.isHeaderComplete)
		return (false);
	processMultipartUpload(client); //? ....
	if (client.type == NO_CONTENT && client.contentLen == -1)
		return ((client.progress = COLLECTED), true);
	else if (client.type == MULTIPART_FORM && client.contentLen <= static_cast<long>(client.bodyReded))
	{
		client.isRequestComplete = true;
		std::cout << COL_RED << "Multipart form data complete" <<  END_COL << std::endl;
		return ((client.progress = COLLECTED), true);
	}
	else if (client.contentLen >= 0 && client.request.size() >= static_cast<size_t>(client.contentLen))
		return ((client.progress = COLLECTED), true);
	return (false);
}

bool	WebservHandler::isRequestValid(ClientData& client)
{
	printWarning("isRequestValid....");
	if (!client.isHeadersChecked)
		this->validateRequestHeaders(client);
	size_t max = client.server->getLimitClientBodySize();
	(void)max;
	//! if bad request chunked and content length
	//! if bad request content length and no content
	//! if bad request no content and chunked
	//! if content length more than server limit
	if (client.bodyReded > static_cast<long>(max))
	{
		HttpErrors::httpResponse413(client);
		std::cout << COL_RED << "Request body size exceeds server limit" << END_COL << std::endl;
		return (false);
	}
	std::cout << COL_GREEN << "------------------ >> (Request Valid....) << ----------------" << END_COL << std::endl;
	return (true);
}

void	WebservHandler::setBoundary(ClientData& client)
{
	if (!client.isHeaderComplete || client.type != MULTIPART_FORM || !client.boundary.empty())
		return;
	std::cout << COL_MAGENTA << "setBoundary...." << END_COL << std::endl;
	size_t pos = client.headers.find("boundary=");
	if (pos == std::string::npos)
		return;
	size_t start = client.headers.find("=", pos) + 1;
	size_t end = client.headers.find("\r\n", start);
	if (end == std::string::npos)
		return;
	client.boundary = client.headers.substr(start, end - start);
	std::cout << "*>> Boundary: " << client.boundary << std::endl;
}

void	WebservHandler::setMethod(ClientData& client)
{
	if (!client.isHeaderComplete || !client.method.empty())
		return;
	size_t pos = client.headers.find(" ");
	if (pos == std::string::npos)
		return;
	client.method = client.headers.substr(0, pos);
}

void	WebservHandler::handleRequest(ClientData& client)
{
	if (!client.error.empty()) {
		send(client.fd, client.error.c_str(), client.error.size(), 0);
		this->_closeClient(client.fd);
		return ;
	}
	std::string	exampleHtml = "<html><body><h1> <center> Welcome to 1337 | testing Webserv </center></h1></body></html>";
	std::string response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: " + FtPars::toString(exampleHtml.size()) + "\r\n"
                           "Access-Control-Allow-Origin: *\r\n" 
                           "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                           "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
                           "\r\n" + exampleHtml;
	printWarning("handleRequest....");
	send(client.fd, response.c_str(), response.size(), 0); //! MSG_NOSIGNAL (this flag not exist in MACOS)
	this->_closeClient(client.fd); //! Close client connection only if the response is sent
}

void	WebservHandler::validateRequestHeaders(ClientData& client)
{
	if (!client.isHeaderComplete)
		return;
	printWarning("validate Request Headers....");
	this->validateUrl(client);
	if (client.type == CHUNKED)
		return HttpErrors::httpResponse400(client), this->enablePOLLOUT(client.fd);
	client.isHeadersChecked = true;
}

void	WebservHandler::validateUrl(ClientData& client)
{
	// 414 Request-URI Too Long (more than URL_MAX_SIZE characters)
	size_t start = client.headers.find_first_of("/", 0);
	size_t end = client.headers.find(" HTTP/1.1", start);
	if (start == std::string::npos || end == std::string::npos)
		return HttpErrors::httpResponse400(client), this->enablePOLLOUT(client.fd);
	std::string url = client.headers.substr(start, end - start);
	if (end - start > URL_MAX_SIZE)
		return HttpErrors::httpResponse414(client), this->enablePOLLOUT(client.fd);
	// 400 Bad Request url contains invalid characters
	if (url.find_first_not_of(ALLOWED_CHARS) != std::string::npos)
		return HttpErrors::httpResponse400(client), this->enablePOLLOUT(client.fd);
}


void WebservHandler::enablePOLLOUT(int fd) {
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		if (_pollfds[i].fd == fd) {
			_pollfds[i].events |= POLLOUT;
			break;
		}
	}
}
