/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:43:53 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/20 12:43:55 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Request.hpp"

// GET /favicon.ico HTTP/1.1
// Host: 127.0.0.1:8080
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:123.0) Gecko/20100101 Firefox/123.0
// Accept: image/avif,image/webp,*/*
// Accept-Language: en-US,en;q=0.5
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive
// Referer: http://127.0.0.1:8080/
// Sec-Fetch-Dest: image
// Sec-Fetch-Mode: no-cors
// Sec-Fetch-Site: same-origin

Request::Request(const std::string &requestString, ClientData& c) :client(c)
{
	size_t methodEnd = requestString.find_first_of(" \t");
	this->method = requestString.substr(0, methodEnd);

	size_t pathStart = requestString.find_first_not_of(" \t", methodEnd);
	size_t pathEnd = requestString.find_first_of(" \t", pathStart);
	this->path = requestString.substr(pathStart, pathEnd - pathStart);
	
	if (this->path.find_first_of("?") != std::string::npos)
	{
		this->query = this->path.substr(this->path.find_first_of("?"));
		size_t queryStart = this->path.find_first_of("?") + 1;
		while (true)
		{
			size_t queryEnd = this->path.find_first_of("&", queryStart);
			if (queryEnd != queryStart)
				this->vQuery.push_back(this->path.substr(queryStart, queryEnd - queryStart));
			if (queryEnd == std::string::npos)
				break;
			queryStart = ++queryEnd;
		}
		this->path = this->path.substr(0, this->path.find_first_of("?"));
	}

	size_t versionStart = requestString.find_last_of(" \t", pathEnd) + 1;
	size_t versionEnd = requestString.find_first_of("\n", versionStart);
	this->version = requestString.substr(versionStart, versionEnd - versionStart - 1); // -1 for /r before \n in the request

	std::string headerKey, headerValue;
	while (true)
	{
		size_t headerStart = requestString.find_first_not_of(" \t", versionEnd + 1);
		if (headerStart == std::string::npos || requestString[headerStart] == '\r' || requestString[headerStart] == '\n')
			break;
		size_t headerKeyEnd = requestString.find_first_of(":", headerStart);
		if (headerKeyEnd == std::string::npos)
			break;
		headerKey = requestString.substr(headerStart, headerKeyEnd - headerStart);
		size_t headerValueStart = requestString.find_first_not_of(": \t", headerKeyEnd);
		versionEnd = requestString.find_first_of("\n", headerValueStart);
		headerValue = requestString.substr(headerValueStart, versionEnd - headerValueStart);
		this->headerPairs[headerKey] = headerValue;
	}
	this->body = requestString.substr(versionEnd + 2, requestString.size() - versionEnd);
	// this->convertToEnv();
}


template <typename T>
void	printMap(T mp) {
	for (typename T::iterator it = mp.begin(); it != mp.end(); ++it){
		std::cout << "->" << *it << std::endl;	
	}
}
void Request::convertToEnv(void)
{
	vEnv.push_back("REQUEST_METHOD="+ method);
	vEnv.push_back("SERVER_NAME=Webserv");
	vEnv.push_back("SERVER_PORT="+ std::to_string(client.server->getPort()));
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
	
	if (!headerPairs["User-Agent"].empty())
		vEnv.push_back("HTTP_USER_AGENT="+ headerPairs["User-Agent"]);
	if (!headerPairs["Cookie"].empty())
		vEnv.push_back("HTTP_COOKIE="+ headerPairs["Cookie"]); // is this correct HTTP_COOKIE=session=0c4982e7b7ef3dca??
	if (!headerPairs["Authorization"].empty())
		vEnv.push_back("HTTP_AUTHORIZATION="+ headerPairs["Authorization"]);
	printMap(vEnv); // remove this
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
