#include "request.hpp"

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

Request::Request(const std::string &requestString)
{
	size_t start = requestString.find_first_of(" \t");
	this->method = requestString.substr(0, start);

	start = requestString.find_first_not_of(" \t", start);
	size_t end = requestString.find_first_of(" \t", start);
	this->path = requestString.substr(start, end - start);
	
	if (this->path.find_first_of("?") != std::string::npos)
	{
		size_t q_start = this->path.find_first_of("?") + 1;
		while (true)
		{
			size_t q_end = this->path.find_first_of("&", q_start);
			if (q_end != q_start)
				this->query.push_back(this->path.substr(q_start, q_end - q_start));
			if (q_end == std::string::npos)
				break ;
			q_start = ++q_end;
		}
		this->path = this->path.substr(0, this->path.find_first_of("?"));
	}

	start = requestString.find_last_of(" \t", end) + 1;
	end = requestString.find_first_of("\n", start);
	this->version = requestString.substr(start, end - start - 1); // -1 for /r before \n in the request

	std::string key, value;
	while (true)
	{
		start = requestString.find_first_not_of(" \t", end + 1);
		if (start == std::string::npos || requestString[start] == '\r' || requestString[start] == '\n')
			break;
		end = requestString.find_first_of(":", start);
		if (end == std::string::npos)
			break;
		key = requestString.substr(start, end - start);
		start = requestString.find_first_not_of(": \t", end);
		end = requestString.find_first_of("\n", start);
		value = requestString.substr(start, end - start);
		this->headerPairs[key] = value;
	}
	this->body = requestString.substr(start + 2, requestString.size() - start);
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
	if (i >= this->query.size())
		return "";
	return this->query[i];
}

size_t Request::getQuerySize(void) const
{
	return this->query.size();
}

void Request::printHeaders(void) const
{
	std::map<std::string, std::string>::const_iterator it;
	for (it = this->headerPairs.begin(); it != this->headerPairs.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
}

Request::~Request() {}
