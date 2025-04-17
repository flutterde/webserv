/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:29:43 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/15 13:08:49 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ResponseUtils.hpp"

std::string ResponseUtils::getDateTime() {
	time_t rawtime;
	struct tm * datetime;
	char buffer [80];

	time (&rawtime);
	datetime = localtime(&rawtime);

	strftime (buffer,80,"%a, %d %h %Y %T %Z", datetime);
	return std::string(buffer);
}

std::string ResponseUtils::allowHeaderValue(std::map<std::string, bool> allowedMethods) {
	std::ostringstream oss;
	bool first = true;

	std::map<std::string, bool>::iterator it;
	for (it = allowedMethods.begin(); it != allowedMethods.end(); it++) {
		if (it->second) {
			if (!first) oss << ", ";
			oss << it->first;
			first = false;
		}
	}
	return oss.str();
}

bool ResponseUtils::pathExists(const std::string& path) {
    struct stat info;

	if (stat(path.c_str(), &info) != 0)
		return 0;
    return 1;
}