NAME = ircserv
C++ = c++
C++_FLAGS = -Wall -Wextra -Werror -std=c++98 -Iincludes

FILES = src/main.cpp \
		src/Channel.cpp \
		src/Client.cpp \
		src/Commands.cpp \
		src/Message.cpp \
		src/Server.cpp \
		src/Socket.cpp \
		src/Utils.cpp

OBJFILES = $(FILES:.cpp=.o)

all: $(NAME)

$(NAME):$(OBJFILES)
	$(C++) $(C++_FLAGS) $(OBJFILES) -o $(NAME)

%.o: %.cpp
	$(C++) $(C++_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJFILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re