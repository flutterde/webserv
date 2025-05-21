/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 19:30:28 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/21 12:15:26 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Server.hpp"
#include "../../headers/FtPars.hpp"


std::string&	Server::validateAndTrim(std::string& str) {
	try
	{
		str = str.substr(str.find_first_of('=') + 1, str.length());
		str = FtPars::strTrim(str, " 	");
		FtPars::isValidPattern(str);
		str = FtPars::strTrim(str, "\"");
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
	return (str);
}

void	Server::fillServerData(std::string& line, Server& srv) {
	std::string str;
	str = line;
	if (!line.compare(0, 11, "server_name")) {
		if (!srv.getserverName().empty())
			throw std::runtime_error("server_name already set");
		this->validateAndTrim(str);
		srv.setserverName(str);
	} else if (!line.compare(0, 4, "host")) {
		if (!srv.getHost().empty())
			throw std::runtime_error("host already set");
		this->validateAndTrim(str);
		FtPars::isValidIP4(str);
		srv.setHost(str);
	} else if (!line.compare(0, 4, "port")) {
		this->validateAndTrim(str);
		FtPars::serverPortsHandler(srv, str);
	} else if (!line.compare(0, 20, "client_max_body_size")) {
		this->validateAndTrim(str);
		srv.setLimitClientBodySize(std::atol(str.c_str()));
	} else if (!line.compare(0, 14, "error_page_404")) {
		this->validateAndTrim(str);
		srv.setErrorPage404(str);
	} else if (!line.compare(0, 14, "error_page_500")) {
		this->validateAndTrim(str);
		srv.setErrorPage500(str);
	}else if (!line.compare(0, 15, "allowed_methods")) {
		this->validateAndTrim(str);
		srv.setMethods(FtPars::parseMethods(srv.getAllowedMethods(), str));
	} else if (!line.compare(0, 7, "indexes")) {
		this->validateAndTrim(str);
		FtPars::setServerIndexes(srv, str);
	} else if (!line.compare(0, 9, "autoindex")) {
		this->validateAndTrim(str);
		FtPars::autoIndexHandler(srv, str);
	} else if (!line.compare(0, 12, "upload_store")) {
		this->validateAndTrim(str);
		srv.setUploadsPath(str);
	} else if (!line.compare(0, 14, "upload_enabled")) {
		this->validateAndTrim(str);
		FtPars::enableUploadsHandler(srv, str);
	} else if (!line.compare(0, 13, "location_root")) {
		this->validateAndTrim(str);
		srv.setRootPath(str);
	} else if (!line.compare(0, 9, "redirects")) {
		this->validateAndTrim(str);
		FtPars::handleRedirects(srv, str);
	} else if (!line.compare(0, 21, "client_body_temp_path")) {
		this->validateAndTrim(str);
		srv.setClientBodyTempPath(str);
	} else if (!line.compare(0, 3, "cgi")) {
		this->validateAndTrim(str);
		FtPars::handleCGIs(srv, str);
	} else if (!line.compare(0, 14, "client_timeout")) {
		this->validateAndTrim(str);
		srv.setTimeout(std::atoi(str.c_str()));
	}
}


void	Server::setServer(std::vector<std::string>& arr, size_t& idx, Server& srv)
{
	idx++;
	for (size_t i = idx; i < arr.size(); ++i) {
		if (FtPars::isNewServer(arr[i])) {
			break;
		}
		this->fillServerData(arr[i], srv);
	}
	idx--;
}
