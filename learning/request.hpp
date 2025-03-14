#pragma once

#include <iostream>
#include <string>
#include <map>

class Request {
private:
	std::string							method;
	std::string							path;
	std::string							version;
	std::map<std::string, std::string>	headerPairs;

public:
	Request( const std::string& requestString);
	std::string getMethod(void) const;
	std::string getPath(void) const;
	std::string getVersion(void) const;
	std::string getHeader(const std::string &key) const;
	// std::pair<std::string, std::string> getHeader(size_t index) const;
	~Request();
};
