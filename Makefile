NAME = ircserv
C++ = c++
C++_FLAGS = -Wall -Wextra -Werror -std=c++98

FILES = main.cpp \
		Channel.cpp \
		Client.cpp \
		Commands.cpp \
		Message.cpp \
		Server.cpp \
		Socket.cpp \
		Utils.cpp

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