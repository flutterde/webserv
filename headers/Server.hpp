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
#include <cstddef>
# include <iostream>
# include <cstring>
#include <string>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <fcntl.h>
# include <netinet/in.h>
# include "FtPars.hpp"

#ifndef SO_NOSIGPIPE
#define SO_NOSIGPIPE    0x1022
#endif

#define LISTEN_BACKLOG 12 //! should we increase this ?
#define DEFAULT_ROOT_PATH "./website"
#define BODY_TEMP_PATH "./tmp/path"

typedef	unsigned int	uint32_t;
/// @brief server class that hold data for each website
class	Server {
	private:
		std::string							host;
		uint32_t							port;
		size_t								timeout;
		std::string							serverName;
		std::string							rootPath;
		uint32_t							limitClientBodySize;
		std::string							clientBodyTempPath;
		std::string							errorPage404;
		std::string							errorPage500;
		std::string							uploadsPath;
		std::map<std::string, bool> 		allowedMethods; //?
		std::map<std::string, bool> 		indexes;
		std::map<std::string, std::string>	redirects;
		std::map<std::string, std::string>	cgis;
		std::vector<uint32_t>				ports;
		bool								enableUploads; //? !
		bool								autoIndex; //?
		int									serverSocket;
		int									serverBind;
		int									serverListenFd;
		void								ftSocket(void);
		void								ftBind(void);
		void								ftListen(void);
		void								setSocketOptions(void);

		// utils functions
		std::string&						validateAndTrim(std::string& str);
		void								fillServerData(std::string& line, Server& srv);
		void								setServer(std::vector<std::string>& arr, size_t& idx, Server& srv);


	public:
		Server(const Server& srv, uint32_t port);
		Server(void);
		~Server(void);
		static void						setNonBlocking(int fd);
		Server(std::vector<std::string>& arr, size_t& idx);
		//? Getters
		uint32_t	getPort(void)	const;
		size_t		getTimeout(void)	const;
		const std::string&	getClientBodyTempPath(void) const;
		const std::string&	getUploadsPath(void) const;
		std::string	getHost(void)	const;
		std::string	getserverName(void)	const;
		std::string	getRootPath(void)	const;
		uint32_t	getLimitClientBodySize(void)	const;
		std::string	getErrorPage404(void)	const;
		std::string	getErrorPage500(void)	const;
		std::map<std::string, bool>	getAllowedMethods(void) const;
		std::map<std::string, bool>	getIndexes(void) const;
		bool		getAutoIndex(void) const;
		const std::vector<uint32_t>&	getPorts(void) const;
		bool	getEnableUploads(void) const;
		int		getSocket() const;
		const std::map<std::string, std::string>&	getRedirects(void)	const;
		const std::string&	getCGI(std::string& val)	const;
		const std::map<std::string, std::string>&	getCGIs()	const;
		//? Setters
		void	setPort(uint32_t val);
		void	setTimeout(size_t val);
		void	setHost(std::string& val);
		void	setserverName(std::string& val);
		void	setLimitClientBodySize(uint32_t val);
		void	setClientBodyTempPath(std::string& val);
		void	setUploadsPath(std::string& val);
		void	setErrorPage404(std::string& val);
		void	setErrorPage500(std::string& val);
		void	setIndex(std::string& key, bool val);
		void	setMethods(std::map<std::string, bool> mp);
		void	setAutoIndex(bool val);
		void	setPorts(uint32_t val);
		void	setEnableUploads(bool val);
		void	setRootPath(std::string& val);
		void	setRedirects(const std::string& key, const std::string& val);
		void	setCGI(std::string& key, std::string& val);
		// Server_handlers
		void	initServer(void);
};
