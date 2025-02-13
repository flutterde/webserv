/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:40:33 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 15:58:28 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

typedef	unsigned int	uint32_t;

class	Server {
	private:
		std::string	host;
		uint32_t	port;
		std::string	serverName;
		uint32_t	limitClientBodySize;
		std::string	errorPage404;
		std::string	errorPage500;
	public:
		Server(std::vector<std::string>& arr, int& idx);
};
