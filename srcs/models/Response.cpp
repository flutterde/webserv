/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:00 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/14 09:54:06 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Response.hpp"
#include <ostream>

Response::Response(void) {}

Response::~Response(void) {}

Response::Response(const Response& obj) {
	*this = obj;
}

Response& Response::operator=(const Response& obj) {
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
	cgi = new Cgi();
	std::string full_path = client.server->getRootPath() + req.getPath();
	std::cout << COL_GREEN << "Full path: " << full_path << END_COL << std::endl;
	http_version = req.getVersion();

	headers["Server"] = "NorthServ/1.0";
	// headers["Content-Type"] = "text/html";
	headers["Connection"] = "keep-alive";
	headers["Date"] = ResponseUtils::getDateTime();
	fd = -1;
	
	if (full_path.find("..") != std::string::npos)
		status_code = FORBIDDEN;
	else if (!client.server->getAllowedMethods()[req.getMethod()])
		status_code = METHOD_NOT_ALLOWED;
	else if (!client.server->getRedirects()[req.getPath()].empty())
	{
		status_code = MOVED_PERMANENTLY;
		headers["Location"] = client.server->getRedirects()[req.getPath()];
		return ;
	}
	else if (!ResponseUtils::pathExists(full_path)) {
		status_code = NOT_FOUND;
		std::cout << COL_RED << "File really doesn't exist" << END_COL << std::endl;
	}
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
			if (body.empty())
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
		case NOCONTENT:
			break;
		case INTERNAL_SERVER_ERROR:
			break;
		default:
			status_code = OK;
			status_text = "OK";
			if (body.empty())
				body = "<html><body><center><h1>All is good</h1></center></body></html>";
			headers["Content-Length"] = ResponseUtils::toString(body.length());
			status_code = OK;
	}
	std::cout << "Allowed methods => [" << ResponseUtils::allowHeaderValue(client.server->getAllowedMethods()) << "]" << std::endl;
}


void Response::handleGet(struct ClientData &client, Request &req, std::string &path) {
	std::cout << COL_YELLOW << "Getting here" << END_COL << std::endl;
	bool isFile = true;
	std::string index;

	if (path.find("..") != std::string::npos) {
		status_code = FORBIDDEN; 
		return;
	}
	std::cout << COL_YELLOW << "Getting here" << END_COL << std::endl;
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
		std::cout << COL_YELLOW << "Status setted: " << status_code << END_COL << std::endl;
	}
	if (isFile) {
		if (!index.empty()) {
			path += index;
		}
		if (!path.substr(path.find_last_of('.')).compare(".py") 
			|| !path.substr(path.find_last_of('.')).compare(".php")) {
			body = cgi->executeCgiScript(req, serverEnv);
		}
		else {
			!index.empty() 
				? fd = ResponseUtils::openFile(req.getPath() + index)  
				: fd = ResponseUtils::openFile(path);
		}	
	}
	std::cout << COL_RED << "End get request" << END_COL << std::endl;
}

void Response::handlePost(struct ClientData &client, Request &req, const std::string &path) {	
	(void) client;
	(void) req;
	(void) path;
	std::cout << "post" << std::endl;
}

void Response::handleDelete(struct ClientData &client, Request &req, const std::string &path) {
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
}
