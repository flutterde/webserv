/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:50:19 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 16:02:33 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../../headers/readConfig.hpp"

readConfig::readConfig(void)
{
	this->lines = new std::vector<std::string>();
	if (!this->lines)
		throw std::runtime_error("No ressources");
}

readConfig::~readConfig(void)
{
	delete	this->lines;
}

const char *readConfig::OpenFileException::what() const throw()
{
	return ("can't open that file");
}

bool	readConfig::isNewServer(std::string& line)
{
	if (line[0] == '[' && line[line.size() - 1]
		&& FtPars::charsCount(line, ']') == 1
			&& FtPars::charsCount(line, '[') == 1)
		return (true);
	return (false);
}


std::vector<std::string>*	readConfig::readLines(std::ifstream& file)
{
	std::string	line;
	while (std::getline(file, line))
	{
		line = strTrim(line, " \n\r\t");
		if ((line.empty()) || (line[0] == '#'))
			continue ;
		this->lines->push_back(line);
	}
	return (this->lines);
}

void	readConfig::printLines() const
{
	std::cout << " ----------- \n";
	for (std::vector<std::string>::const_iterator it = this->lines->begin(); it != lines->end(); ++it)
		std::cout << "-> " << *it << std::endl;
}

void	readConfig::readFile(char *argFile)
{
	if (!argFile)
		throw std::runtime_error("no file provided");
	try
	{
		std::ifstream	file(argFile);
		if (file.fail())
			throw std::runtime_error("can't open file");
		this->lines = this->readLines(file);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	this->printLines();
}
