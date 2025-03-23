#include "request.hpp"
#include <unistd.h>
// SCRIPT_FILENAME – hold the executeble file path
// REQUEST_METHOD – hold the method name
// CONTENT_TYPE – e.g., application/json, multipart/form-data
// CONTENT_LENGTH – Number of bytes in the request body
// QUERY_STRING – For GET requests, the URL parameters
// HTTP_ + header name – Standard headers get converted into environment variables
// 		HTTP_USER_AGENT → User-Agent: Mozilla/5.0
// 		HTTP_HOST → Host: example.com

// std::vector<std::string> prepareEnv(Request &r)
// {

// }

// void cgi(Request &r)

char ** makeEnv(Request &r, char **mainEnv)
{
	size_t i = 0;
	while(mainEnv[i]) ++i;
	char **env = new char *[i + r.getEnvSize()]; // free this 
	i = 0;
	while(mainEnv[i])
	{
		env[i] = strdup(mainEnv[i]);
		++i;
	}
	for (size_t j = 0; !r.getEnv(j).empty(); ++j) env[i++] = strdup(r.getEnv(j).c_str());
	env[i] = NULL;
	return env;
}

/// @brief find a path of executed command for scripte like usr/bin/python
char **findPath(Request &r)
{
	std::string extantion = r.getPath().substr(r.getPath().find_last_of("."));

	// if ()

}

void cgi(Request &r, char **mainEnv)
{
	char **env = makeEnv(r, mainEnv);
	// std::cout << "---------------- CGI env: ----------------\n";
	// for (i = 0; env[i]; ++i)
	// 	std::cout << env[i] << std::endl;
	char *path;

	// pid_t pid = fork();
	// if (pid == 0)
	// {

	// }
	// else if (pid == -1)
	// 	exit(EXIT_FAILURE);
}
