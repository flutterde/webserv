/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:00 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/24 08:37:30 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Response.hpp"


Response::Response(void) {}

Response::~Response(void) {}

Response::Response(const Response& obj){
	*this = obj;
}

Response& Response::operator=(const Response& obj)
{
	(void) obj;
	return (*this);
}

int Response::getFd() const {
	return fd;
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
	// std::string root_path = "/Users/mboujama/Desktop/webserv/var/www/html";
	// root path for linux:
	std::string root_path = "/home/mmboujamaa/Main/1337/1337-webserv/var/www/html";
	std::string full_path = root_path + req.getPath();
	http_version = req.getVersion();

	headers["Server"] = "NorthServ/1.0";
	headers["Content-Type"] = "text/html";
	headers["Connection"] = "keep-alive";
	headers["Date"] = ResponseUtils::getDateTime();
	
	//TODO: add checkRedirect:
	if (full_path.find("..") != std::string::npos)
		status_code = FORBIDDEN;
	else if (!client.server->getAllowedMethods()[req.getMethod()])
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
		// 30x
		case MOVED_PERMANENTLY:
			std::cout << "MOVED PERMANENTLY" << std::endl;
			body = "<html><body><center><h1>301 Moved Permanently</h1></center></body></html>";
			headers["Content-Length"] = ResponseUtils::toString(body.length());
			break ;
		// 40x
		case FORBIDDEN:
			std::cout << "FORBIDDEN" << std::endl;
			body = ResponseUtils::getErrorPage(FORBIDDEN);
			headers["Content-Length"] = ResponseUtils::toString(body.length());
			break ;
		case NOT_FOUND:
			body = ResponseUtils::getErrorPage(NOT_FOUND);
			headers["Content-Length"] = ResponseUtils::toString(body.length());
			break ;
		case METHOD_NOT_ALLOWED:
			body = ResponseUtils::getErrorPage(METHOD_NOT_ALLOWED);
			headers["Content-Length"] = ResponseUtils::toString(body.length());
			break ;
		default:
			status_code = OK;
			status_text = "OK";
			body = "<html><body><center><h1>All is good</h1></center></body></html>";
			headers["Content-Length"] = ResponseUtils::toString(body.length());
			status_code = OK;
	}
	
	std::cout << "Allowed methods => [" << ResponseUtils::allowHeaderValue(client.server->getAllowedMethods()) << "]" << std::endl;
}


void Response::handleGet(struct ClientData &client, Request &req, const std::string &path) {
	if (path.find("..") != std::string::npos) {
		status_code = FORBIDDEN; return;
	}
	if (ResponseUtils::isDirectory(path)) {
		if (path.at(path.length() - 1) != '/') {
			status_code = MOVED_PERMANENTLY;
			headers["Location"] = req.getPath() + "/";
			return ;
		}
		
		std::map<std::string, bool> indexes = client.server->getIndexes();
		std::string index = ResponseUtils::isIndexFileExist(indexes, path);
		
		if (!index.empty()) {
			fd = ResponseUtils::openFile(path + index);
			fd == -1 ? status_code = FORBIDDEN : status_code = OK;
			return ;
		}
		// TODO:	2.3 - if autoindex enabled show it (200) otherwise (403)
		else if (client.server->getAutoIndex()) {
			std::cout << "Autoindex enabled" << std::endl;
			status_code = OK;
		}
		else {
			std::cout << "Autoindex disabled" << std::endl;
			status_code = FORBIDDEN;
		}
	}
	else {
		std::string mimeType = getMimeType(path);
		
		std::cout << "Extension: " << mimeType << std::endl;
		// TODO: 3- if file serve it (CGI || 200)
		std::cout << "this is a file" << std::endl;
	}
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
