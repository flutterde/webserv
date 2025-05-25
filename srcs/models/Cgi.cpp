/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/25 15:39:09 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../headers/Cgi.hpp"
#include <stdexcept>
 #include <sys/wait.h> //!
#include <unistd.h>

// SCRIPT_FILENAME – hold the executeble file path
// REQUEST_METHOD – hold the method name
// CONTENT_TYPE – e.g., application/json, multipart/form-data
// CONTENT_LENGTH – Number of bytes in the request body
// QUERY_STRING – For GET requests, the URL parameters
// HTTP_ + header name – Standard headers get converted into environment variables
// 		HTTP_USER_AGENT → User-Agent: Mozilla/5.0
// 		HTTP_HOST → Host: example.com

Cgi::Cgi() {}

Cgi::~Cgi() {}

char **Cgi::createEnvironmentVariables(Request &request)
{
	size_t index = 0;
	char **envVariables = new char *[request.getEnvSize() + 1];

	while (index < request.getEnvSize()){
		envVariables[index] = strdup(request.getEnv(index).c_str());
		++index;
	}

	envVariables[index] = NULL;
	return envVariables;
}

std::vector<std::string> Cgi::extractBinaryPaths(char **environmentVariables)
{
	std::vector<std::string> binaryPaths;
	for (size_t i = 0; environmentVariables[i]; ++i)
	{
		std::string envEntry = environmentVariables[i];
		if (envEntry.compare(0, 5, "PATH=") == 0)
		{
			size_t startPos = 5;
			size_t delimiterPos;
			while (true)
			{
				delimiterPos = envEntry.find_first_of(":", startPos);
				if (delimiterPos == std::string::npos)
					break;
				binaryPaths.push_back(envEntry.substr(startPos, delimiterPos - startPos));
				startPos = delimiterPos + 1;
			}
			binaryPaths.push_back(envEntry.substr(startPos));
			break;
		}
	}
	return binaryPaths;
}

std::string Cgi::locateExecutable(const std::vector<std::string> &searchPaths, const std::string &executableName)
{
	for (size_t i = 0; i < searchPaths.size(); ++i)
	{
		std::string fullPath = searchPaths[i] + "/" + executableName;
		if (access(fullPath.c_str(), X_OK) == 0)
			return fullPath;
	}
	return "";
}

std::string Cgi::executeCgiScript(Request &request, char **systemEnv)
{
	request.convertToEnv();
	char **envVariables = createEnvironmentVariables(request);
	std::vector<std::string> binaryPaths = extractBinaryPaths(systemEnv);
	std::string scriptExtension;
	std::string interpreterPath;
	size_t extensionPos;
	std::string file;
	extensionPos = request.getPath().find_last_of(".");
	if (extensionPos != std::string::npos)
		scriptExtension = request.getPath().substr(extensionPos);
	else
		scriptExtension = "";
	if (scriptExtension == ".php")
		interpreterPath = locateExecutable(binaryPaths, "php");
	else if (scriptExtension == ".py")
		interpreterPath = locateExecutable(binaryPaths, "python3");
	// Multiple CGI Not tested
	else {
		interpreterPath = request.client.server->getCGI(scriptExtension);
		if(interpreterPath.empty()){
			return ""; //! is this return value valid ?
		}
	}

	int stdoutPipe[2], stdinPipe[2];
	if (pipe(stdoutPipe) < 0)
		std::cerr << COL_RED << "Error: pipe stdout creation failed\n" << END_COL;
	if (pipe(stdinPipe) < 0){
		std::cerr << COL_RED << "Error: pipe stdin creation failed\n" << END_COL;
		close(stdoutPipe[0]);
		close(stdoutPipe[1]);
	}
	pid_t processId = fork();
	if (processId == 0)
	{
		dup2(stdinPipe[0], STDIN_FILENO);

		dup2(stdoutPipe[1], STDOUT_FILENO);
		// dup2(stdoutPipe[1], STDERR_FILENO);
		
		close(stdoutPipe[0]);
		close(stdoutPipe[1]);
		close(stdinPipe[0]);
		close(stdinPipe[1]);

		std::string fullpath = request.client.server->getRootPath() + request.getPath();
		char *arguments[3] = {strdup(interpreterPath.c_str()), strdup(fullpath.c_str()), NULL};
		execve(arguments[0], arguments, envVariables);
		std::cerr << "Error: execve failed\n";
	}
	else if (processId > 0)
	{
		close(stdinPipe[0]);
		close(stdoutPipe[1]);

		if (request.getMethod() == "POST" && !request.getBody().empty()) {
            std::string postData = request.getBody();
            ssize_t bytesWritten = write(stdinPipe[1], postData.c_str(), postData.length());
            if (bytesWritten < 0) {
                std::cerr << COL_RED << "Error: Failed to write POST data to CGI\n" << END_COL;
            }
        }

		close(stdinPipe[1]);

		char outputBuffer[1024];
		ssize_t bytesRead;
		while ((bytesRead = read(stdoutPipe[0], outputBuffer, sizeof(outputBuffer) - 1)) > 0)
		{
			outputBuffer[bytesRead] = '\0';
			file += outputBuffer;
		}
		
		close(stdoutPipe[0]);
		waitpid(processId, NULL, 0);
	}
	else
	{
		close(stdoutPipe[0]);
		close(stdoutPipe[1]);
		close(stdinPipe[0]);
		close(stdinPipe[1]);
		for(size_t i = 0; envVariables[i]; ++i)
            delete[] envVariables[i];
        delete[] envVariables;
        return "";
	}
	for(size_t i = 0; envVariables[i]; ++i)
		delete envVariables[i];
	delete[] envVariables;
	return file;
}
