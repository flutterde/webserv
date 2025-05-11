#include "./../../headers/Webserv.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <string>
#include <unistd.h>

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

// void badCharHeuristic(std::string pattern, size_t size, int badChar[CHARS_SIZE])
// {
// 	for (size_t i = 0; i < CHARS_SIZE; ++i)
// 		badChar[i] = -1;

// 	for (size_t i = 0; i < size; ++i)
// 		badChar[(size_t)pattern[i]] = i;
// }

// // the txt variable should stay string not string& because i pass a char[n] to it.
// int search(const std::string &txt, const std::string &pattern)
// {
// 	if (pattern.empty() || txt.empty())
// 		return -1;

// 	int badChar[CHARS_SIZE];

// 	int patternSize = pattern.size();
// 	int txtSize = txt.size();
// 	badCharHeuristic(pattern, patternSize, badChar);

// 	int skip = 0;
// 	int i = 0;
// 	int j;
// 	while (i <= txtSize - patternSize)
// 	{
// 		j = patternSize - 1;
// 		while (j >= 0 && pattern[j] == txt[i + j])
// 			--j;
// 		if (j < 0)
// 			return i;
// 		skip = std::max(1, j - badChar[(int)txt[i + j]]);
// 		i += skip;
// 	}
// 	return -1;
// }

/// get file name from a buffer request
std::string getFileName(const std::string &buffer){
	std::string fileName; 
	size_t i = buffer.find("filename=\"");
	if (i != std::string::npos) {
		for (i = i + 10; buffer[i] != '"' && buffer[i] ; ++i){
			fileName += buffer[i];
		}
		if(buffer[i] != '"')
			return "";
	}
	return fileName;
}

void closeFiles(ClientData &client)
{
    std::map<std::string, int>::iterator it;

    for (it = client.uploadFd.begin(); it != client.uploadFd.end(); ++it)
        close(it->second);
    client.uploadFd.clear();
}

//! set client.currentFileFd to -1
//! call setheader() function (I am not sure about the function name)
// You should put all tmp files in a temp folder
// you should close all the files  + you should reset the files
void processMultipartUpload(ClientData &client)
{
	ssize_t written;


    while(!client.request.empty()) {
        if (client.uploadFd.find(client.tmpFileName) == client.uploadFd.end()){
            size_t headers = client.request.find("\r\n\r\n");
            std::string	fileName;
            if (headers != std::string::npos) {
                fileName = getFileName(client.request);
                if (!fileName.empty()) {
                    client.tmpFileName = "upload_" + fileName;
                    if (client.uploadFd.find(client.tmpFileName) != client.uploadFd.end())
                        close(client.uploadFd[client.tmpFileName]);
                    client.uploadFd[client.tmpFileName] = open(client.tmpFileName.c_str() ,O_CREAT | O_TRUNC | O_WRONLY, 0644);
                    if (client.uploadFd[client.tmpFileName] == -1){
                        closeFiles(client);
                        break; //!  close all the files
                    }
                }
                client.request.erase(0, headers + 4);
            }
            else
                return;
        }

        std::cout << COL_RED << "uploading..." << END_COL << std::endl;

        if (client.request.find("\r") != std::string::npos){
            size_t boundaryPos = client.request.find("--" + client.boundary + "\r\n");
            size_t endBoundary;
            if (boundaryPos != std::string::npos) {
                if (client.uploadFd.find(client.tmpFileName) != client.uploadFd.end()){
                    written =  write(client.uploadFd[client.tmpFileName],
                                            client.request.c_str(),
                                            boundaryPos > 2 ? boundaryPos - 2: 0);
                    if (written == -1){
                        closeFiles(client);
                        return;
                    }
                }
                client.request.erase(0, boundaryPos + client.boundary.size() + 4);
                client.tmpFileName.clear();
                continue;
            }
            else if((endBoundary = client.request.find("--" + client.boundary + "--\r\n")) != std::string::npos){
                written =  write(client.uploadFd[client.tmpFileName],
                                            client.request.c_str(),
                                            endBoundary > 2 ? endBoundary - 2: 0);
                client.request.clear();
                client.tmpFileName.clear();
                closeFiles(client);
                // REQUEST_COMPLETE
            }
            else
                return;
        }

        if (client.uploadFd.find(client.tmpFileName) != client.uploadFd.end()) {
            written = write(client.uploadFd[client.tmpFileName], 
                            client.request.c_str(),
                            client.request.size());
            if (written == -1){
                closeFiles(client);
                return;
            }
            client.request.erase(0, written);
        }
    }
}
