/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:43:53 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/26 14:16:42 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Request.hpp"
#include "../../headers/ResponseUtils.hpp"


Request::Request(const std::string &requestString, ClientData& c) :client(c)
{
	std::string requestLine = requestString.substr(0, requestString.find("\r\n", 0));

	size_t methodEnd = requestLine.find(' ');
	this->method = requestString.substr(0, methodEnd);

	size_t pathStart = requestLine.find_first_not_of(' ', methodEnd);
	size_t pathEnd = requestLine.find(' ', pathStart);
	std::string fullPath = requestLine.substr(pathStart, pathEnd - pathStart);

	size_t queryPos = fullPath.find("?");
	if (queryPos != std::string::npos) {
		this->path = fullPath.substr(0, queryPos);
		this->query = fullPath.substr(queryPos + 1);
		size_t queryStart = 0;
		while (queryStart < this->query.length()) {
			size_t queryEnd = this->path.find('&', queryStart);
			if (queryEnd == std::string::npos)
				queryEnd = this->query.length();
			if (queryEnd > queryStart)
				this->vQuery.push_back(this->query.substr(queryStart, queryEnd - queryStart));
			if (queryEnd == this->query.length())
				break;
			queryStart = queryEnd + 1;
		}
	}
	else
		this->path = fullPath;

	size_t versionStart = requestLine.find_last_of(' ', pathEnd) + 1;
	this->version = requestLine.substr(versionStart); // -1 for /r before \n in the request

	std::string headerKey, headerValue;
	size_t headerStart = requestLine.length() + 2;
	size_t headerEnd = headerStart;
	while (true)
	{
		if (headerStart == std::string::npos || requestString[headerStart] == '\r' || requestString[headerStart] == '\n')
			break;
		size_t headerKeyEnd = requestString.find_first_of(':', headerStart);
		if (headerKeyEnd == std::string::npos)
			break;
		headerKey = requestString.substr(headerStart, headerKeyEnd - headerStart);
		size_t headerValueStart = requestString.find_first_not_of(": ", headerKeyEnd);
		headerEnd = requestString.find_first_of("\r\n", headerValueStart);
		headerValue = requestString.substr(headerValueStart, headerEnd - headerValueStart);
		this->headerPairs[headerKey] = headerValue;
		headerStart = requestString.find_first_not_of("\r\n", headerEnd); // I remove +1 for here
	}
	this->body = requestString.substr(headerEnd + 3, requestString.size() - headerEnd);
}


void Request::convertToEnv(void)
{
	vEnv.push_back("REQUEST_METHOD="+ method);
	// vEnv.push_back("REDIRECT_STATUS=1");
	vEnv.push_back("SERVER_NAME=Webserv");
	vEnv.push_back("SERVER_PORT="+ ResponseUtils::toString(client.server->getPort()));
	vEnv.push_back("SCRIPT_FILENAME="+ path);
	vEnv.push_back("GATEWAY_INTERFACE=CGI/1.1");
	vEnv.push_back("SCRIPT_FILENAME="+ client.server->getRootPath() + path); // add the info path
	if (!query.empty())
		vEnv.push_back("QUERY_STRING="+ query);
	if (!headerPairs["Content-Type"].empty())
		vEnv.push_back("CONTENT_TYPE="+ headerPairs["Content-Type"]);
	else
		vEnv.push_back("CONTENT_TYPE=text/html");

	if (!headerPairs["Content-Length"].empty())
		vEnv.push_back("CONTENT_LENGTH="+ headerPairs["Content-Length"]);
	else
		vEnv.push_back("CONTENT_LENGTH=0");

	if (!headerPairs["Host"].empty())
		vEnv.push_back("HTTP_HOST="+ headerPairs["Host"]);
	
	vEnv.push_back(("DOCUMENT_ROOT="+ client.server->getRootPath()));

	if (!headerPairs["User-Agent"].empty())
		vEnv.push_back("HTTP_USER_AGENT="+ headerPairs["User-Agent"]);
	if (!headerPairs["Cookie"].empty())
		vEnv.push_back("HTTP_COOKIE="+ headerPairs["Cookie"]); // is this correct HTTP_COOKIE=session=0c4982e7b7ef3dca ??
	if (!headerPairs["Authorization"].empty())
		vEnv.push_back("HTTP_AUTHORIZATION="+ headerPairs["Authorization"]);
}

std::string Request::getEnv(size_t i) const
{
	if (i >= vEnv.size())
		return "";
	return vEnv[i];
}

std::string Request::getMethod(void) const
{
	return this->method;
}

std::string Request::getBody(void) const
{
	return this->body;
}

std::string Request::getPath(void) const
{
	return this->path;
}

std::string Request::getVersion(void) const
{
	return this->version;
}

std::string Request::getHeader(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator it = headerPairs.find(key);
	if (it != this->headerPairs.end())
		return it->second;
	return "";
}

std::string Request::getQuery(const size_t i) const
{
	if (i >= this->vQuery.size())
		return "";
	return this->vQuery[i];
}

std::string Request::getQuery() const
{
	return this->query;
}

size_t Request::getQuerySize(void) const
{
	return this->vQuery.size();
}

void Request::printHeaders(void) const
{
	std::map<std::string, std::string>::const_iterator it;
	for (it = this->headerPairs.begin(); it != this->headerPairs.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
}

size_t Request::getEnvSize(void) const
{
	return vEnv.size();
}

void Request::setPath(std::string &newPath) {
	path = newPath;
}

Request::~Request() {}
