/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:37:31 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/06 14:30:59 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Request.hpp"

Request::Request(void) : Http(), isCompleted(false), contentLength(0) {}

Request::Request(const Request& src) : Http(src) {
	this->method = src.method;
	this->uri = src.uri;
	this->isCompleted = src.isCompleted;
	this->contentLength = src.contentLength;
}

Request	&Request::operator=(const Request& src) {
	if (this != &src) {
		Http::operator=(src);
		this->method = src.method;
		this->uri = src.uri;
		this->isCompleted = src.isCompleted;
		this->contentLength = src.contentLength;
	}
	return (*this);
}

Request::~Request(void) {}
