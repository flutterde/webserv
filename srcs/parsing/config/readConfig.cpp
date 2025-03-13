/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:50:19 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/24 10:16:23 by ochouati         ###   ########.fr       */
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
	delete	this->env;
}

const char *readConfig::OpenFileException::what() const throw()
{
	return ("can't open that file");
}

void	readConfig::collectEnv(char **env)
{
	this->env = new std::vector<std::string>();
	for (size_t i = 0; env[i]; i++)
		this->env->push_back(env[i]);
}

void	readConfig::setNewEnv(std::string& val)
{
	this->env->push_back(val);
}

char	**readConfig::getEnv(void) //!
{
	size_t	i;
	char	**strs = new char*[this->env->size() + 1]();
	for (i = 0; i < this->env->size(); i++)
	{
		strs[i] = FtPars::stringToChar((*this->env)[i]);
	}
	strs[i] = NULL;
	return (strs);
}

void	readConfig::parseConf(void)
{
	Server srv;
	for (size_t i = 0; i < this->lines->size(); ++i) {
		if (FtPars::isNewServer((*this->lines)[i])) {
			srv = Server(*this->lines, i);
			this->servers.push_back(srv);
		}
	}
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

void	readConfig::readFile(char *argFile, char **env)
{
	if (!argFile || !env || !*env)
		throw std::runtime_error("no config file or no env provided");
	try {
		this->collectEnv(env);
		std::ifstream	file(argFile);
		if (file.fail())
			throw std::runtime_error("can't open file");
		this->lines = this->readLines(file);
		parseConf();
	}
	catch(const std::exception& e) {
		throw std::runtime_error(e.what());
	}
	delete this->lines;
	this->lines = NULL;
	for (size_t x = 0; x < this->servers.size();) {
		if (this->servers[x].getserverName().empty())
			this->servers.erase(this->servers.begin() + x);
		else
			++x;
	}
	this->seperateServers();
	std::cout << this->servers.size() << " ################################################### \n";
	for (size_t i = 0; i < this->servers.size(); ++i) { //!
		std::cout << "Id: " << i << std::endl;
		printServer(this->servers[i]);
	}
}

void	readConfig::seperateServers(void)
{
	for (size_t i = 0; i < this->servers.size(); ++i) {
		for (size_t j = 1; j < this->servers[i].getPorts().size(); j++) {
			Server tmp = Server(this->servers[i], this->servers[i].getPorts()[j]);
			this->servers.push_back(tmp);
		}
	}
}
