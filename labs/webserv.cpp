/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:04:35 by ochouati          #+#    #+#             */
/*   Updated: 2025/02/21 19:15:22 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8081

int main() {
	int sockfd, bindNbr, listenNbr, newsockfd;

	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cerr << "Error opening socket" << std::endl;
		exit(1);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);
	bindNbr = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if (bindNbr < 0) {
		std::cerr << "Error on binding" << std::endl;
		exit(1);
	}
	listenNbr = listen(sockfd, 5);
	if (listenNbr < 0) {
		std::cerr << "Error on listening" << std::endl;
		exit(1);
	}
	newsockfd = accept(sockfd, NULL, NULL);
	if (newsockfd < 0) {
		std::cerr << "Error on accept" << std::endl;
		exit(1);
	}
	write(newsockfd, "Hello, world!\n", 13);
	close(newsockfd);
	close(sockfd);
	return 0;
}
