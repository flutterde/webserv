

#include "../../../headers/header.hpp"
#include "../../../headers/Config.hpp"
#include <fstream>

//----> test
// 1.read line by line
// 2.ignore if it start with (#)
// 3.for now just print data in stdout

static bool	newServerDDetected(std::string& line)
{
	if (line[0] == '[' && (line[line.length() - 1] == ']'))
		return (true);
	return (false);
}

void    readConfigs(std::ifstream& file)
{
	std::string	line;
	while (std::getline(file, line))
	{
		line = strTrim(line, " \n\t");
		if (line[0] == '#')
			continue;
		else if (newServerDDetected(line))
		{
			std::cout << " ============================================ \n";
			std::cout << line << std::endl;
		} else
			std::cout << line << std::endl;
	}
}
