/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cont.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 10:15:48 by ochouati          #+#    #+#             */
/*   Updated: 2025/01/26 12:39:40 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>

#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#endif

// Cont is a class that contain config file lines
class	Cont {
	private:
		bool		isStart;
		bool		isEnd;
		std::string	line;
	public:
		Cont();
		Cont(std::string& line, bool start, bool end);
		Cont(const Cont& obj);
		Cont&	operator=(const Cont& obj);
		~Cont();
	// Getters & Setters
	bool	getIsStart()	const;
	bool	getIsEnd()	const;
	std::string	getLine()	const;
	void	setIsStart(bool x);
	void	setIsEnd(bool x);
	void	setLine(std::string& x);
};
