#pragma once
#include "Types.hpp"
#include "ClientData.hpp"

class Request {
private:
	std::string							method;
	std::string							path;
	std::string							query;
	std::string							version;
	std::string							body;
	std::vector<std::string>			vQuery;
	std::map<std::string, std::string>	headerPairs;
	std::vector<std::string>			vEnv;
public:
	ClientData&							client;
	Request(const std::string& requestString, ClientData& c);
	~Request();

	void		setPath(std::string &newPath);
	void		convertToEnv(void); 
	void 		printHeaders(void) const;
	size_t		getQuerySize(void) const;
	size_t		getEnvSize(void) const;
	std::string getMethod(void) const;
	std::string getPath(void) const;
	std::string getVersion(void) const;
	std::string getHeader(const std::string &key) const;
	std::string getQuery(const size_t i) const;
	std::string getQuery() const;
	std::string getBody(void) const;
	std::string	getEnv(size_t i) const;
};
