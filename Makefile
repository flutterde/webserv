CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -I./headers/

NAME=webserv
SRCS=main.cpp srcs/parsing/config/readConfig.cpp srcs/parsing/helpers/strTrim.cpp \
srcs/parsing/helpers/FtPars.cpp srcs/models/Server.cpp  debug/printing.cpp 

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
test:
	bash test/test01.sh

re: fclean all

.PHONY: all clean fclean re