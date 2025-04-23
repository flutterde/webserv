/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:35:38 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/23 14:40:43 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <map>

class	MimeTypes {
	private:
		std::map<std::string, std::string> types;
	public:
		MimeTypes();
		~MimeTypes();
		std::string getMimeType(std::string filepath);
};

