/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 09:27:31 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/01 17:31:49 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "FtPars.hpp"
#include "header.hpp"
#include "Server.hpp"
#include "readConfig.hpp"
#include <iostream>
#include <vector>

class Server;
void	printing(std::string vr);
void	printServer(Server& srv);


template <typename T>
void	ftPrintln(T val) {
	std::cout << val << std::endl;
}

template <typename T>
void printContainer(T& cont) 
{
	std::cout << "\n -------------------PINT_ENV----------------- "<< std::endl;
	for (typename T::iterator it = cont.begin(); it != cont.end(); ++it)
		std::cout << *it << std::endl;
	std::cout << "\n --------------------END_PINT_ENV---------------- "<< std::endl;
}

void	printTime();

void	printWarning(std::string str);

void	printRequestType(int nbr);
