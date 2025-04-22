/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:00 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/22 14:45:57 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Response.hpp"
#include <iostream>
#include <sys/stat.h>

Response::Response(void)
{}

Response::~Response(void)
{}

Response::Response(const Response& obj)
{
	*this = obj;
}

Response& Response::operator=(const Response& obj)
{
	(void) obj;
	return (*this);
}

void Response::printResHeaders(void) const
{
	std::map<std::string, std::string>::const_iterator it;
	for (it = this->headers.begin(); it != this->headers.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
}

std::string Response::combineResponse(void) {
	std::ostringstream res;

	headers["Date"] = ResponseUtils::getDateTime();
	res << http_version << " " << status_code << " " << status_text << "\r\n";
	std::map<std::string, std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++)
		res << it->first << ": " << it->second << "\r\n";
	res << "\r\n" << body;
	return res.str();
}

Response::Response(struct ClientData &client, Request &req) {
	std::string root_path = "/Users/mboujama/Desktop/webserv/var/www/html";
	std::string full_path = root_path + req.getPath();
	http_version = req.getVersion();

	status_text = "OK";
	body = "<html><body><center><h1>All Is Good</h1></center></body></html>";
	headers["Content-Length"] = std::to_string(body.size());
	headers["Server"] = "NorthServ/1.0";
	headers["Content-Type"] = "text/html";
	headers["Connection"] = "keep-alive";
	headers["Date"] = ResponseUtils::getDateTime();
	
	//TODO: add checkRedirect:
	if (!client.server->getAllowedMethods()[req.getMethod()])
		status_code = METHOD_NOT_ALLOWED;
	else if (!ResponseUtils::pathExists(full_path))
		status_code = NOT_FOUND;
	else if (req.getMethod() == "GET")
		handleGet(client, req, full_path);
	else if (req.getMethod() == "POST")
		handlePost(client, req, full_path);
	else if (req.getMethod() == "DELETE")
		handleDelete(client, req, full_path);

	switch (status_code) {
		case NOT_FOUND:
			std::cout << "NOT FOUND" << std::endl;
			body = "<html><body><center><h1>404 Not Found</h1></center></body></html>";
			headers["Content-Length"] = std::to_string(body.size());
			break ;
		case METHOD_NOT_ALLOWED:
			std::cout << "METHOD NOT ALLOWED" << std::endl;
			body = "<html><body><center><h1>405 Method Not Allowed</h1></center></body></html>";
			headers["Content-Length"] = std::to_string(body.size());
			break ;
		case MOVED_PERMANENTLY:
			std::cout << "MOVED PERMANENTLY" << std::endl;
			body = "<html><body><center><h1>301 Moved Permanently</h1></center></body></html>";
			headers["Content-Length"] = std::to_string(body.size());
			break ;
		default:
			std::cout << "OK" << std::endl;
			body = "<html><body><center><h1>All is good</h1></center></body></html>";
			headers["Content-Length"] = std::to_string(body.size());
			status_code = OK;
	}
	
	std::cout << "Allowed methods => [" << ResponseUtils::allowHeaderValue(client.server->getAllowedMethods()) << "]" << std::endl;
}

void Response::handleGet(struct ClientData &client, Request &req, const std::string &path) {
	// TODO: 1- Check is dir or file
	if (ResponseUtils::isDirectory(path)) {
		// TODO: 2- if directory
		// TODO: 	2.1 - if path doesn't end with /, add it and redirect to /path/ (301)
		std::cout << "this is a directory" << std::endl;
		if (path.at(path.length() - 1) != '/') {
			status_code = MOVED_PERMANENTLY;
			headers["Location"] = req.getPath() + "/";
			return ;
		}
		// TODO:	2.2 - if index file exists serve it (CGI || 200)
		if (true) {
			std::cout << "" << std::endl;
		}
		else {
			status_code = FORBIDDEN;
		}
		// TODO:	2.3 - if autoindex enabled show it (200) otherwise (403)
	}
	else {
		std::cout << "this is a file" << std::endl;
		// TODO: 3- if file serve it (CGI || 200)
	}
	(void) client;
	(void) req;
}

void Response::handlePost(struct ClientData &client, Request &req, const std::string &path) {
	(void) client;
	(void) req;
	(void) path;
	std::cout << "post" << std::endl;
}

void Response::handleDelete(struct ClientData &client, Request &req, const std::string &path) {
	(void) client;
	(void) req;
	(void) path;
	std::cout << "delete" << std::endl;
}
