/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:20:57 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/14 12:25:39 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/ClientData.hpp"
#include "../../headers/Response.hpp"

ClientData::~ClientData()
{
	delete resp, resp = NULL;
}

