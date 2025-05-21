#include "../../headers/Webserv.hpp"


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

    std::cout << COL_RED << "uploading..." << END_COL << std::endl;
    while(!client.request.empty()) {
        if (client.uploadFd.find(client.tmpFileName) == client.uploadFd.end()){
            size_t headers = client.request.find("\r\n\r\n");
            if (headers != std::string::npos) {
                client.fileName = getFileName(client.request);
                if (!client.fileName.empty()) {
                    client.tmpFileName = client.server->getClientBodyTempPath() + "/upload_" + client.fileName; //! remove prefix & chan + add temp folder
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
                closeFiles(client);
                std::cout << COL_GREEN << "tmpfile: "<< client.tmpFileName.c_str() << END_COL << std::endl;
                std::cout << COL_GREEN << "dist: "<< client.server->getUploadsPath() + client.fileName << END_COL << std::endl;
                std::rename(client.tmpFileName.c_str(), (client.server->getUploadsPath() + client.fileName).c_str()); //! handle if it failed
                std::remove(client.tmpFileName.c_str());
                client.tmpFileName.clear();
                std::cout << COL_GREEN << "finish uploading..." << END_COL << std::endl;
        
                //! REQUEST_COMPLETE
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
