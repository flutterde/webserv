/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:32:01 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/06 15:00:58 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Response.hpp"

Response::Response(void) : Http() {}

Response::Response(const Response& src) : Http(src) {
	this->_statusCode = src._statusCode;
}

Response	&Response::operator=(const Response& src) {
	if (this != &src) {
		Http::operator=(src);
		this->_statusCode = src._statusCode;
	}
	return (*this);
}

Response::~Response(void) {}
