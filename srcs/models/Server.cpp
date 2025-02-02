#include "../../headers/Server.hpp"

Server::Server(void)
{
	std::cout << "Server Default constructor called" << std::endl;
}

Server::~Server(void)
{
	std::cout << "Server Destructor called" << std::endl;
}

Server::Server(const Server& obj)
{
	std::cout << "Server Copy constructor called" << std::endl;
	*this = obj;
}

Server& Server::operator=(const Server& obj)
{
	std::cout << "Server Copy assignment operator called" << std::endl;
	if (this != &obj)
	{
		// Add any assignment logic here
	}
	return (*this);
}
