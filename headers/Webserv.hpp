/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:49:53 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/26 21:33:20 by ochouati         ###   ########.fr       */
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

struct ClientData {
	int			fd;
	bool		isRequestComplete;
	size_t		bytesSent;
	std::string	request;
	Server		*server;
	ClientData() : isRequestComplete(false), bytesSent(0), server(NULL) {}
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
		std::map<int, ClientData>	_requests;
		void					_closeClients();
		void					_closeClient(int fd);
		bool					_isRequestComplete(const std::string& request) const;
		void					_init();
		void					setNonBlocking(int fd);

	public:
		Webserv();
		Webserv(readConfig& config, char **env);
		~Webserv();
		Server	getServerByFd(int fd) const;
		void	run();
		bool	isServerSocket(int fd) const;
		void	acceptNewConnection(int fd);

};
