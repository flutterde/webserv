CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

NAME=webserv
SRCS=main.cpp srcs/models/Config.cpp srcs/models/Cont.cpp \
srcs/parsing/readConfigFile.cpp srcs/parsing/helpers/strTrim.cpp 

HEADERS=headers/*
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

re: fclean all

.PHONY: all clean fclean re