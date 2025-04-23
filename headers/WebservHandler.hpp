/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:37:19 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/23 20:11:03 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "FtPars.hpp"
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include "debug.hpp"

# define RUNNING 1
# define POLL_TIMEOUT 0
# define END_OF_HEADERS "\r\n\r\n"
# define READ_SIZE 4096
# define CHUNCK_SIZE 4096

enum	requestType {
	NOT_SET = -1,
	CONTENT_LENGTH,
	CHUNKED,
	NO_CONTENT,
	MULTIPART_FORM,
};

struct ClientData {
	int			fd;
	requestType	type;
	bool		isRequestComplete;
	size_t		bytesSent;
	long		contentLen;
	size_t		readed; //! why ?
	bool		isHeaderComplete;
	int			file; //! 
	std::string	request;
	Server		*server;
	std::string	headers;
	long		bodyReded;
	std::string	boundary;
	//! add map
	ClientData() : type(NOT_SET), isRequestComplete(false), bytesSent(0), contentLen(-1), readed(0), isHeaderComplete(false), file(-1), server(NULL), bodyReded(-1) {}
};

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

	public:
		WebservHandler();
		static int requestCount;
	~WebservHandler();

};
