/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:37:19 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/12 18:57:43 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "ClientData.hpp"
#include "HttpErrors.hpp"
#include "debug.hpp"

# define RUNNING 1
# define POLL_TIMEOUT 0
# define END_OF_HEADERS "\r\n\r\n"
# define READ_SIZE 1024
# define URL_MAX_SIZE 2048
# define CHUNCK_SIZE 4096

void processMultipartUpload(ClientData &client);
typedef std::map<int, ClientData>::iterator mapIt;
class HttpErrors;
class WebservHandler
{
	protected:
		std::vector<Server>			_servers;
		std::vector<struct pollfd>	_pollfds;
		std::map<int, ClientData>	_requests;
		void	setRequestType(ClientData& client);
		void	setContentLength(ClientData& client);
		bool	isHeaderComplete(ClientData& client);
		bool	isRequestComplete(ClientData& client);
		bool	isRequestValid(ClientData& client);
		void	handleRequest(ClientData& client);
		void	_closeClient(int fd);
		void	setBoundary(ClientData& client);
		void	setMethod(ClientData& client);
		//* Validate the request
		void	validateRequestHeaders(ClientData& client);
		void	validateUrl(ClientData& client);

	public:
		WebservHandler();
		void 					enablePOLLOUT(int fd);
	~WebservHandler();
};

extern WebservHandler	*wServ;
