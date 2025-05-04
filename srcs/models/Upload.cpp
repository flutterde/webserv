#include "./../../headers/Webserv.hpp"
#include <cstdio>
#include <fstream>
#include <string>
// #include <iostream>
#include <algorithm>
#include <unistd.h>
#include <vector>

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
int search(const std::string &txt, const std::string &pattern)
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
		skip = std::max(1, j - badChar[(int)txt[i + j]]);
		i += skip;
	}
	return -1;
}

/// get file name from a buffer request
std::string getFileName(const std::string &buffer){
	std::string fileName;
	int i = search(buffer, "filename=\"");
	if (i != -1) {
		for (i = i + 10; buffer[i] != '"' && buffer[i] ; ++i){
			fileName += buffer[i];
		}
		// std::cout << COL_RED << fileName << END_COL << std::endl;
		if(buffer[i] != '"')
			return "";
	}
	return fileName;
}

// You should put all tmp files in a temp
void processMultipartUpload(ClientData &client)
{
	std::string	fileName;
	size_t			firstBoundary;
	
	firstBoundary = client.request.find("\r\n\r\n");
	if (firstBoundary != std::string::npos){
		fileName = getFileName(client.request);
		if (!fileName.empty()) {
			if (client.files.find(client.tmpFileName) != client.files.end() && 
				client.files[client.tmpFileName] != -1) {
					close(client.files[client.tmpFileName]);
			}
			client.tmpFileName = "upload_" + fileName;
			client.files[client.tmpFileName] = open(client.tmpFileName.c_str() ,O_CREAT | O_TRUNC | O_WRONLY, 0644);
			if (client.files[client.tmpFileName] == -1)
                return;
		}
		client.request.erase(0, firstBoundary + 4);
		// std::cout << COL_GREEN <<  client.request <<END_COL<< std::endl;
		// std::cout << "|" << client.tmpFileName << "|" << client.files[client.tmpFileName] << " HHAHAHHAHAHHAAHHAHH\n" ;
		size_t secondBoundary = client.request.find("--" + client.boundary);
		if (secondBoundary != std::string::npos){
			if (client.files[client.tmpFileName] != -1)
				write(client.files[client.tmpFileName],client.request.c_str(),client.request.size() - secondBoundary);
			client.request.erase(0, secondBoundary + 2);
			// processMultipartUpload(client);
		}
	}

	// if (search(client.request, "\r\n\r\n") != -1)
	// 	return;

	// write(client.files[client.tmpFileName], client.request.c_str(), client.request.size());
	// std::cout << std::endl << COL_BLUE << " --------------------------------- " << END_COL << std::endl; //! remove this
	// std::cout << "|" << client.request << "|";
	// std::cout << std::endl << COL_BLUE << " --------------------------------- " << END_COL << std::endl; //! remove this
	// client.request.clear();


	// if (fileName.empty()){
	// 	write(client.files[client.tmpFileName], client.request.c_str(), client.request.size());
	// 	client.request.clear();
	// 	return;
	// }
	// std::cout << COL_GREEN <<  fileName << END_COL << std::endl;
}

