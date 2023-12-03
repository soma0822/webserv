SERVER_NAME = server
SERVER_SRCS = srcs/server.cpp
SERVER_OBJS = $(patsubst srcs/%.cpp,objs/%.o,$(SERVER_SRCS))

CLIENT_NAME = client
CLIENT_SRCS = srcs/client.cpp
CLIENT_OBJS = $(patsubst srcs/%.cpp,objs/%.o,$(CLIENT_SRCS))

DAEMON_NAME = daemon
DAEMON_SRCS = srcs/daemon.cpp
DAEMON_OBJS = $(patsubst srcs/%.cpp,objs/%.o,$(DAEMON_SRCS))

SIGNALTEST_NAME = signaltest
SIGNALTEST_SRCS = srcs/signaltest.cpp
SIGNALTEST_OBJS = $(patsubst srcs/%.cpp,objs/%.o,$(SIGNALTEST_SRCS))

CC = c++
CFLAGS = -Wall -Wextra -Werror -I./include -DUNIT_TEST -DUSE_SIGNAL
RM = rm -rf

all: $(SERVER_NAME) $(CLIENT_NAME) $(DAEMON_NAME) $(SIGNALTEST_NAME)

$(SERVER_NAME): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS)

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_NAME) $(CLIENT_OBJS)

$(DAEMON_NAME): $(DAEMON_OBJS)
	$(CC) $(CFLAGS) -o $(DAEMON_NAME) $(DAEMON_OBJS)

$(SIGNALTEST_NAME): $(SIGNALTEST_OBJS)
	$(CC) $(CFLAGS) -o $(SIGNALTEST_NAME) $(SIGNALTEST_OBJS)

objs/%.o: srcs/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) objs

fclean: clean
	$(RM) $(SERVER_NAME) $(CLIENT_NAME) $(DAEMON_NAME) $(SIGNALTEST_NAME)

re: fclean all

.PHONY: all clean fclean re
