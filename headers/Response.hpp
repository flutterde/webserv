/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:11 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/23 14:50:21 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include "../learning/request.hpp"
# include "./WebservHandler.hpp"
# include "./ResponseUtils.hpp"
# include "./MimeTypes.hpp"
# include <dirent.h>
#include <cstddef>
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <string>
#include <sys/_types/_ssize_t.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include <unistd.h>

class Response : public MimeTypes
{
	private:
		Response(void);
		Response(const Response& obj);
		Response&	operator=(const Response& obj);
		
		std::string http_version;
		RESPONSE_CODE status_code;
		std::string status_text;
		std::map<std::string, std::string> headers;
		std::string body;
		std::string date;
		int fd;

		int getFd() const;
		void handleGet(struct ClientData &client, Request &req, const std::string &path);
		void handlePost(struct ClientData &client, Request &req, const std::string &path);
		void handleDelete(struct ClientData &client, Request &req, const std::string &path);

		int checkRequestedPath(struct ClientData &client, const std::string &req);
		int checkAllowedMethods(struct ClientData &client, const Request &req);
	public:
		Response(struct ClientData &clientData , Request &request);
		~Response();
		std::string combineResponse();
};	
