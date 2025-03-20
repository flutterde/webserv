/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:00 by mboujama          #+#    #+#             */
/*   Updated: 2025/03/20 10:06:56 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Response.hpp"

Response::Response(void)
{
	std::cout << "Response Default constructor called" << std::endl;
}

Response::~Response(void)
{
	std::cout << "Response Destructor called" << std::endl;
}

Response::Response(const Response& obj)
{
	std::cout << "Response Copy constructor called" << std::endl;
	*this = obj;
}

Response& Response::operator=(const Response& obj)
{
	std::cout << "Response Copy assignment operator called" << std::endl;
	if (this != &obj)
	{}
	return (*this);
}

std::string Response::getHttpVersion() {
	return this->http_version;
}

int Response::getStatusCode() {
	return this->status_code;
}

std::string Response::getStatusText() {
	return this->status_text;
}

std::string Response::getBody() {
	return this->body;
}


void Response::setHttpVersion(std::string version) {
	this->http_version = version;
}

void Response::setStatusCode(int status) {
	this->status_code = status;
}

void Response::setStatusText(std::string status) {
	this->status_text = status;
}

void Response::setBody(std::string body) {
	this->body = body;
}
