/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:00 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/25 17:03:56 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Response.hpp"
#include "../../headers/ResponseUtils.hpp"
#include "../../headers/header.hpp"
#include <iostream>

Response::~Response(void) {
	if (fd != -1)
		close(fd);
	delete cgi;
	cgi = NULL;
}

int Response::getFd() const {
	return fd;
}

std::string Response::getBody() const {
	return body;
}

std::map<std::string, std::string> Response::getHeaders() const {
	return headers;
}

std::string Response::getHeadersString() const {
	std::ostringstream res;

	std::map<std::string, std::string>::const_iterator it;
	for (it = headers.begin(); it != headers.end(); it++)
		res << it->first << ": " << it->second << "\r\n";
	res << "\r\n";
	return res.str();
}

size_t Response::getContentlength() const {
	return contentLength;
}

std::string Response::combineResponse(void) {
	std::ostringstream res;

	std::cout << COL_RED << body << END_COL << std::endl;
	res << http_version << " " << status_code << " " << status_text << "\r\n";
	std::map<std::string, std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++)
		res << it->first << ": " << it->second << "\r\n";
	if (!isCgi)
		res << "\r\n";
	res << body;
	return res.str();
}

size_t Response::getBodyLength() {
    if (isCgi)
        return body.length() - body.find("\n\n") - 4;
    return body.length();
}


Response::Response(struct ClientData &client, Request &req) {
	cgi = new Cgi();
	isCgi = false;
	std::string full_path = client.server->getRootPath() + req.getPath();
	http_version = req.getVersion();

	headers["Server"] = "NorthServ/1.0";
	headers["Content-Type"] = "text/html";
	headers["Connection"] = "keep-alive";
	headers["Content-Length"] = "0";
	headers["Date"] = ResponseUtils::getDateTime();
	fd = -1;
	
	if (full_path.find("..") != std::string::npos)
		status_code = FORBIDDEN;
	else if (!client.server->getAllowedMethods()[req.getMethod()]) {
		status_code = METHOD_NOT_ALLOWED;
		headers["Allow"] = ResponseUtils::getAllowHeader(client.server->getAllowedMethods());
	}
	else if (!client.server->getRedirects()[req.getPath()].empty())
	{
		status_code = MOVED_PERMANENTLY;
		headers["Location"] = client.server->getRedirects()[req.getPath()];
		return ;
	}
	else if (!ResponseUtils::pathExists(full_path))
		status_code = NOT_FOUND;
	else if (req.getMethod() == "GET")
		handleGet(client, req, full_path);
	else if (req.getMethod() == "POST")
		handlePost(client, req, full_path);
	else if (req.getMethod() == "DELETE")
		handleDelete(client, req, full_path);
	
	switch (status_code) {
		case CREATED:
			if (!body.empty())
				headers["Content-Length"] = ResponseUtils::toString(getBodyLength());
			break ;
		// 30x
		case MOVED_PERMANENTLY:
			if (body.empty())
				body = "<html><body><center><h1>301 Moved Permanently</h1></center></body></html>";
			headers["Content-Length"] = ResponseUtils::toString(body.length());
			break ;
		// 40x
		case FORBIDDEN:
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
		case NOCONTENT:
			break;
		// 50x
		case INTERNAL_SERVER_ERROR:
			body = ResponseUtils::getErrorPage(INTERNAL_SERVER_ERROR);
			headers["Content-Length"] = ResponseUtils::toString(body.length());
			break ;
			break;
		default:
			status_code = OK;
			status_text = "OK";
			if (!body.empty())
				headers["Content-Length"] = ResponseUtils::toString(getBodyLength());
	}
}

void Response::handleGet(struct ClientData &client, Request &req, std::string &path) {
	bool isFile = true;
	std::string index;

	if (path.find("..") != std::string::npos) {
		status_code = FORBIDDEN; 
		return;
	}
	if (ResponseUtils::isDirectory(path)) {
		if (path.at(path.length() - 1) != '/') {
			status_code = MOVED_PERMANENTLY;
			headers["Location"] = req.getPath() + "/";
			return ;
		}
		isFile = false;
		std::map<std::string, bool> indexes = client.server->getIndexes();
		index = ResponseUtils::isIndexFileExist(indexes, path);
		if (!index.empty()) isFile = true;
		else if (client.server->getAutoIndex()) {
			body = ResponseUtils::generateAutoIndex(path);
			status_code = OK;
		}
		else status_code = FORBIDDEN;
	}
	if (isFile) {
		if (!index.empty())
			path += index;

		// ! Check if directory instead of dot !!!
		int dot = path.find_last_of(".");
		if ((int)dot != -1) {        
			std::string extension = path.substr(dot);
        	if (!extension.compare(".py") || client.server->getCGI(extension).compare("not_found")) {            
				body = cgi->executeCgiScript(req, serverEnv);
				if (req.client.status != 0)
					status_code = INTERNAL_SERVER_ERROR;
				else
					isCgi = true;      
			}
   		}
		else {
			struct stat fileStat;

			fd = ResponseUtils::openFile(path);
			if (stat(path.c_str(), &fileStat) == -1) {
				status_code = INTERNAL_SERVER_ERROR;
				return ;
			}
			contentLength = fileStat.st_size;
			headers["Content-Length"] = FtPars::toString(contentLength);
			headers["Content-Type"] = MimeTypes::getMimeType(path);
		}	
	}
	wServ->enablePOLLOUT(client.fd);
	client.progress = READY;
}

void Response::handlePost(struct ClientData &client, Request &req, std::string &path) {	
	if (path.find("..") != std::string::npos) {
		status_code = FORBIDDEN; 
		return;
	}
	int dot = path.find_last_of(".");
	if ((int)dot != -1) {        
		std::string extension = path.substr(dot);
		if (!extension.compare(".py") || client.server->getCGI(extension).compare("not_found")) {            
			body = cgi->executeCgiScript(req, serverEnv);
			if (req.client.status != 0)
				status_code = INTERNAL_SERVER_ERROR;
			else
				isCgi = true;      
		}
	} else {
		status_code = CREATED;
		status_text = "Created";
	}
	headers["Allow-Origin"] = "*";
	(void) req;
	wServ->enablePOLLOUT(client.fd);
	client.progress = READY;
}

void Response::handleDelete(struct ClientData &client, Request &req, std::string &path) {
	(void) client;
	if (path.find("..") != std::string::npos) {
		status_code = FORBIDDEN; 
		return;
	}

	if (ResponseUtils::isDirectory(path)) {
		if ((path.at(path.length() - 1)) != '/') {
			status_code = MOVED_PERMANENTLY;
			headers["Location"] = req.getPath() + "/";
			return ;
		}
		if (ResponseUtils::deleteFolder(path))
			status_code = NOCONTENT;
		else {
			if (access(path.c_str(), W_OK))
				status_code = FORBIDDEN;
			else
				status_code = INTERNAL_SERVER_ERROR;
		}
	} else {
		if (ResponseUtils::deleteFile(path))
			status_code = NOCONTENT;
		else {
			if (access(path.c_str(), W_OK))
				status_code = FORBIDDEN;
			else
				status_code = INTERNAL_SERVER_ERROR;
		}
	}
	wServ->enablePOLLOUT(client.fd);
	client.progress = READY;
}
