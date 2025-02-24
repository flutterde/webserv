/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtPars.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:22:11 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/24 10:15:37 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>

namespace FtPars {
	static uint32_t limitBodySize =  800001;
	static uint32_t port =  8080;
	size_t	charsCount(std::string& str, char c);
	bool	strnCompare(std::string& str1, std::string str2, size_t n);
	bool	isNewServer(std::string& line);
	std::string&	strTrim(std::string&str, std::string set);
}
