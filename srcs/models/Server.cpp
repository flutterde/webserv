/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:56:06 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 16:00:05 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/Server.hpp"

Server::Server(std::vector<std::string>& arr, int& idx)
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
