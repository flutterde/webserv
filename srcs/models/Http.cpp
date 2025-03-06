/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:18:59 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/06 14:21:00 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Http.hpp"

Http::Http(void) {}

Http::Http(const Http& src) {
	this->_headers = src._headers;
	this->_body = src._body;
}

Http& Http::operator=(const Http& src) {
	if (this != &src) {
		this->_headers = src._headers;
		this->_body = src._body;
	}
	return (*this);
}

Http::~Http(void) {}
