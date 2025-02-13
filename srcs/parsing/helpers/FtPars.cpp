/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtPars.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:08:31 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 17:41:56 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../../headers/FtPars.hpp"

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
}
