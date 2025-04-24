/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:27:49 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/24 08:45:03 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <sys/stat.h>
# include <sstream>
# include <time.h>
# include <map>
# include <sys/fcntl.h>
# include <unistd.h>
# include <dirent.h>
#include <cstdio>
# include <fstream>
# include <cstring>
# include <dirent.h>
# include <cstring>
# include <iostream>
# include <sys/stat.h>

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
};