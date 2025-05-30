CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -I./headers/  

NAME=webserv

SRCS=main.cpp srcs/parsing/config/readConfig.cpp srcs/parsing/helpers/strTrim.cpp \
srcs/parsing/helpers/FtPars.cpp srcs/models/Server.cpp \
srcs/utils/httpResponseErrors.cpp srcs/utils/serverUtils.cpp srcs/utils/ClientData.cpp \

SRCS +=   srcs/models/Upload.cpp srcs/models/Request.cpp

SRCS +=  srcs/models/Response.cpp srcs/models/ResponseUtils.cpp srcs/models/MimeTypes.cpp srcs/models/Cgi.cpp

SRCS += srcs/models/Webserv.cpp srcs/models/WebservHandler.cpp

HEADERS=headers/*.hpp
OBJS=$(SRCS:.cpp=.o)

all: $(NAME)
	@mkdir -p ./var/tmp ./var/www/html/main/uploads

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

up:
	docker compose up -d --build

down:
	docker compose down

exec:
	docker compose exec webserv bash

re: fclean all

.PHONY: all clean fclean re