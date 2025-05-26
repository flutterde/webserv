#include "../../headers/Webserv.hpp"
#include "../../headers/HttpErrors.hpp"

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
    // client.uploadFd.clear();
}

//! set client.currentFileFd to -1
//! call setheader() function (I am not sure about the function name)
// You should put all tmp files in a temp folder
// you should close all the files  + you should reset the files
void processMultipartUpload(ClientData &client)
{
	ssize_t written;
    std::string tmpFileName;

    // std::cout << COL_RED << "uploading..." << END_COL << std::endl;
    while(!client.request.empty()) {
        if (client.uploadFd.find(client.fileName) == client.uploadFd.end()){
            size_t headers = client.request.find("\r\n\r\n");
            if (headers != std::string::npos) {
                client.fileName = getFileName(client.request);
                if (!client.fileName.empty() && !client.server->getEnableUploads()){
                    client.isRequestComplete = true;
                    HttpErrors::httpResponse403(client);
                }
                if (!client.fileName.empty()) {
                    tmpFileName = client.server->getClientBodyTempPath() + "/upload_" + client.fileName; //! remove prefix & chan + add temp folder
                    if (client.uploadFd.find(client.fileName) != client.uploadFd.end())
                        close(client.uploadFd[client.fileName]);
                    client.uploadFd[client.fileName] = open(tmpFileName.c_str() ,O_CREAT | O_TRUNC | O_WRONLY, 0644);
                    if (client.uploadFd[client.fileName] == -1){
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
                if (client.uploadFd.find(client.fileName) != client.uploadFd.end()){
                    written =  write(client.uploadFd[client.fileName],
                                            client.request.c_str(),
                                            boundaryPos > 2 ? boundaryPos - 2: 0);
                    if (written == -1){
                        closeFiles(client);
                        return;
                    }
                }
                client.request.erase(0, boundaryPos + client.boundary.size() + 4);
                client.fileName.clear();
                continue;
            }
            else if((endBoundary = client.request.find("--" + client.boundary + "--\r\n")) != std::string::npos){
                written =  write(client.uploadFd[client.fileName],
                                            client.request.c_str(),
                                            endBoundary > 2 ? endBoundary - 2: 0);
                for(std::map<std::string, int>::iterator it = client.uploadFd.begin(); it != client.uploadFd.end(); ++it){
                    tmpFileName = client.server->getClientBodyTempPath() + "/upload_" + it->first;
                    std::cout << COL_GREEN << "tmpfile: "<< tmpFileName << END_COL << std::endl;
                    std::cout << COL_GREEN << "dist: "<< client.server->getUploadsPath() + it->first << END_COL << std::endl;
                    std::rename(tmpFileName.c_str(), (client.server->getUploadsPath() + it->first).c_str()); //! handle if it failed
                    std::remove(tmpFileName.c_str());
                    std::cout << COL_GREEN << "finish uploading..." << END_COL << std::endl;
                }
                client.request.clear();
                closeFiles(client);
                client.fileName.clear();
                client.uploadFd.clear();
                //! REQUEST_COMPLETE
            }
            else
                return;
        }

        if (client.uploadFd.find(client.fileName) != client.uploadFd.end()) {
            written = write(client.uploadFd[client.fileName], 
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
