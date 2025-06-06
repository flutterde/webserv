#pragma once

# include <unistd.h>
# include <arpa/inet.h>
# include <algorithm>
# include <cstddef>
# include <iostream>
# include <vector>
# include <map>
# include <sstream>
# include <cstdlib>
# include <cctype>
# include <sys/socket.h>
# include <time.h>
#include <climits>
# include <sys/fcntl.h>
# include <fstream>
# include <dirent.h>
# include <string>
# include <cstring>
# include <netinet/in.h>
# include <poll.h>
# include <sys/stat.h>
# include <signal.h>
#include <cstddef>
#include <ostream>

# define FIRST_INDEX 0

# define SEND_SIZE 2048

# define FT_LIMIT_BODY_SIZE 80000
# define FT_PORT 8080

# define ALLOWED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;="

# define COL_RED "\033[0;91m"
# define COL_GREEN "\033[0;92m"
# define COL_YELLOW "\033[0;93m"
# define COL_BLUE "\033[0;94m"
# define COL_MAGENTA "\033[0;95m"
# define END_COL "\033[0m"

#ifndef SO_NOSIGPIPE
#define SO_NOSIGPIPE    0x1022
#endif

#define LISTEN_BACKLOG 2048
#define BODY_TEMP_PATH "./var/tmp"


# define PHP_CGI_PATH "/Users/achakkaf/Documents/webser42/configs/php-cgi" //! PHP CGI Path

# define RUNNING 1
# define POLL_TIMEOUT 50
# define END_OF_HEADERS "\r\n\r\n"
# define READ_SIZE 4096
# define URL_MAX_SIZE 2048
# define CHUNCK_SIZE 4096

# define DEFAULT_TIME_OUT 50

#define CHARS_SIZE 256


enum	requestType {
	NOT_SET = -1,
	CONTENT_LENGTH,
	CHUNKED,
	NO_CONTENT,
	MULTIPART_FORM,
};

enum requestProgress {
	NOT_STARTED = -1,
	WORKING,
	COLLECTED,
	READY,
};

enum RESPONSE_CODE {
	NOT_FOUND = 404,
	MOVED_PERMANENTLY = 301,
	METHOD_NOT_ALLOWED = 405,
	FORBIDDEN = 403,
	OK = 200,
	CGI,
	CREATED = 201,
	NOCONTENT = 204,
	INTERNAL_SERVER_ERROR = 500
};

typedef	unsigned int	uint32_t;
typedef unsigned char	uint8_t;


