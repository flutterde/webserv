/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:11 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/13 16:08:23 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include "../learning/request.hpp"
# include "WebservHandler.hpp"
# include <time.h>

class Response
{
	private:
		Response(const Response& obj);
		Response&	operator=(const Response& obj);
		~Response();
		
		std::string http_version;
		int status_code;
		std::string status_text;
		std::map<std::string, std::string> headers;
		std::string body;
		std::string date;
		
		std::string combineResponse();
		void printResHeaders(void) const;
		void handleGet(struct ClientData &client, const Request &req);
		void handlePost(struct ClientData &client, const Request &req);
		void handleDelete(struct ClientData &client, const Request &req);
	public:
		Response(void);
		std::string handleResponse( struct ClientData &clientData , const Request &request);
};	

#endif
