#include "../../headers/Webserv.hpp"
#include "../../headers/HttpErrors.hpp"

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
}

void processMultipartUpload(ClientData &client)
{
	ssize_t written;
    std::string tmpFileName;

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
                    tmpFileName = client.server->getClientBodyTempPath() + "/upload_" + client.fileName;
                    if (client.uploadFd.find(client.fileName) != client.uploadFd.end())
                        close(client.uploadFd[client.fileName]);
                    client.uploadFd[client.fileName] = open(tmpFileName.c_str() ,O_CREAT | O_TRUNC | O_WRONLY, 0644);
                    if (client.uploadFd[client.fileName] == -1){
                        closeFiles(client);
                        break;
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
                    std::rename(tmpFileName.c_str(), (client.server->getUploadsPath() + it->first).c_str());
                    std::remove(tmpFileName.c_str());
                }
                client.request.clear();
                closeFiles(client);
                client.fileName.clear();
                client.uploadFd.clear();
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
