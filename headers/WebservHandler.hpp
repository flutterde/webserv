/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:37:19 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/14 12:24:03 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "Types.hpp"
#include "ClientData.hpp"

class WebservHandler
{
	protected:
		bool						isRequestValid(ClientData& client);
		bool						isHeaderComplete(ClientData& client);
		bool						isRequestComplete(ClientData& client);
		void						setRequestType(ClientData& client);
		void						setContentLength(ClientData& client);
		void						handleRequest(ClientData& client);
		void						_closeClient(int fd);
		void						setBoundary(ClientData& client);
		void						setMethod(ClientData& client);

		//* Validate the request
		void						validateRequestHeaders(ClientData& client);
		void						validateUrl(ClientData& client);

		std::vector<Server>			_servers;
		std::vector<struct pollfd>	_pollfds;
		std::map<int, ClientData>	_requests;

	public:
		WebservHandler();
		~WebservHandler();
		void 	enablePOLLOUT(int fd);
};

