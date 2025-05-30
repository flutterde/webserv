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

#include "headers/Webserv.hpp"
#include "headers/header.hpp"
#include "headers/readConfig.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>

char	**serverEnv;
std::string	*notFound;
WebservHandler	*wServ;

int server(char *argFile, char **env) {
    try {
        readConfig conf;
        conf.readFile(argFile);
        Webserv webserv(conf, env);
        webserv.run();
    } catch (const std::exception &e) {
        std::cerr << "\033[0;91m ** " << e.what() << "  ** \033[0m" << std::endl;
        return (1);
    }
    return (0);
}

int	main(int ac, char **av, char **env)
{
	if (ac != 2) {
		std::cout << "config file require!" << std::endl;
		return (1);
	}
    serverEnv = env;
    notFound = new std::string("not_found");
    return (server(av[1], env));
}
