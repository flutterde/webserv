/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readconfigFile.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 10:05:42 by ochouati          #+#    #+#             */
/*   Updated: 2025/01/26 13:29:16 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/header.hpp"
#include <fstream>
#include <cctype>

static bool	readLines(std::vector<Cont>& vec, std::ifstream& file)
{
	std::string			line;
	
	while (std::getline(file, line))
	{
		line = strTrim(line, " \t");
		std::cout << line << std::endl;
		if (line == "server {")
			vec.push_back(Cont(line, true, false));
		else if (line[0] == '#')
			continue;
		else
			vec.push_back(Cont(line, true, false));
	}
	return (true);
}

std::vector<Cont>	readConfigFile(std::string& file)
{
	std::vector<Cont>	vec;

	if (file.empty()) {
		std::cerr << "file not exist" << std::endl;
		std::exit(1);
	}
	std::ifstream	confFile(file.c_str());
	if (confFile.fail()) {
		std::cerr << "config file could not opened" << std::endl;
		std::exit(1);
	}
	readLines(vec, confFile);
	confFile.close();
	return (vec);
}
