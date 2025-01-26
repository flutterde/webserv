/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 16:21:32 by ochouati          #+#    #+#             */
/*   Updated: 2025/01/26 10:24:54 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Config.hpp"

static void	_printing(std::string str)
{
	std::cout << str << " called." << std::endl;
}

Config::Config()
{
	if (DEBUG_MODE)
		_printing("Config Default Constructor");
	this->_host = "127.0.0.1";
	this->_port = 8080;
	this->_page404 = "configs/404.html";
	this->_rootFolder = "/tmp/www/";
	this->_clientBodySize = 1048576;
	this->_methods.push_back("GET");
	this->_methods.push_back("POST");
	this->_methods.push_back("DELETE");
	// BUFFER
	this->buffer = new char[this->_clientBodySize]();
}

Config::~Config()
{
	if (DEBUG_MODE)
		_printing("Config Destructor");
	delete[] this->buffer;
}

std::string	Config::getPage404()	const
{return (this->_page404);}

std::string	Config::getHost()	const
{
	return (this->_host);
}

std::string	Config::getRootFolder()	const
{
	return (this->_rootFolder);
}

uint32_t	Config::getPort()	const
{
	return (this->_port);
}

uint32_t	Config::getBodySize()	const
{
	return (this->_clientBodySize);
}

std::list<std::string>	Config::getMethods()	const
{
	return (this->_methods);
}
