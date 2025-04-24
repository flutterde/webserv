// #include "./../../headers/Webserv.hpp"
#include <string>
#include <iostream>

/*-------- LARGE FILES ALGORITHM: --------*/
//	 Receive chunk → store in buffer
//	 Scan for --boundary123:
//	 	Found:
//	 		Save previous data to temp file
//	 		Parse headers for new filename
//	 		Start new temp file
//	 	Not found: Append to current temp file
//	 On --boundary123--:
//	 	Finalize last file
//	 	Delete partial data if any
// 	Only after ALL boundaries processed:
//	 	Rename temp files to final names
/*-----------------------------------------*/
#define CHARS_SIZE 256

void badCharHeuristic(std::string pattern, size_t size, int badChar[CHARS_SIZE])
{
	for (size_t i = 0; i < CHARS_SIZE; ++i)
		badChar[i] = -1;

	for (size_t i = 0; i < size; ++i)
		badChar[(size_t)pattern[i]] = i;
}
//! NOT WORKING
int search(std::string txt, std::string &pattern)
{
	int badChar[CHARS_SIZE];
	size_t patternSize = txt.size();
	size_t txtSize = txt.size();

	// std::vector<int> matchIndexes;

	badCharHeuristic(pattern, patternSize, badChar);

	size_t i = patternSize;
	size_t j = patternSize;
	while (i < txtSize)
	{
		if (pattern[j] == txt[i])
		{
			if (j == 0)
				return i;
			--i;
			--j;
		}
		else
		{
			if (badChar[(size_t)txt[i]] > 0)
				i += badChar[(size_t)txt[i]];
			else 
				i += patternSize;
			j = patternSize;
		}
	}
	return -1;
}

int main()
{
	std::string txt = "PIGBCTDOGCAT";
	std::string pattern = "CAT";
	std::cout << search(txt, pattern) << std::endl;
}