#include <iostream>
#include <sys/stat.h>

#define FILE_PATH "/Users/ochouati/goinfre/tsts.txt" //? add file or directory path here!!

int main() {
	struct stat fileStat;

	if (stat(FILE_PATH, &fileStat) == -1) {
		std::cerr << "Some error or we should close the conn, or return server error" << std::endl;
		return 1;
	}
	std::cout << "file size: " << fileStat.st_size << " bytes" << std::endl;
	std::cout << "is directory: " << (S_ISDIR(fileStat.st_mode) ? "Yes" : "No") << std::endl;
	std::cout << "is regular file: " << (S_ISREG(fileStat.st_mode) ? "Yes" : "No") << std::endl;
}
