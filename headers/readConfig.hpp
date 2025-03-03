/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:50:46 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/19 13:33:50 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "./header.hpp"
#include "./FtPars.hpp"
#include "./Server.hpp"
#include "./debug.hpp"


class	readConfig {
	private:
		std::vector<Server>			servers;
		std::vector<std::string>	*lines;
		std::vector<std::string>	*env;
		std::vector<std::string>*	readLines(std::ifstream& file);
		void						parseConf(void);
		void						printLines(void) const; //!
	public:
		readConfig(void);
		~readConfig(void);
		void	collectEnv(char **env);
		void	setNewEnv(std::string& val);
		char	**getEnv(void);
		void	readFile(char *argFile, char **env);
		class OpenFileException : public std::exception {
			public:
				const char* what() const throw();
		};
};