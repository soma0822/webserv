SERVER_NAME = server
SERVER_SRCS = srcs/server.cpp
SERVER_OBJS = $(patsubst srcs/%.cpp,objs/%.o,$(SERVER_SRCS))

CLIENT_NAME = client
CLIENT_SRCS = srcs/client.cpp
CLIENT_OBJS = $(patsubst srcs/%.cpp,objs/%.o,$(CLIENT_SRCS))

CC = c++
CFLAGS = -Wall -Wextra -Werror -I./include
RM = rm -f

all: $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS)

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_NAME) $(CLIENT_OBJS)

objs/%.o: srcs/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
