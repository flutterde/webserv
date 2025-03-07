/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:07:27 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/07 17:31:08 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <netinet/in.h>
// #include <arpa/inet.h>

#define PORT 8080

void	printHeader(std::string header)
{
	std::vector<std::string> headers;
	std::istringstream iss(header);
	std::string line;

	while (getline(iss, line, '\n'))
		headers.push_back(line);
	for (size_t i = 0; i < headers.size(); i++)
		std::cout << headers[i] << std::endl;
}


int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./simple <port>" << std::endl;
		return (1);
	}
	int port = std::stoi(av[1]);
	struct sockaddr_in	addr;
	int		fsock_d = socket(AF_INET, SOCK_STREAM, 0);
	if (fsock_d == -1)
	{
		std::cerr << "socket failed" << std::endl;
		return (1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	int bind_res = bind(fsock_d, (struct sockaddr *)&addr, sizeof(addr));
	if (bind_res == -1)
	{
		std::cerr << "bind failed" << std::endl;
		return (1);
	}
	int listen_res = listen(fsock_d, 10);
	if (listen_res == -1)
	{
		std::cerr << "listen failed" << std::endl;
		return (1);
	}
	struct pollfd fds[1];
	fds[0].fd = fsock_d;
	fds[0].events = POLLIN;
	while (1)
	{
		int poll_res = poll(fds, 1, -1);
		if (poll_res == -1)
		{
			std::cerr << "poll failed" << std::endl;
			return (1);
		}
		if (fds[0].revents & POLLIN)
		{
			int csock_d = accept(fsock_d, NULL, NULL);
			if (csock_d == -1)
			{
				std::cerr << "accept failed" << std::endl;
				return (1);
			}
			char buffer[1024] = {0};
			int read_res;
			while ((read_res = read(csock_d, buffer, 1024)) > 0)
			{
				std::string header(buffer);
				printHeader(header);
				if (header.find("GET / HTTP/1.1") != std::string::npos)
					break;
				bzero(buffer, 1024);
			}
			
			std::ifstream index("./index.html");
			if (!index.is_open())
			{
				std::cerr << "index.html failed" << std::endl;
				return (1);
			}
			std::string file;
			std::string line;
			while (getline(index, line))
				file += line;
			// return ./index.html file
			std::string len = std::to_string(file.size());
			std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + len + "\n\n" + file;
			send(csock_d, response.c_str(), response.size(), 0);
			close(csock_d);
		}
	}
	return (0);
}

