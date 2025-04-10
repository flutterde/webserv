/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:49:53 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/10 14:59:07 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "WebservHandler.hpp"
# include "readConfig.hpp"
#include <cstddef>
#include <map>
#include <sys/poll.h>
# include <vector>
# include <poll.h>
# include <unistd.h>



class Webserv : public WebservHandler {
	private:
		readConfig					*_config;
		// std::vector<Server>			_servers;
		// std::vector<struct pollfd>	_pollfds;
		// std::vector<int>			_fds;
		std::vector<char *>			_envs;
		int							_nbrEvents;
		// std::map<int, ClientData>	_requests;
		bool					_isRequestComplete(ClientData& client);
		void					_init();
		// void					setNonBlocking(int fd);

	public:
		Webserv();
		Webserv(readConfig& config, char **env);
		~Webserv();
		// void	_closeClient(int fd);
		Server*	getServerByFd(int fd);
		void	run();
		bool	isServerSocket(int fd) const;
		void	acceptNewConnection(int srvfd);
		void	handleClientRequest(int pollIdx, int fd);

};
