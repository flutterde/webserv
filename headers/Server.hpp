/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:40:33 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/19 13:25:23 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <vector>
#include "FtPars.hpp"

typedef	unsigned int	uint32_t;

class	Server {
	private:
		std::string	host;
		uint32_t	port;
		std::string	serverName;
		uint32_t	limitClientBodySize;
		std::string	errorPage404;
		std::string	errorPage500;

	public:
		Server(std::vector<std::string>& arr, size_t& idx);
		Server(void);
		// Getters
		uint32_t	getPort(void)	const;
		std::string	getHost(void)	const;
		std::string	getserverName(void)	const;
		uint32_t	getLimitClientBodySize(void)	const;
		std::string	getErrorPage404(void)	const;
		std::string	getErrorPage500(void)	const;
		// Setters
		void	setPort(uint32_t val);
		void	setHost(std::string& val);
		void	setserverName(std::string& val);
		void	setLimitClientBodySize(uint32_t val);
		void	setErrorPage404(std::string& val);
		void	setErrorPage500(std::string& val);
};
