/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:40:33 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/26 10:05:06 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
#include <string>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <netinet/in.h>
# include "FtPars.hpp"

typedef	unsigned int	uint32_t;
/// @brief server class that hold data for each website
class	Server {
	private:
		std::string					host;
		uint32_t					port;
		std::string					serverName;
		uint32_t					limitClientBodySize;
		std::string					errorPage404;
		std::string					errorPage500;
		std::string					uploadsPath;
		std::map<std::string, bool> allowedMethods;
		std::map<std::string, bool> indexes;
		std::vector<uint32_t>		ports;
		bool						autoIndex;
		bool						enableUploads;
		int							serverSocket;
		int							serverBind;
		// struct sockaddr_in			addr;
		void						ftSocket(void);
		void						ftBind(void);
		void						ftListen(void);

	public:
		Server(const Server& srv, uint32_t port);
		Server(void);
		~Server(void);
		Server(std::vector<std::string>& arr, size_t& idx);
		// Getters
		uint32_t	getPort(void)	const;
		std::string	getHost(void)	const;
		std::string	getserverName(void)	const;
		uint32_t	getLimitClientBodySize(void)	const;
		std::string	getErrorPage404(void)	const;
		std::string	getErrorPage500(void)	const;
		std::map<std::string, bool>	getAllowedMethods(void) const;
		std::map<std::string, bool>	getIndexes(void) const;
		bool		getAutoIndex(void) const;
		const std::vector<uint32_t>&	getPorts(void) const;
		bool	getEnableUploads(void) const;
		// Setters
		void	setPort(uint32_t val);
		void	setHost(std::string& val);
		void	setserverName(std::string& val);
		void	setLimitClientBodySize(uint32_t val);
		void	setErrorPage404(std::string& val);
		void	setErrorPage500(std::string& val);
		void	setIndex(std::string& key, bool val);
		void	setMethods(std::map<std::string, bool> mp);
		void	setAutoIndex(bool val);
		void	setPorts(uint32_t val);
		void	setEnableUploads(bool val);
		// Server_handlers
		void	initServer(void);
};
