/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:14:03 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/06 14:20:11 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>

class	Http {
	protected:
		std::map<std::string, std::string>	_headers;
		std::string	_body;

	public:
		Http(void);
		Http(const Http &src);
		Http &operator=(const Http &src);
		virtual ~Http(void);
};
