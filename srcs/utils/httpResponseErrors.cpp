/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponseErrors.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:10:39 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/25 11:02:54 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/HttpErrors.hpp"
#include "./../../headers/header.hpp"


void	HttpErrors::httpResponse414(ClientData& client) {
	std::string response = "HTTP/1.1 414 Request-URI Too Long\r\n"
						   "Content-Type: text/html\r\n"
						   "Content-Length: 59\r\n"
						   "\r\n<html><body><h1>414 Request-URI Too Long</h1></body></html>";
	std::cout << COL_RED << response << END_COL << std::endl;
	client.error = response;
	client.progress = READY;
	wServ->enablePOLLOUT(client.fd);
}

void	HttpErrors::httpResponse400(ClientData& client) {
	std::cout << " ************************* HTTP 400 ERROR ************************* " << std::endl;
	std::string response = "HTTP/1.1 400 Bad Request\r\n"
						   "Content-Type: text/html\r\n"
						   "Content-Length: 50\r\n"
						   "\r\n<html><body><h1>400 Bad Request</h1></body></html>";
	std::cout << COL_RED << response << END_COL << std::endl;
	client.error = response;
	client.progress = READY;
	wServ->enablePOLLOUT(client.fd);
}

void	HttpErrors::httpResponse413(ClientData& client) {
	std::string response = "HTTP/1.1 413 Payload Too Large\r\n"
						   "Content-Type: text/html\r\n"
						   "Content-Length: 56\r\n"
						   "\r\n<html><body><h1>413 Payload Too Large</h1></body></html>";
	std::cout << COL_RED << response << END_COL << std::endl;
	client.error = response;
	client.progress = READY;
	wServ->enablePOLLOUT(client.fd);
}

void	HttpErrors::httpResponse405(ClientData& client) {
	std::string response = "HTTP/1.1 405 Method Not Allowed\r\n"
						   "Content-Type: text/html\r\n"
						   "Content-Length: 57\r\n"
						   "\r\n<html><body><h1>405 Method Not Allowed</h1></body></html>";
	std::cout << COL_RED << response << END_COL << std::endl;
	client.error = response;
	client.progress = READY;
	wServ->enablePOLLOUT(client.fd);
}

void	HttpErrors::httpResponse403(ClientData& client) {
	std::string response = "HTTP/1.1 403 Forbidden\r\n"
						   "Content-Type: text/html\r\n"
						   "Content-Length: 48\r\n"
						   "\r\n<html><body><h1>403 Forbidden</h1></body></html>";
	std::cout << COL_RED << response << END_COL << std::endl;
	client.error = response;
	client.progress = READY;
	wServ->enablePOLLOUT(client.fd);
}
