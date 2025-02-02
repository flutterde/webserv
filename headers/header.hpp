/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 15:42:22 by ochouati          #+#    #+#             */
/*   Updated: 2025/01/26 12:51:16 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstdlib>
#include "Cont.hpp"

typedef unsigned int    uint32_t;
std::string	strTrim(std::string&str, std::string set);
std::vector<Cont>	readConfigFile(std::string& file);
void				readConfigs(std::ifstream& file);