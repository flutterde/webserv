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
// rename it to tooles;
# include "Types.hpp"
#include "WebservHandler.hpp"
// #include "Cgi.hpp"
#include "ClientData.hpp"
#include "debug.hpp"
#include "FtPars.hpp"

extern char				**serverEnv;
extern std::string		*notFound;
extern WebservHandler	*wServ;

std::string&	strTrim(std::string&str, std::string set); //!
void 			processMultipartUpload(ClientData &client);
