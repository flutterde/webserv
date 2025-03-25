/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:35:38 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/25 16:49:50 by ochouati         ###   ########.fr       */
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
};

