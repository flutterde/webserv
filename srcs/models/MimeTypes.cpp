/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:47:27 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/26 12:17:35 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/MimeTypes.hpp"


MimeTypes::MimeTypes()
{
	types[".mp4"] = "video/mp4";
	types[".mp3"] = "audio/mpeg";
	types[".jpg"] = "image/jpeg";
	types[".jpeg"] = "image/jpeg";
	types[".png"] = "image/png";
	types[".gif"] = "image/gif";
	types[".html"] = "text/html";
	types[".txt"] = "text/plain";
	types[".css"] = "text/css";
	types[".js"] = "text/javascript";
	types[".pdf"] = "application/pdf";
	types[".zip"] = "application/zip";
	types[".tar"] = "application/x-tar";
	types[".json"] = "application/json";
	types[".xml"] = "application/xml";
	types[".svg"] = "image/svg+xml";
	types[".webp"] = "image/webp";
	types[".woff"] = "font/woff";
	types[".woff2"] = "font/woff2";
	types[".eot"] = "application/vnd.ms-fontobject";
	types[".otf"] = "font/otf";
	types[".ttf"] = "font/ttf";
	types[".csv"] = "text/csv";
	types[".md"] = "text/markdown";
	types[".avi"] = "video/x-msvideo";
	types[".webm"] = "video/webm";
	types[".ico"] = "image/x-icon";
	
}

MimeTypes::~MimeTypes()
{}

std::string MimeTypes::getMimeType(std::string filepath) {
	int dot = filepath.find_last_of('.');
	if (dot == -1) 
		return "plain/text";
	return types[filepath.substr(dot)];
}