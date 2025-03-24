/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:49:53 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/24 23:52:45 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "Server.hpp"
# include "readConfig.hpp"
#include <map>
#include <sys/poll.h>
# include <vector>
# include <poll.h>
# include <unistd.h>

# define RUNNING 1
# define POLL_TIMEOUT 0
# define END_OF_HEADERS "\r\n\r\n"
# define READ_SIZE 4096

struct requestData {
	std::string	request;
	bool		isRequestComplete;

	requestData() : isRequestComplete(false) {}
};

class Webserv {
	private:
		readConfig					_config;
		// char						_buffer[READ_SIZE];
		std::vector<Server>			_servers;
		std::vector<struct pollfd>	_pollfds;
		std::vector<int>			_fds;
		// int							_nbrEvents;
		// ssize_t						_readBytes;
		std::map<int, int>		_clientToServer;
		std::map<int, requestData>	_requests;
		void					_closeClients();
		bool					_isRequestComplete(const std::string& request) const;
		void					_init();
		
	public:
		Webserv();
		Webserv(readConfig& config);
		~Webserv();
		Server	getServerByFd(const int clientFd) const;
		void	run();

};
