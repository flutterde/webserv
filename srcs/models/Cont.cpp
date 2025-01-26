/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cont.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 10:07:22 by ochouati          #+#    #+#             */
/*   Updated: 2025/01/26 12:40:54 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Cont.hpp"

static void	_printing(std::string str)
{
	std::cout << str << " called." << std::endl;
}

Cont::Cont()
{
	if (DEBUG_MODE)
		_printing("Cont Default Constructor");
}

Cont::Cont(std::string& line, bool start, bool end)
{
	if (DEBUG_MODE)
		_printing("Cont Parametrize Constructor");
	this->line = line;
	this->isStart = start;
	this->isEnd = end;
}

Cont::Cont(const Cont& obj)
{
	(void)obj;
	//!
}

Cont&	Cont::operator=(const Cont& obj)
{
	(void)obj;
	//!
	return (*this);
}

Cont::~Cont()
{
	if (DEBUG_MODE)
		_printing("Cont Destructor");
}

bool	Cont::getIsStart()	const
{
	return (this->isStart);
}

bool	Cont::getIsEnd()	const
{
	return (this->isEnd);
}

std::string	Cont::getLine()	const
{
	return (this->line);
}

void	Cont::setIsStart(bool x)
{
	this->isStart = x;
}

void	Cont::setIsEnd(bool x)
{
	this->isEnd = x;
}

void	Cont::setLine(std::string& x)
{
	this->line = x;
}
