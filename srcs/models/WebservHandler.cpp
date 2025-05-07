/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:40:21 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/07 13:41:10 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/WebservHandler.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/fcntl.h>
#include <sys/socket.h>

int WebservHandler::requestCount = 0;

WebservHandler::WebservHandler() {
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
	if (client.headers.empty())
		return;
	if (client.headers.find("Content-Type: multipart/form-data") != std::string::npos)
		client.type = MULTIPART_FORM;
	else if (client.headers.find("Content-Length:") != std::string::npos)
		client.type = CONTENT_LENGTH;
	else if (client.headers.find("Transfer-Encoding: chunked\r\n") != std::string::npos)
		client.type = CHUNKED;
	else
		client.type = NO_CONTENT;
}

void	WebservHandler::setContentLength(ClientData& client)
{
	printWarning("setContentLength....");
	if (!client.isHeaderComplete || client.type == NO_CONTENT || client.type == CHUNKED )
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

bool WebservHandler::isChunkedComplete(ClientData& client) //! will be cancelled
{
	printWarning("isChunkedComplete....");
    if (client.type != CHUNKED)
        return (true);
    if (client.request.size() < 5)
        return false;
    std::string tmp = client.request.substr(client.request.size() - 5);
    if (tmp == "0\r\n\r\n") {
        return true;
    }
    if (tmp.find("\r\n0\r\n") != std::string::npos) {
        std::string trailers = client.request.substr(client.request.find("\r\n0\r\n") + 5);
        if (trailers.find("\r\n") != std::string::npos) {
            return true;
        }
    }
    return false;
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
		client.headers = client.request.substr(0, pos + 4); //! should stop at pos or pos + 4
		client.request = client.request.substr(pos + 4);
		std::cout << "Header complete: \n" << client.headers << std::endl; //! remove this
		this->setBoundary(client);
		client.bodyReded = client.request.size();
		// std::cout << "Body readed: " << client.bodyReded << std::endl;
		return (true);
	}
	return (false);
}

bool	WebservHandler::isRequestComplete(ClientData& client)
{
	printWarning("isRequestComplete....");
	this->isRequestValid(client);
	if (!client.isHeaderComplete)
		return (false);
	else if (client.type == CHUNKED) //! no longer
		return (isChunkedComplete(client));
	else if (client.type == NO_CONTENT && client.contentLen == -1)
		return (true);
	else if (client.type == MULTIPART_FORM && client.contentLen <= static_cast<long>(client.bodyReded))
	{
		std::cout << COL_RED << "client.contentLen: " << client.contentLen << " client.bodyReded: " << client.bodyReded << END_COL << std::endl;
		std::cout << "Multipart form data complete" << std::endl;
		return (true);
	}
	else if (client.contentLen >= 0 && client.request.size() >= static_cast<size_t>(client.contentLen))
		return (true);
	std::cout << "Request not complete" << std::endl;
	return (false);
}

bool	WebservHandler::isRequestValid(ClientData& client)
{
	if (!client.isHeadersChecked)
		this->validateRequestHeaders(client);
	size_t max = client.server->getLimitClientBodySize();
	std::cout << COL_GREEN << "------------------ >> (isRequestValid....) << ----------------" << END_COL << std::endl;
	std::cout << COL_MAGENTA << "Body readed: " << client.bodyReded << " & Max: " << max << std::endl;
	(void)max;
	//! if bad request chunked and content length
	//! if bad request content length and no content
	//! if bad request no content and chunked
	//! if content length more than server limit
	if (client.bodyReded > static_cast<long>(max))
	{
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
	size_t pos = client.headers.find("boundary=");
	if (pos == std::string::npos)
		return;
	size_t start = client.headers.find("=", pos) + 1;
	size_t end = client.headers.find("\r\n", start);
	if (end == std::string::npos)
		return;
	client.boundary = client.headers.substr(start, end - start);
	std::cout << "Boundary: " << client.boundary << std::endl;
}

void	WebservHandler::handleRequest(ClientData& client)
{
	if (!client.error.empty()) {
		send(client.fd, client.error.c_str(), client.error.size(), 0);
		return ;
	}
	std::string	exampleHtml = "<html><body><h1> <center> Welcome to 1337 Webserv </center></h1></body></html>";
	// std::string exampleHtml = "{\"message\": \"File uploaded successfully!\"}";
	std::string response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: " + FtPars::toString(exampleHtml.size()) + "\r\n"
                           "Access-Control-Allow-Origin: *\r\n" // Allow requests from any origin
                           "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n" // Allow specific methods
                           "Access-Control-Allow-Headers: Content-Type, Authorization\r\n" // Allow specific headers
                           "\r\n" + exampleHtml;
	printWarning("handleRequest....");
	std::cout << COL_MAGENTA << "Request: \n" << END_COL << client.request << std::endl;
	send(client.fd, response.c_str(), response.size(), 0); //! MSG_NOSIGNAL (this flag not exist in MACOS)
	this->requestCount++; //! increment request count (Delete this)
	this->_closeClient(client.fd);
}

void	WebservHandler::validateRequestHeaders(ClientData& client)
{
	if (!client.isHeaderComplete)
		return;
	printWarning("validate Request Headers....");
	this->validateUrl(client);
	// client.isHeadersChecked = true;
}

static void httpResponse414(ClientData& client) {
	(void)client;
	std::string response = "HTTP/1.1 414 Request-URI Too Long\r\n"
						   "Content-Type: text/html\r\n"
						   "Content-Length: 59\r\n"
						   "\r\n<html><body><h1>414 Request-URI Too Long</h1></body></html>";
	std::cout << COL_RED << response << END_COL << std::endl;
	client.error = response;
	client.progress = READY;
}

static void httpResponse400(ClientData& client) {
	(void)client;
	std::string response = "HTTP/1.1 400 Bad Request\r\n"
						   "Content-Type: text/html\r\n"
						   "Content-Length: 50\r\n"
						   "\r\n<html><body><h1>400 Bad Request</h1></body></html>";
	std::cout << COL_RED << response << END_COL << std::endl;
	client.error = response;
	client.progress = READY;
}

void	WebservHandler::validateUrl(ClientData& client)
{
	// 414 Request-URI Too Long (more than URL_MAX_SIZE characters)
	size_t start = client.headers.find_first_of("/", 0);
	size_t end = client.headers.find(" HTTP/1.1", start);
	std::cout << "start: " << start << " end: " << end << std::endl;
	if (start == std::string::npos || end == std::string::npos)
		return httpResponse400(client), this->enablePOLLOUT(client.fd);
	std::string url = client.headers.substr(start, end - start);
	if (end - start > URL_MAX_SIZE)
		return httpResponse414(client), this->enablePOLLOUT(client.fd);
	std::cout << "URL: " << url << " Lenght: " << end - start << std::endl;
	// 400 Bad Request url contains invalid characters
	if (url.find_first_not_of(ALLOWED_CHARS) != std::string::npos)
		return httpResponse400(client), this->enablePOLLOUT(client.fd);
}


void WebservHandler::enablePOLLOUT(int fd) {
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		if (_pollfds[i].fd == fd) {
			_pollfds[i].events |= POLLOUT;
			break;
		}
	}
}
