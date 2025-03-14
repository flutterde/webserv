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

Request::Request(const std::string& requestString)
{
	size_t start = requestString.find_first_of(" \t");
	this->method = requestString.substr(0, start);

	// std::cout << "1.start: " << 0 << " end: " << start << std::endl;
	// std::cout << "Method: |" << this->method << "|" << std::endl;

	start = requestString.find_first_not_of(" \t", start);
	size_t end = requestString.find_first_of(" \t", start);
	this->path = requestString.substr(start, end - start);

	// std::cout << "1.start: " << start << " end: " << end << std::endl;
	// std::cout << "Path: |" << this->path << "|" << std::endl;

	start = requestString.find_first_not_of(" \t", end);
	end = requestString.find_first_of("\n", start);
	this->version = requestString.substr(start, end - start - 1); // -1 for /r before \n in the request
	
	// std::cout << "2. start: " << start << " end: " << end << std::endl;
	// std::cout << "Version: |" << this->version << "|" << std::endl;
	std::string key, value;
	while(true)
	{
		start = requestString.find_first_not_of(" \t", end + 1);
		if (start == std::string::npos)
			break;
		end = requestString.find_first_of(":", start);
		if (end == std::string::npos)
			break;
		key = requestString.substr(start, end - start);
		start = requestString.find_first_not_of(": \t", end);
		// if (start == std::string::npos)
		// 	break;
		end = requestString.find_first_of("\n", start);
		// if (end == std::string::npos)
		// 	break;
		value = requestString.substr(start, end - start - 1);

		// std::cout << "key: |" << key << "|" << std::endl; 
		// std::cout << "value: |" << value << "|" << std::endl;

	}
	// you have the problem of body in request
}

std::string Request::getMethod(void) const
{
	return this->method;
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
	std::map<std::string, std::string >::const_iterator it = headerPairs.find(key);
	if (it != this->headerPairs.end())
		return it->second;
	return "";
}

// std::pair<std::string, std::string> Request::getHeader(size_t index) const
// {
// 	if (index >= headerPairs.size())
// 		return std::pair<std::string, std::string>("", "");
// 	std::map<std::string, std::string>::const_iterator it = headerPairs.begin();

// 	std::advance(it, index);
// 	return *it;
// }

Request::~Request(){}