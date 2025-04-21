/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:40:21 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/21 12:32:47 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/WebservHandler.hpp"
#include <cstddef>
#include <string>

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
			// std::cout << "Removing fd: " << tmp->fd << std::endl;
			_pollfds.erase(tmp);
			_requests.erase(fd);
			break;
		}
	}
	// remove from requests
}

void	WebservHandler::setRequestType(ClientData& client)
{
	printWarning("setRequestType....");
	if (client.headers.empty())
		return;
	if (client.headers.find("Content-Length:") != std::string::npos)
		client.type = CONTENT_LENGTH;
	else if (client.headers.find("Transfer-Encoding: chunked\r\n") != std::string::npos)
		client.type = CHUNKED;
	else if (client.headers.find("Content-Type: multipart/form-data\r\n") != std::string::npos)
		client.type = MULTIPART_FORM;
	else
		client.type = NO_CONTENT;
	std::cout << "=> Request type: " ; printRequestType(client.type); 
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

bool WebservHandler::isChunkedComplete(ClientData& client)
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
		std::cout << "Header complete: \n" << client.headers << std::endl;
		return (true);
	}
	return (false);
}

bool	WebservHandler::isRequestComplete(ClientData& client)
{
	printWarning("isRequestComplete....");
	if (!client.isHeaderComplete)
		return (false);
	else if (client.type == CHUNKED)
		return (isChunkedComplete(client));
	else if (client.type == NO_CONTENT && client.contentLen == -1)
		return (true);
	else if (client.contentLen >= 0 && client.request.size() >= static_cast<size_t>(client.contentLen))
		return (true);
	std::cout << "Request not complete" << std::endl;
	return (false);
}

bool	WebservHandler::isRequestValid(ClientData& client)
{
	size_t max = client.server->getLimitClientBodySize();
	(void)max;
	//! if bad request chunked and content length
	//! if bad request content length and no content
	//! if bad request no content and chunked
	//! if content length more than server limit
	return (true);
}

void	WebservHandler::handleRequest(ClientData& client)
{
	Request req(client.headers + client.request);
	client.response = new Response(client, req); //! free this

	std::string res = client.response->combineResponse();
	// std::string response = res->handleResponse(client, req);
	std::cout << "=======>" << res << "<=======" << std::endl;
	send(client.fd, res.c_str(), res.size(), 0);
}
