/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:29:43 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/24 08:34:52 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ResponseUtils.hpp"


std::string ResponseUtils::getDateTime() {
	time_t rawtime;
	struct tm * datetime;
	char buffer[80];

	time(&rawtime);
	datetime = localtime(&rawtime);

	strftime (buffer, 80, "%a, %d %h %Y %T", datetime);
	return std::string(buffer);
}

std::string ResponseUtils::allowHeaderValue(std::map<std::string, bool> allowedMethods) {
	std::ostringstream oss;
	bool first = true;

	std::map<std::string, bool>::iterator it;
	for (it = allowedMethods.begin(); it != allowedMethods.end(); it++) {
		if (it->second) {
			if (!first) oss << ", ";
			oss << it->first;
			first = false;
		}
	}
	return oss.str();
}

bool ResponseUtils::pathExists(const std::string& path) {
	std::cout << "Path to search: " << path << std::endl;
    return access(path.c_str(), F_OK) == 0;
}

bool ResponseUtils::isDirectory(const std::string& path) {
	struct stat info;

	stat(path.c_str(), &info);
	return S_ISDIR(info.st_mode);
}

int ResponseUtils::openFile(const std::string& filepath) {
	int fd = open(filepath.c_str(), O_RDONLY);
	// ? What is the file didn't open ?

	return fd;
}

std::string ResponseUtils::toString(long value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

std::string ResponseUtils::isIndexFileExist(std::map<std::string, bool> &indexes, const std::string &path) {
	DIR *dir;
	struct dirent *ent;
	std::map<std::string, bool>::iterator it;

	dir = opendir(path.c_str());
	while ((ent = readdir(dir))) {
		if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
			continue ;
		if ((it = indexes.find(ent->d_name)) != indexes.end())
			return it->first;
	}
	return "";
}

std::string ResponseUtils::getErrorPage(RESPONSE_CODE status) {
	std::string content;
	std::string errorsPath = "/Users/mboujama/Desktop/webserv/var/www/html/errors/";

	std::ifstream file;
	switch (status) {
		case FORBIDDEN:
			file.open((errorsPath + std::string("403.html")).c_str());
			break;
		case NOT_FOUND:
			file.open((errorsPath + std::string("404.html")).c_str());
			break;
		case METHOD_NOT_ALLOWED:
			file.open((errorsPath + std::string("405.html")).c_str());
			break;
		case INTERNAL_SERVER_ERROR:
			file.open((errorsPath + std::string("500.html")).c_str());
			break;
		default:
			content = "Under control";
	}

	if (file.is_open()) {
		std::string line;

		while (file) {
			std::getline(file, line);
			content.append(line);
		}
	}
	else
		std::cout << "File doesn't opened" << std::endl;

	return content;
}
