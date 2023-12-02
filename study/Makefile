NAME = server
SRCS = srcs/server.cpp
OBJS = $(patsubst srcs/%.cpp,objs/%.o,$(SRCS))

CC = c++
CFLAGS = -Wall -Wextra -Werror -I./include
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

objs/%.o: srcs/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
