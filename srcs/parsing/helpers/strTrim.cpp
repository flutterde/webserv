/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strTrim.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 12:03:41 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 11:35:52 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

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
