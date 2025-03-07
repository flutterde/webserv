/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 09:28:54 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/01 17:31:25 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include "../headers/debug.hpp"

void	printing(std::string vr)
{
	std::cout << " -------------------------------------------- \n";
	std::cout << " the Value is: " << vr << std::endl;
	std::cout << " -------------------------------------------- \n";
}

template <typename T>
void	printMap(std::map<std::string, T> mp) {
	for (typename std::map<std::string, T>::iterator it = mp.begin(); it != mp.end(); ++it){
		std::cout << "	->key is: " << it->first << " val: " << (it->second) << std::endl;	
	}
}

void	printServer(Server& srv) {
	std::cout << " ------------------------------------- \n";
	std::cout << " Host: " << srv.getHost() << std::endl;
	std::cout << " Port: " << srv.getPort() << std::endl;
	std::cout << " Server Name: " << srv.getserverName() << std::endl;
	std::cout << " Limit body size: " << srv.getLimitClientBodySize() << std::endl;
	std::cout << " Page 404: " << srv.getErrorPage404() << std::endl;
	std::cout << " Page 500: " << srv.getErrorPage500() << std::endl;
	// std::cout << " SN lenght: " << srv.getserverName().length() << std::endl;
	std::cout << " Methods: \n";
	printMap(srv.getAllowedMethods());
	std::cout << " Indexes: \n";
	printMap(srv.getIndexes());
	std::cout << " AutoIndex: " << (srv.getAutoIndex() ? "true" : "false") << std::endl;
	std::cout << " ------------------------------------- \n";
}
