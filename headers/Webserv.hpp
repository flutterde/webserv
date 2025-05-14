/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:49:53 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/14 13:07:06 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientData.hpp"
# include "WebservHandler.hpp"
# include "readConfig.hpp"
#include <sys/poll.h>
# include <vector>
# include <poll.h>
# include <unistd.h>


class Webserv : public WebservHandler {
	private:
		readConfig					*_config;
		int							_nbrEvents;
		std::vector<char *>			_envs;
		bool					_isRequestComplete(ClientData& client);
		void					_init();
		
	public:
		Webserv();
		Webserv(readConfig& config, char **env);
		~Webserv();
		Server*	getServerByFd(int fd);
		void	run();
		bool	isServerSocket(int fd) const;
		void	acceptNewConnection(int srvfd);
		void	handleClientRequest(int fd);
		void	prepareClientResponse(ClientData& client);
		void	sendResponse(int fd);
		void	timeoutHandler(void);

};
