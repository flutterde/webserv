/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:49:53 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/21 18:02:10 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "Server.hpp"
#include <sys/poll.h>
# include <vector>
# include <poll.h>
# include <unistd.h>

# define RUNNING 1
# define POLL_TIMEOUT 0

class Webserv {
	private:
		std::vector<Server>			_servers;
		std::vector<struct pollfd>	_pollfds;
		std::vector<int>			_fds;
		int							_nbrEvents;
		void					_closeClients();

	public:
		Webserv();
		~Webserv();
		void	run();

};
