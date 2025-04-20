/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:00 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/19 13:22:27 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Response.hpp"

Response::Response(void)
{
	headers["Server"] = "NorthServ/1.0";
}

Response::Response(struct ClientData &client, const Request &req) {
	std::string root_path = "/var/www/html";
	std::string full_path = root_path + req.getPath();
	http_version = req.getVersion();

	//TODO: add checkRedirect: I need the
	if (!checkRequestedPath(client, full_path) || !checkAllowedMethods(client, req))
		return ;
	fd = ResponseUtils::openFile(full_path);
	if (req.getMethod() == "GET")
		handleGet(client, req);
	else if (req.getMethod() == "POST")
		std::cout << "post" << std::endl;
	else if (req.getMethod() == "DELETE")
		std::cout << "delete" << std::endl;
	
	status_code = 200;
	status_text = "OK";
	body = "<html><body><center><h1>All Is Good</h1></center></body></html>";
	headers["Server"] = "NorthServ/1.0";
	headers["Content-Type"] = "text/html";
	headers["Content-Length"] = std::to_string(body.size());
	headers["Connection"] = "close";
	headers["Date"] = "";
	std::cout << "Allowed methods => [" << ResponseUtils::allowHeaderValue(client.server->getAllowedMethods()) << "]" << std::endl;
}

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

// Check if requested url exists
int Response::checkRequestedPath(struct ClientData &client, const std::string &full_path) {
	if (!ResponseUtils::pathExists(full_path)) {
		status_code = 404;
		status_text = "Not Found";
		body = client.server->getErrorPage404();
		headers["Content-Type"] = "text/html";
		headers["Content-Length"] = std::to_string(body.size());
		headers["Connection"] = "close";
		return 0;
	}
	return 1;
}

int Response::checkAllowedMethods(struct ClientData &client, const Request &req) {
	if (!client.server->getAllowedMethods()[req.getMethod()]) {
		status_code = 405;
		status_text = "Method Not Allowed";
		
		body = "<html><body><center><h1>405 Not Allowed Method</h1></center></body></html>";
		headers["Content-Type"] = "text/html";
		headers["Content-Length"] = std::to_string(body.size());
		headers["Allow"] = ResponseUtils::allowHeaderValue(client.server->getAllowedMethods());
		headers["Connection"] = "close";
		return 0;
	}
	return 1;
}

void Response::handleGet(struct ClientData &client, const Request &req) {
	// TODO: 1- 404 not found if the requested path doesn't exist.
	// TODO: 2- 301 if the path doesn't have / at the end.
	// TODO: 3- In case of directory Serve index.html if exists otherwise serve autoindex if enabled.
	// TODO: 4- Serve the requested file.
	(void) client;
	(void) req;
	std::cout << "get" << std::endl;
}

void Response::handlePost(struct ClientData &client, const Request &req) {
	(void) client;
	(void) req;
	std::cout << "post" << std::endl;
}

void Response::handleDelete(struct ClientData &client, const Request &req) {
	(void) client;
	(void) req;
	std::cout << "delete" << std::endl;
}
