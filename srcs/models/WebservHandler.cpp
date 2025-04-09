/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:40:21 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/09 20:49:21 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/WebservHandler.hpp"
#include <cstddef>
#include <string>

WebservHandler::WebservHandler() {
}

WebservHandler::~WebservHandler() {
}

void	WebservHandler::setRequestType(ClientData& client)
{
	if (client.headers.empty())
		return;
	if (client.headers.find("Content-Length:") != std::string::npos)
		client.type = CONTENT_LENGTH;
	else if (client.headers.find("Transfer-Encoding: chunked\r\n") != std::string::npos)
		client.type = CHUNCKED;
	else if (client.headers.find("Content-Type: multipart/form-data\r\n") != std::string::npos)
		client.type = MULTIPART_FORM;
	else
		client.type = NO_CONTENT;
}

void	WebservHandler::setContentLength(ClientData& client)
{
	if (!client.isHeaderComplete || client.type == NO_CONTENT || client.type == CHUNCKED )
		return;
	size_t contentLength = client.headers.find("Content-Length: ");
	if (contentLength == std::string::npos)
		return;
	size_t vStart = client.headers.find(" ", contentLength + 15);
	size_t vEnd = client.headers.find("\r\n", vStart);
	std::string value = client.headers.substr(vStart, vEnd - vStart);
	client.contentLen = std::atoi(value.c_str());
}

bool	WebservHandler::isChunkedComplete(ClientData& client)
{
	if (client.type != CHUNCKED)
		return (true);
	if (client.request.size() < 9) //!
		return (false);
	std::string	tmp = client.request.substr(client.request.size() - 9);
	if (tmp.find("\r\n0\r\n\r\n") != std::string::npos)
		return (true);
	return (false);
}

bool	WebservHandler::isHeaderComplete(ClientData& client)
{
	size_t	pos;
	if (client.isHeaderComplete)
		return (true);
	pos = client.request.find("\r\n\r\n");
	if (pos != std::string::npos) {
		client.isHeaderComplete = true;
		client.headers = client.request.substr(0, pos + 4); //! should stop at pos or pos + 4
		client.request = client.request.substr(pos + 4);
		return (true);
	}
	return (false);
}

bool	WebservHandler::isRequestComplete(ClientData& client)
{
	if (!client.isHeaderComplete)
		return (false);
	else if (client.type == CHUNCKED)
		return (isChunkedComplete(client));
	else if (client.contentLen >= 0 && client.request.size() >= static_cast<size_t>(client.contentLen))
		return (true);
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

