#include "request.hpp"
#include <unistd.h>
#include <algorithm>

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

char **makeEnv(Request &r, char **mainEnv)
{
	size_t i = 0;
	while (mainEnv[i])
		++i;
	char **env = new char *[i + r.getEnvSize()]; // free this
	i = 0;
	while (mainEnv[i])
	{
		env[i] = strdup(mainEnv[i]);
		++i;
	}
	for (size_t j = 0; !r.getEnv(j).empty(); ++j)
		env[i++] = strdup(r.getEnv(j).c_str());
	env[i] = NULL;
	return env;
}

std::vector<std::string> splitPath(char **mainEnv)
{
	std::vector<std::string> binPath;
	for (size_t i = 0; mainEnv[i]; ++i)
	{
		std::string tmp = mainEnv[i];
		if (tmp.compare(0, 5, "PATH=") == 0)
		{
			size_t start = 5;
			size_t end;
			while (true)
			{
				end = tmp.find_first_of(":", start);
				if (end == std::string::npos)
					break;
				binPath.push_back(tmp.substr(start, end - start));
				start = end + 1;
			}
			binPath.push_back(tmp.substr(start));
			break;
		}
	}
	return binPath;
}

std::string findExecutablePath(const std::vector<std::string> &binPath, const std::string &executable)
{
	for (size_t i = 0; i < binPath.size(); ++i)
	{
		std::string path = binPath[i] + "/" + executable;
		if (access(path.c_str(), X_OK) == 0)
			return path;
	}
	return "";
}

void cgi(Request &r, char **mainEnv)
{
	char **env = makeEnv(r, mainEnv);
	std::vector<std::string> binPath = splitPath(mainEnv);
	std::string fileExtension;
	std::string executablePath;
	size_t t;
	t = r.getPath().find_last_of(".");
	if (t != std::string::npos)
		fileExtension = r.getPath().substr(t);
	else
		fileExtension = "";
	if (fileExtension == ".php")
		executablePath = findExecutablePath(binPath, "php");
	else if (fileExtension == ".py")
		executablePath = findExecutablePath(binPath, "python3");

	int pip[2];
	if (pipe(pip) < 0)
		std::cerr << "pipe failed\n";
	pid_t pid = fork();
	if (pid == 0)
	{
		close(pip[0]);
		dup2(pip[1], STDOUT_FILENO);
		dup2(pip[1], STDERR_FILENO);
		close(pip[1]);
		char *args[3] = {strdup(executablePath.c_str()), strdup(r.getPath().c_str()), NULL};
		execve(args[0], args, env);
		std::cerr << "Error: execve failed\n";
	}
	else if (pid > 0)
	{
		close(pip[1]);
		char buffer[1024];
		ssize_t bytesRead;
		while ((bytesRead = read(pip[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytesRead] = '\0';
			std::cout << buffer;
		}
		close(pip[0]);
		waitpid(pid, NULL, 0);
	}
	else
		exit(EXIT_FAILURE);
}
