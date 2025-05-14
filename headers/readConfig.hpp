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

#include "Types.hpp"
#include "Server.hpp"

class	readConfig {
	private:
		std::vector<Server>			servers;
		std::vector<std::string>	*lines;
		std::vector<std::string>*	readLines(std::ifstream& file);
		void						parseConf(void);
		void						printLines(void) const; //!
	public:
		readConfig(void);
		~readConfig(void);
		void					readFile(char *argFile);
		void					seperateServers(void);
		std::vector<Server>&	getServers();

		class OpenFileException : public std::exception {
			public:
				const char* what() const throw();
		};
};