#include <iostream>

bool isCgi = true;
std::string body = "HTTP/1.1 200 OK\r\n"
        "\r\n"
        "</html>";

size_t getBodyLength() {
    if (isCgi) {
        return body.length() - body.find("\r\n\r\n") - 4;
    }
    return body.length();
}

int main() {
    std::cout << getBodyLength() << std::endl;
    return 0;
}