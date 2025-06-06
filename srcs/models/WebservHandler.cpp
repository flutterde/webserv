/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:40:21 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/26 15:45:09 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/header.hpp"
#include "../../headers/HttpErrors.hpp"
#include "../../headers/Response.hpp"



WebservHandler::WebservHandler() {
	_buffer = new char[SEND_SIZE];
	wServ = this;
}

WebservHandler::~WebservHandler() {
	if (_buffer)
		delete[] _buffer;
}

void	WebservHandler::_closeClient(int fd)
{
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
}

void	WebservHandler::setContentLength(ClientData& client)
{
	if (!client.isHeaderComplete || client.contentLen != -1 || client.type == NO_CONTENT)
		return;
	size_t contentLength = client.headers.find("Content-Length: ");
	if (contentLength == std::string::npos)
		return;
	size_t vStart = client.headers.find(" ", contentLength + 15);
	size_t vEnd = client.headers.find("\r\n", vStart);
	std::string value = client.headers.substr(vStart, vEnd - vStart);
	client.contentLen = std::atoi(value.c_str());
}

bool	WebservHandler::isHeaderComplete(ClientData& client)
{
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
	this->isRequestValid(client);
	if (!client.isHeaderComplete)
		return (false);
	if (!client.boundary.empty())
		processMultipartUpload(client); //? ....
	if (client.type == NO_CONTENT && client.contentLen == -1)
		return ((client.progress = COLLECTED), true);
	else if (client.type == MULTIPART_FORM && client.contentLen <= static_cast<long>(client.bodyReded))
	{
		client.isRequestComplete = true;
		return ((client.progress = COLLECTED), true);
	}
	else if (client.contentLen >= 0 && client.request.size() >= static_cast<size_t>(client.contentLen))
		return ((client.progress = COLLECTED), true);
	return (false);
}

bool	WebservHandler::isRequestValid(ClientData& client)
{
	if (!client.isHeadersChecked)
		this->validateRequestHeaders(client);
	size_t max = client.server->getLimitClientBodySize();
	if (client.bodyReded > static_cast<long>(max))
	{
		HttpErrors::httpResponse413(client);
		return (false);
	}
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
	if (!client.error.empty())
		return send(client.fd, client.error.c_str(), client.error.size(), 0), this->_closeClient(client.fd);
	if (!client.resp)
		return this->_closeClient(client.fd);
	if (!client.isHeadersSent) {
		std::string res = client.resp->combineResponse();
		send(client.fd, res.c_str(), res.size(), 0);
		client.isHeadersSent = true;
		return;
	}
	if ((client.bytesSent < client.resp->getContentlength()) && (client.resp->getFd() != -1)) {
		char s__buffer[SEND_SIZE];
		int n = read(client.resp->getFd(), s__buffer, SEND_SIZE - 1);
		if (n <= 0) {
			return this->_closeClient(client.fd);
		}
		s__buffer[n] = '\0';
		int j = send(client.fd, s__buffer, n, 0);
		if (j <= 0 || j != n) {
			return this->_closeClient(client.fd);
		}
		client.bytesSent += j;
	} else
		this->_closeClient(client.fd);
}

void	WebservHandler::validateRequestHeaders(ClientData& client)
{
	if (!client.isHeaderComplete)
		return;
	this->validateUrl(client);
	if (client.type == CHUNKED)
		return HttpErrors::httpResponse400(client), this->enablePOLLOUT(client.fd);
	std::map<std::string, bool>::iterator it = client.server->getAllowedMethods().find(client.method);
	if (it == client.server->getAllowedMethods().end())
		return HttpErrors::httpResponse405(client);
	client.isHeadersChecked = true;
}

void	WebservHandler::validateUrl(ClientData& client)
{
	size_t start = client.headers.find_first_of("/", 0);
	size_t end = client.headers.find(" HTTP/1.1", start);
	if (start == std::string::npos || end == std::string::npos)
		return HttpErrors::httpResponse400(client), this->enablePOLLOUT(client.fd);
	std::string url = client.headers.substr(start, end - start);
	if (end - start > URL_MAX_SIZE)
		return HttpErrors::httpResponse414(client), this->enablePOLLOUT(client.fd);
	if (url.find_first_not_of(ALLOWED_CHARS) != std::string::npos) {
		return HttpErrors::httpResponse400(client), this->enablePOLLOUT(client.fd);
	}
}


void WebservHandler::enablePOLLOUT(int fd) {
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		if (_pollfds[i].fd == fd) {
			_pollfds[i].events |= POLLOUT;
			break;
		}
	}
}
