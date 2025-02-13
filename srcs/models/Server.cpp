/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:56:06 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 18:10:40 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Server.hpp"

Server::Server(void)
{		
	this->limitClientBodySize = 800000;
	this->port = 8080;
	this->serverName = "localhost";
}

Server::Server(std::vector<std::string>& arr, size_t& idx)
{
	(void) this->errorPage404;
	(void) this->errorPage500;
	(void) this->host;
	(void) this->limitClientBodySize;
	(void) this->port;
	(void) this->serverName;
	(void) arr;
	(void) idx;
}
