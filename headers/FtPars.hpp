/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtPars.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:22:11 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 17:42:12 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>

namespace FtPars {
	size_t	charsCount(std::string& str, char c);
	bool	strnCompare(std::string& str1, std::string str2, size_t n);
	bool	isNewServer(std::string& line);
}
