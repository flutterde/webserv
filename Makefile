CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -I./headers/  -fsanitize=address

NAME=webserv

# OTM
SRCS=main.cpp srcs/parsing/config/readConfig.cpp srcs/parsing/helpers/strTrim.cpp \
srcs/parsing/helpers/FtPars.cpp srcs/models/Server.cpp  debug/printing.cpp \
srcs/utils/httpResponseErrors.cpp srcs/utils/serverUtils.cpp srcs/utils/ClientData.cpp \

# ACHAKKAF
SRCS +=  srcs/cgi/ft_cgi.cpp srcs/models/Upload.cpp learning/request.cpp

# MOAD
SRCS +=  srcs/models/Response.cpp srcs/models/ResponseUtils.cpp srcs/models/MimeTypes.cpp srcs/models/Cgi.cpp

#OTM
SRCS += srcs/models/Webserv.cpp srcs/models/WebservHandler.cpp

HEADERS=headers/*.hpp
OBJS=$(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
tests:
	bash test/test01.sh

re: fclean all

.PHONY: all clean fclean re