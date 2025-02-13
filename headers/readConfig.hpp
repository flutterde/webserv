/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:50:46 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 16:01:14 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "./header.hpp"
#include "./FtPars.hpp"
#include "./Server.hpp"

class	readConfig {
	private:
		std::vector<Server>			servers;
		std::vector<std::string>	*lines;
		std::vector<std::string>*	readLines(std::ifstream& file);
		void						printLines(void) const; //!
		bool						isNewServer(std::string& line);
	public:
		readConfig(void);
		~readConfig(void);
		void	readFile(char *argFile);
		class OpenFileException : public std::exception {
			public:
				const char* what() const throw();
		};
};