/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:27:49 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/12 09:04:24 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <sys/stat.h>
# include <sstream>
# include <time.h>
# include <map>
# include <sys/fcntl.h>
# include <unistd.h>
# include <fstream>
# include <iostream>
# include <dirent.h>
# include <string>

enum RESPONSE_CODE {
	NOT_FOUND = 404,
	MOVED_PERMANENTLY = 301,
	METHOD_NOT_ALLOWED = 405,
	FORBIDDEN = 403,
	OK = 200,
	CGI,
	CREATED = 201,
	CONFLICT = 409,
	NOCONTENT = 204,
	INTERNAL_SERVER_ERROR = 500
};

class ResponseUtils {
    public:
        static std::string getDateTime();
        static std::string allowHeaderValue(std::map<std::string, bool> allowedMethods);
        static bool pathExists(const std::string& path);
        static bool isDirectory(const std::string& path);
        static int openFile(const std::string& filepath);
        static std::string toString(long value);
        static std::string isIndexFileExist(std::map<std::string, bool> &indexes, const std::string &path);
        static std::string getErrorPage(RESPONSE_CODE status);
        static std::string getMimeType(std::string filepath);
        static std::string generateAutoIndex(std::string filepath);
        static bool    deleteFile(const std::string& path);
        static bool    deleteFolder(const std::string& path);
};