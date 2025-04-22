/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:29:43 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/22 11:34:46 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ResponseUtils.hpp"
#include <sys/unistd.h>

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