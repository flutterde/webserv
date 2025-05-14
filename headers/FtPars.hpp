/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtPars.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:22:11 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/25 13:53:45 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Types.hpp"
#include "Server.hpp"
namespace FtPars {
	void						setServerIndexes(Server& server, std::string& line);
	void						autoIndexHandler(Server& server, std::string& line);
	void						serverPortsHandler(Server& srv, std::string& line);
	void						enableUploadsHandler(Server& server, std::string& line);
	void						handleRedirects(Server& server, std::string& line);
	void						handleCGIs(Server& server, std::string& line);
	char						*stringToChar(std::string& str);
	bool						strnCompare(std::string& str1, std::string str2, size_t n);
	bool						isNewServer(std::string& line);
	bool						isValidPattern(std::string& str);
	bool						isValidIP4(std::string& str);
	bool						containSpaces(std::string& str);
	bool						isNumbersOnly(const std::string& str);
	size_t						hexaToNbr(std::string& str); //! not created yet
	size_t						getCurrentTimeMs();
	size_t						charsCount(std::string& str, char c);
	uint32_t					ftInetPton(const std::string& str); //! delete this
	std::string&				strTrim(std::string&str, std::string set);
	std::string					toString(size_t nbr);
	std::map<std::string, bool>	parseMethods(const std::map<std::string, bool>& oldMp, std::string& str);
}
