/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:07:27 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/02 11:11:36 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <poll.h>

	uint32_t	ftInetPton(const std::string& str)//!< Convert an IPv4 address from its text representation to binary form
	{
		std::vector<std::string> arr;
		std::stringstream ss(str);
		uint32_t	ip = 0;
		uint8_t		byte;

		while (ss.good())
		{
			std::string substr;
			getline(ss, substr, '.');
			arr.push_back(substr);
		}
		if (arr.size() != 4)
			return (0);
		 for (size_t i = 0; i < arr.size(); i++) {
        byte = std::atoi(arr[i].c_str());
        ip |= byte << (24 - (i * 8));  // Shift each byte into the correct position
    }
		return (ip);
	}

int	main()
{
	std::string	ip = "246.88.246.216";
	uint32_t	ipVal = ftInetPton(ip);
	if (4133025496 == ipVal)
		std::cout << "Yesssssssssss" << std::endl;
	else
		std::cout << "Noooooooooooo" << std::endl;
	std::cout << "Val: " << ipVal << std::endl;
	return (0);
}

