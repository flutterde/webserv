/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 09:28:54 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/19 17:59:08 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../headers/debug.hpp"

void	printing(std::string vr)
{
	std::cout << " -------------------------------------------- \n";
	std::cout << " the Value is: " << vr << std::endl;
	std::cout << " -------------------------------------------- \n";
}

void	printServer(Server& srv) {
	std::cout << " ------------------------------------- \n";
	std::cout << " Host: " << srv.getHost() << std::endl;
	std::cout << " Port: " << srv.getPort() << std::endl;
	std::cout << " Server Name: " << srv.getserverName() << std::endl;
	std::cout << " Limit body size: " << srv.getLimitClientBodySize() << std::endl;
	std::cout << " Page 404: " << srv.getErrorPage404() << std::endl;
	std::cout << " Page 500: " << srv.getErrorPage500() << std::endl;
	std::cout << " SN lenght: " << srv.getserverName().length() << std::endl;
	
	std::cout << " ------------------------------------- \n";
}
