/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 16:21:49 by ochouati          #+#    #+#             */
/*   Updated: 2025/01/25 18:42:34 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <list>

#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#endif

typedef unsigned int    uint32_t;
class Config {
	private:
		std::string				_host;
		std::string				_page404;
		std::string				_rootFolder;
		uint32_t				_port;
		uint32_t				_clientBodySize;
		std::list<std::string>	_methods;

	public:
		char	*buffer;
		Config();
		~Config();

		// Getters & Setters
		std::string	getHost()	const;
		std::string	getPage404()	const;
		std::string	getRootFolder()	const;
		uint32_t	getPort()	const;
		uint32_t	getBodySize()	const;
		std::list<std::string>	getMethods()	const;
};
