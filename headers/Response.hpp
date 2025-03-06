/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:45:52 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/06 14:17:35 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Http.hpp"

typedef unsigned int uint32;

class	Response : public Http {
	private:
		uint32	_statusCode;

	public:
		Response(void);
		~Response(void);
		Response(const Response &src);
		Response &operator=(const Response &src);
};
