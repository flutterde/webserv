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

# include <iostream>
# include <vector>
# include <map>
#include <sstream>
#include <cstdlib>

typedef	unsigned int	uint32_t;
typedef unsigned char	uint8_t;


#define FT_LIMIT_BODY_SIZE 800001
#define FT_PORT 8080
namespace FtPars {
	size_t	charsCount(std::string& str, char c);
	bool	strnCompare(std::string& str1, std::string str2, size_t n);
	bool	isNewServer(std::string& line);
	std::string&	strTrim(std::string&str, std::string set);
	bool	isValidPattern(std::string& str);
	bool	isValidIP4(std::string& str);
	uint32_t	ftInetPton(const std::string& str);
	char	*stringToChar(std::string& str);
	std::map<std::string, bool>& parseMethods(std::map<std::string, bool>& mp, std::string& str);
}
