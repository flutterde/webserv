/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:52:42 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/14 19:08:39 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Cgi.hpp"

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

char **Cgi::createEnvironmentVariables(Request &request, char **systemEnv)
{
	size_t systemEnvCount = 0;
	while (systemEnv[systemEnvCount])
		++systemEnvCount;

	char **envVariables = new char *[systemEnvCount + request.getEnvSize() + 1]; // free this
	size_t index = 0;

	while (systemEnv[index])
	{
		envVariables[index] = strdup(systemEnv[index]);
		++index;
	}

	for (size_t j = 0; !request.getEnv(j).empty(); ++j)
		envVariables[index++] = strdup(request.getEnv(j).c_str());

	envVariables[index - 1] = NULL;
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
	char **envVariables = createEnvironmentVariables(request, systemEnv);
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

	int pipeFd[2];
	if (pipe(pipeFd) < 0)
		std::cerr << "Error: pipe creation failed\n";
	pid_t processId = fork();
	if (processId == 0)
	{
		close(pipeFd[0]);
		dup2(pipeFd[1], STDOUT_FILENO);
		dup2(pipeFd[1], STDERR_FILENO);
		close(pipeFd[1]);

		std::string fullpath = "var/www/html" + request.getPath();
		char *arguments[3] = {strdup(interpreterPath.c_str()), strdup(fullpath.c_str()), NULL};
		execve(arguments[0], arguments, envVariables);
		std::cerr << "Error: execve failed\n";
	}
	else if (processId > 0)
	{
		close(pipeFd[1]);
		char outputBuffer[1024];
		ssize_t bytesRead;
		while ((bytesRead = read(pipeFd[0], outputBuffer, sizeof(outputBuffer) - 1)) > 0)
		{
			outputBuffer[bytesRead] = '\0';
			file += outputBuffer;
		}
		close(pipeFd[0]);
		waitpid(processId, NULL, 0);
		// delete temp files in temp folder
	}
	else
		exit(EXIT_FAILURE);
	// !! waaaaaa abadelaziz
	for(size_t i = 0; envVariables[i]; ++i)
		delete envVariables[i];
	delete[] envVariables;
	return file;
}
