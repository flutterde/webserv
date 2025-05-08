/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:37:19 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/08 09:41:10 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "debug.hpp"
#include "ClientData.hpp"

# define RUNNING 1
# define POLL_TIMEOUT 0
# define END_OF_HEADERS "\r\n\r\n"
# define READ_SIZE 8192
# define URL_MAX_SIZE 200
# define CHUNCK_SIZE 4096

typedef std::map<int, ClientData>::iterator mapIt;

class WebservHandler
{
	protected:
		std::vector<Server>			_servers;
		std::vector<struct pollfd>	_pollfds;
		std::map<int, ClientData>	_requests;
		void	setRequestType(ClientData& client);
		void	setContentLength(ClientData& client);
		bool	isChunkedComplete(ClientData& client);
		bool	isHeaderComplete(ClientData& client);
		bool	isRequestComplete(ClientData& client);
		bool	isRequestValid(ClientData& client);
		void	handleRequest(ClientData& client);
		void	_closeClient(int fd);
		void	setBoundary(ClientData& client);
		//* Validate the request
		void	validateRequestHeaders(ClientData& client);
		void	validateUrl(ClientData& client);

	public:
		WebservHandler();
		static int requestCount;
		void 					enablePOLLOUT(int fd);
	~WebservHandler();

};
