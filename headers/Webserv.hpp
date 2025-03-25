/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:49:53 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/25 22:04:37 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "Server.hpp"
# include "readConfig.hpp"
#include <cstddef>
#include <map>
#include <sys/poll.h>
# include <vector>
# include <poll.h>
# include <unistd.h>

# define RUNNING 1
# define POLL_TIMEOUT 0
# define END_OF_HEADERS "\r\n\r\n"
# define READ_SIZE 4096
# define CHUNCK_SIZE 4096

struct clientData {
	std::string	request;
	bool		isRequestComplete;
	Server		*server;
	size_t		bytesSent;
	clientData() : isRequestComplete(false), server(NULL) {}
};

class Webserv {
	private:
		readConfig					*_config;
		// char						_buffer[READ_SIZE];
		std::vector<Server>			_servers;
		std::vector<struct pollfd>	_pollfds;
		std::vector<int>			_fds;
		std::vector<char *>			_envs;
		int							_nbrEvents;
		std::map<int, clientData>	_requests;
		void					_closeClients();
		bool					_isRequestComplete(const std::string& request) const;
		void					_init();

	public:
		Webserv();
		Webserv(readConfig& config, char **env);
		~Webserv();
		Server	getServerByFd(const int clientFd) const;
		void	run();
		bool	isServerSocket(int fd) const;
		void	acceptNewConnection(int fd);

};
