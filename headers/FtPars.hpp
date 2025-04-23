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

#include <cstddef>
# include <iostream>
# include <vector>
# include <map>
# include <sstream>
# include <cstdlib>
# include <cctype>
# include "Server.hpp"

typedef	unsigned int	uint32_t;
typedef unsigned char	uint8_t;

#define FT_LIMIT_BODY_SIZE 80000
#define FT_PORT 8080

# define COL_RED "\033[0;91m"
# define COL_GREEN "\033[0;92m"
# define COL_YELLOW "\033[0;93m"
# define COL_BLUE "\033[0;94m"
# define COL_MAGENTA "\033[0;95m"
# define END_COL "\033[0m"

class	Server;
namespace FtPars {
	size_t	charsCount(std::string& str, char c);
	bool	strnCompare(std::string& str1, std::string str2, size_t n);
	bool	isNewServer(std::string& line);
	std::string&	strTrim(std::string&str, std::string set);
	bool	isValidPattern(std::string& str);
	bool	isValidIP4(std::string& str);
	uint32_t	ftInetPton(const std::string& str);
	char	*stringToChar(std::string& str);
	std::map<std::string, bool> parseMethods(const std::map<std::string, bool>& oldMp, std::string& str);
	void	setServerIndexes(Server& server, std::string& line);
	bool	containSpaces(std::string& str);
	void	autoIndexHandler(Server& server, std::string& line);
	void	serverPortsHandler(Server& srv, std::string& line);
	bool	isNumbersOnly(const std::string& str);
	void	enableUploadsHandler(Server& server, std::string& line);
	std::string	toString(size_t nbr);
	void	handleRedirects(Server& server, std::string& line);
	size_t	hexaToNbr(std::string& str); //! not created yet
}
