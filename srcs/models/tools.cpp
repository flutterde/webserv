// #include "./../../headers/Webserv.hpp"
#include <string>
// #include <iostream>
#include <algorithm>

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

// the txt variable should stay string not string& because i pass a char[n] to it.
int search(std::string txt, std::string &pattern)
{
	if (pattern.empty() || txt.empty())
		return -1;

	int badChar[CHARS_SIZE];

	int patternSize = pattern.size();
	int txtSize = txt.size();
	badCharHeuristic(pattern, patternSize, badChar);

	int skip = 0;
	int i = 0;
	int j;
	while (i <= txtSize - patternSize)
	{
		j = patternSize - 1;
		while (j >= 0 && pattern[j] == txt[i + j])
			--j;
		if (j < 0)
			return i;
		skip = std::max(1, j - badChar[txt[i + j]]);
		i += skip;
	}
	return -1;
}
