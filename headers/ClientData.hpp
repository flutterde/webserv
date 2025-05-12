/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:07:08 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/11 16:57:16 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "FtPars.hpp"
#include <cstddef>

enum	requestType {
	NOT_SET = -1,
	CONTENT_LENGTH,
	CHUNKED,
	NO_CONTENT,
	MULTIPART_FORM,
};

enum requestProgress {
	NOT_STARTED = -1,
	WORKING,
	COLLECTED,
	READY,
};

struct ClientData {
	int				fd;
	requestType		type;
	bool			isRequestComplete;
	size_t			bytesSent; //! Why ?
	long			contentLen;
	size_t			readed; //! why ?
	bool			isHeaderComplete;
	bool			isHeadersChecked;
	int				file; //! 
	std::string		request;
	Server			*server;
	std::string		headers;
	long			bodyReded;
	std::string		boundary;
	requestProgress progress;
	size_t			startTime;
	std::string		error;
	std::string tmpFolder;
	std::string	tmpFileName;
	int			currentFileFd;
	//! add map
	ClientData() : type(NOT_SET), isRequestComplete(false),
		bytesSent(0), contentLen(-1), readed(0), isHeaderComplete(false),
		isHeadersChecked(false), file(-1), server(NULL), bodyReded(-1), progress(NOT_STARTED),
		startTime(FtPars::getCurrentTimeMs()), currentFileFd(-1) {}
	~ClientData() {}
};
