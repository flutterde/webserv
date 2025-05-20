/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:07:08 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/17 08:40:44 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Types.hpp"
#include "Server.hpp"
#include "FtPars.hpp"
// #include "Response.hpp"

class Response;
struct ClientData {
	int							fd;
	int							file; //!
	int							currentFileFd;
	long						contentLen;
	long						bodyReded;
	bool						isRequestComplete;
	bool						isHeaderComplete;
	bool						isHeadersChecked;
	bool						isHeadersSent;
	Server						*server;
	size_t						bytesSent; //! Why ?
	size_t						readed; //! why ?
	size_t						startTime;
	Response					*resp;
	std::string					method;
	std::string					request;
	std::string					headers;
	std::string					boundary;
	std::string					error;
	std::string					tmpFolder;
	std::string					tmpFileName;
	requestType					type;
	requestProgress				progress;
	std::map<std::string, int>	uploadFd;
	//! add map
	ClientData() : file(-1),currentFileFd(-1) ,contentLen(-1), bodyReded(-1), isRequestComplete(false), isHeaderComplete(false),isHeadersChecked(false),
				isHeadersSent(false), server(NULL), bytesSent(0),readed(0),startTime(FtPars::getCurrentTimeMs()),resp(NULL), type(NOT_SET), progress(NOT_STARTED){}
	~ClientData();
};

typedef std::map<int, ClientData>::iterator mapIt;
