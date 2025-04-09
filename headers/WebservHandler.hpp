/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:37:19 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/09 20:38:12 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "FtPars.hpp"

enum	requestType {
	NOT_SET = -1,
	CONTENT_LENGTH,
	CHUNCKED,
	NO_CONTENT,
	MULTIPART_FORM,
};

struct ClientData {
	int			fd;
	requestType	type;
	bool		isRequestComplete;
	size_t		bytesSent;
	long		contentLen;
	size_t		readed;
	bool		isHeaderComplete;
	int			file;
	std::string	request;
	Server		*server;
	std::string	headers;
	ClientData() : type(NOT_SET), isRequestComplete(false), bytesSent(0), contentLen(-1), readed(0), isHeaderComplete(false), file(-1), server(NULL) {}
};

class WebservHandler
{
	protected:
		void	setRequestType(ClientData& client);
		void	setContentLength(ClientData& client);
		bool	isChunkedComplete(ClientData& client);
		bool	isHeaderComplete(ClientData& client);
		bool	isRequestComplete(ClientData& client);
		bool	isRequestValid(ClientData& client);
		void	handleRequest(ClientData& client);
	public:
		WebservHandler();
	~WebservHandler();

};
