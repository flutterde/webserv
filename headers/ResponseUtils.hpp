/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:27:49 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/12 14:48:09 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Types.hpp"

class ResponseUtils {
    public:
        static int			openFile(const std::string& filepath);
        static bool         pathExists(const std::string& path);
        static bool			isDirectory(const std::string& path);
        static bool			deleteFile(const std::string& path);
        static bool			deleteFolder(const std::string& path);
        static std::string  getDateTime();
        static std::string  allowHeaderValue(std::map<std::string, bool> allowedMethods);
        static std::string	toString(long value);
        static std::string	isIndexFileExist(std::map<std::string, bool> &indexes, const std::string &path);
        static std::string	getErrorPage(RESPONSE_CODE status);
        static std::string	getMimeType(std::string filepath);
        static std::string	generateAutoIndex(std::string filepath);
        static std::string	getAllowHeader(const std::map<std::string, bool> allowed);
};