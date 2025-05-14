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

#include "Types.hpp"
#include "WebservHandler.hpp"
#include "readConfig.hpp"

class Webserv : public WebservHandler {
	private:
		int							_nbrEvents;
		void						_init();
		bool						_isRequestComplete(ClientData& client);
		readConfig					*_config;
		std::vector<char *>			_envs;
		
	public:
		Webserv();
		~Webserv();
		Webserv(readConfig& config, char **env);

		Server*	getServerByFd(int fd);
		void	run();
		void	acceptNewConnection(int srvfd);
		void	handleClientRequest(int fd);
		void	prepareClientResponse(ClientData& client);
		void	sendResponse(int fd);
		void	timeoutHandler(void);
		bool	isServerSocket(int fd) const;
};
