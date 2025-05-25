/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:29:43 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/25 16:54:17 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ResponseUtils.hpp"
#include <dirent.h>
#include <iostream>
#include <unistd.h>

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
	if (!dir) {
		std::cout << COL_MAGENTA << "Can't open directory in 'isIndexFileExist'" << END_COL << std::endl;
		return "";
	}
	while ((ent = readdir(dir))) {
		if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
			continue ;
		if ((it = indexes.find(ent->d_name)) != indexes.end())
			return closedir(dir), it->first;
	}
	return closedir(dir), "";
}

std::string ResponseUtils::getErrorPage(RESPONSE_CODE status) {
	std::string content;
	std::string errorsPath = "var/www/html/errors/";

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

std::string ResponseUtils::generateAutoIndex(std::string filepath) {
	std::stringstream body;
	DIR *dir;
	struct dirent *ent;
	bool first = true;
	
	body << "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>" << filepath << "</title><style>*{margin:0;padding:0;box-sizing:border-box;font-family:sans-serif;}body{background-color:rgb(23, 43, 61);color:black;padding:20px;}.container{background-color:rgb(229,221,221);padding:10px;border-radius:3px;max-width:650px;margin: 0 auto}hr{margin:10px 0}</style></head><body><div class='container'>";

	dir = opendir(filepath.c_str());
	if (!dir)
		return "";
	while ((ent = readdir(dir))) {
		if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
			continue ;
		if (!first)
			body << "<hr/>";
		body << "<div class='directory'>" 
				"<a href='" << ent->d_name << "'>" << ent->d_name << "</a>" 
			"</div>" << std::endl;
		first = false;
	}
	closedir(dir);
	if (first)
		body << "<center><h2>This directory is empty</h2></center>";
	body << "</div></body></html>";
	return body.str();
}

bool    ResponseUtils::deleteFile(const std::string& path)
{
    if (remove(path.c_str()) != 0) {
        std::cerr << "Error deleting file: " << path << std::endl;
		return false;
	}
	return true;
}

bool ResponseUtils::deleteFolder(const std::string& path)
{
    DIR* dir = opendir(path.c_str());

    if (!dir) {
        std::cout << COL_MAGENTA << "Can't open directory in 'deleteFolder'" << END_COL << std::endl;
        return false;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
            continue;
        std::string filePath = path + "/" + entry->d_name;
        if (entry->d_type == DT_DIR)
            deleteFolder(filePath);
        else
            deleteFile(filePath);
    }
    closedir(dir);
    if (remove(path.c_str()) != 0) {
        std::cerr << "Error deleting directory: " << path << std::endl;
		return (false);
	}
	return true;
}

std::string ResponseUtils::getAllowHeader(std::map<std::string, bool> allowed) {
	std::string ret;
	bool first = true;
	std::map<std::string, bool>::iterator it;
	
	for (it = allowed.begin(); it != allowed.end(); it++) {
		if (!first)
			ret.append(", ");
		ret.append(it->first);
		first = false;
	}
	return ret;
}
