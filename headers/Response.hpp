/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:24:11 by mboujama          #+#    #+#             */
/*   Updated: 2025/03/20 10:05:20 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <iostream>

class Response
{
	private:
		std::string http_version;
		int status_code;
		std::string status_text;
		std::string body;

	public:
		Response(void);
		Response(const Response& obj);
		Response&	operator=(const Response& obj);
		~Response();

		std::string getHttpVersion();
		int getStatusCode();
		std::string getStatusText();
		std::string getBody();

		void setHttpVersion(std::string version);
		void setStatusCode(int status);
		void setStatusText(std::string status);
		void setBody(std::string body);
};

#endif
