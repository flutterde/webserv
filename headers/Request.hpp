/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:36:47 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/06 14:30:00 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Http.hpp"

class	Request : public Http {
	private:
		std::string	method;
		std::string	uri;
		bool		isCompleted;
		size_t		contentLength;

	public:
		Request(void);
		~Request(void);
		Request(const Request &src);
		Request &operator=(const Request &src);
		// Getters
		std::string&	getMethod(void) const;
		std::string&	getUri(void) const;
		std::map<std::string, std::string>&	getHeaders(void) const;
		std::string&	getBody(void) const;
		
};
