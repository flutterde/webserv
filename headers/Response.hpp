/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:11 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/21 13:55:05 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include "../learning/request.hpp"
# include "./WebservHandler.hpp"
# include "./ResponseUtils.hpp"
# include <dirent.h>

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

class Response
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
		
		void printResHeaders(void) const;
		
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
