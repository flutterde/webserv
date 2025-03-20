#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Request {
private:
	std::string							method;
	std::string							path;
	std::vector<std::string>			query;
	std::string							version;
	std::map<std::string, std::string>	headerPairs;
	std::string							body;

public:
	Request( const std::string& requestString);
	std::string getMethod(void) const;
	std::string getPath(void) const;
	std::string getVersion(void) const;
	std::string getHeader(const std::string &key) const;
	std::string getQuery(const size_t i) const;
	size_t		getQuerySize(void) const;
	std::string getBody(void) const;
	void 		printHeaders(void) const;
	// std::pair<std::string, std::string> getHeader(size_t index) const;
	~Request();
};
