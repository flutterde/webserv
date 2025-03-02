/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtPars.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:08:31 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/24 09:48:26 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../../headers/FtPars.hpp"

/// @brief collection of helper functions for parsing
namespace	FtPars {
	size_t	charsCount(std::string& str, char c) {
		size_t	i = 0;
		for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
			if (*it == c)
				i++;
		}
		return (i);
	}

	bool	strnCompare(std::string& str1, std::string str2, size_t n) {
		std::string	tmp = str1.substr(0, n);
		if (tmp == str2)
			return (true);
		return (false);
	}

	bool	isNewServer(std::string& line)
	{
		if (!line.compare(0, 8, "[server.") && line[0] == '[' && line[line.size() - 1]
			&& FtPars::charsCount(line, ']') == 1
				&& FtPars::charsCount(line, '[') == 1)
			return (true);
		return (false);
	}

	std::string&	strTrim(std::string&str, std::string set)
	{
		size_t	f = str.find_first_not_of(set);
		if (f != std::string::npos)
			str = str.erase(0, f);
		size_t	l = str.find_last_not_of(set);
		if (l != std::string::npos)
			str = str.erase(l + 1, str.length());
		return (str);
	}

	bool	isValidPattern(std::string& str)
	{
		if (str.empty() || str[0] != '"' || str[str.size() - 1] != '"' || str.size() < 3)
			throw std::runtime_error("invalid configs");
		return (true);
	}

	bool	isValidIP4(std::string& str)
	{
		if (charsCount(str, '.') != 3)
				throw std::runtime_error("invalid configs");
		for (size_t i = 0; i < str.size(); i++)
		{
			if (str[i] != '.' && !std::isdigit(str[i]))
				throw std::runtime_error("invalid configs");
		}
		return (true);
	}

	/// @brief Convert an IPv4 address from its text representation to binary form
	/// @param const std::string& str
	/// @return uint32_t (ip)
	uint32_t	ftInetPton(const std::string& str)
	{
		std::vector<std::string> arr;
		std::stringstream ss(str);
		uint32_t	ip = 0;
		uint8_t		byte;

		while (ss.good())
		{
			std::string substr;
			getline(ss, substr, '.');
			arr.push_back(substr);
		}
		if (arr.size() != 4)
			return (0);
		 for (size_t i = 0; i < arr.size(); i++) {
			byte = std::atoi(arr[i].c_str());
			ip |= byte << (24 - (i * 8));
		}
		return (ip);
	}
}
