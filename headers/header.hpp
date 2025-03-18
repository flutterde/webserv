/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:37:08 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/13 15:00:40 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
#include "Request.hpp"
#include "Response.hpp"
# include <sys/socket.h>
# include <unistd.h>
# define FIRST_INDEX 0
# define END_OF_HEADERS "\r\n\r\n"

std::string&	strTrim(std::string&str, std::string set); //!