/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:47:27 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/23 14:42:30 by mboujama         ###   ########.fr       */
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
	types[".css"] = "text/css";
	types[".js"] = "text/javascript";
	types[".pdf"] = "application/pdf";
	types[".zip"] = "application/zip";
	types[".tar"] = "application/x-tar";
}

MimeTypes::~MimeTypes()
{}

std::string MimeTypes::getMimeType(std::string filepath) {
	return types[filepath.substr(filepath.find_last_of('.'))];
}