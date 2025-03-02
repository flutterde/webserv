/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 15:41:59 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 13:05:36 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/header.hpp"
#include "headers/debug.hpp"//!
#include "headers/readConfig.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>

int	main(int ac, char **av)
{
	if (ac != 2) {
		std::cout << "config file require!" << std::endl;
		return (1);
	}
    try {
        readConfig conf;
        std::ifstream fle;
        conf.readFile(av[1]);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return (1);
    }
    return (0);
}
