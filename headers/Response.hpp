/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:11 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/14 17:44:41 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Types.hpp"
#include "MimeTypes.hpp"
#include "Cgi.hpp"

class Response : public MimeTypes
{
	private:
		Cgi *cgi;
		Response(void);
		Response(const Response& obj);
		Response&	operator=(const Response& obj);
		
		std::string http_version;
		RESPONSE_CODE status_code;
		std::string status_text;
		std::map<std::string, std::string> headers;
		std::string body;
		std::string date;

		void handleGet(struct ClientData &client, Request &req, std::string &path);
		void handlePost(struct ClientData &client, Request &req, std::string &path);
		void handleDelete(struct ClientData &client, Request &req, std::string &path);

		int checkRequestedPath(struct ClientData &client, const std::string &req);
		int checkAllowedMethods(struct ClientData &client, const Request &req);
	public:
		int fd;
		Response(struct ClientData &clientData , Request &request);
		~Response();
		std::string combineResponse();
		int getFd() const;
		std::string getBody() const;
		std::map<std::string, std::string> getHeaders() const;
};	
