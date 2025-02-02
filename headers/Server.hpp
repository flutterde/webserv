#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>

class Server
{
	private:

	public:
		Server(void); // Default constructor
		Server(const Server& obj); // Copy constructor
		Server&	operator=(const Server& obj); // Copy assignment operator
		~Server(); // Destructor

};

#endif
