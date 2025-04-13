/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:00 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/13 16:16:51 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Response.hpp"

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


std::string getDateTime() {
	time_t rawtime;
	struct tm * datetime;
	char buffer [80];

	time (&rawtime);
	datetime = localtime(&rawtime);

	strftime (buffer,80,"%a, %d %h %Y %T %Z", datetime);
	return std::string(buffer);
}

void Response::printResHeaders(void) const
{
	std::map<std::string, std::string>::const_iterator it;
	for (it = this->headers.begin(); it != this->headers.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
}

std::string Response::combineResponse(void) {
	std::ostringstream res;

	headers["Date"] = getDateTime();
	res << http_version << " " << status_code << " " << status_text << "\r\n";
	std::map<std::string, std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++) {
		res << it->first << ": " << it->second << "\r\n";
	}
	res << "\r\n" << body;
	return res.str();
}

void Response::handleGet(struct ClientData &client, const Request &req) {
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

std::string Response::handleResponse(struct ClientData &client, const Request &req) {
	http_version = "HTTP/1.1";

	if (!client.server->getAllowedMethods()[req.getMethod()]) {
		status_code = 405;
		status_text = "Method Not Allowed";
		headers["Content-Length"] = "0";
		headers["Server"] = "NorthServ/1.0";
		headers["Allow"] = "GET, POST, DELETE";
		headers["Connection"] = "close";
		return (combineResponse());
	}

	if (req.getMethod() == "GET")
		handleGet(client, req);
	else if (req.getMethod() == "POST")
		std::cout << "post" << std::endl;
	else if (req.getMethod() == "DELETE")
		std::cout << "delete" << std::endl;
	else {
		
	}
	
	status_code = 200;
	status_text = "OK";
	body = """<html>\n\t<body>\n\t\t<center>\n\t\t\t<h1>Hello World</h1>\n\t\t</center>\n\t</body>\n</html>""";
	headers["Server"] = "NorthServ/1.0";
	headers["Content-Type"] = "text/html";
	headers["Content-Length"] = std::to_string(body.size());
	headers["Connection"] = "close";
	headers["Date"] = "";
	
	return (combineResponse());
}
