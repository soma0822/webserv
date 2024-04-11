# -- program name --
NAME = webserv

# -- directories --
SRCDIR  = ./src
INCLUDE = $(addprefix -I, $(shell find $(SRCDIR) -type d))
OBJDIR  = obj

# -- files --
SRCS    = $(shell find $(SRCDIR) -type f -name '*.cpp')
HEADERS = $(shell find $(SRCDIR) -type f -name '*.hpp')
OBJS    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPENDS	= $(OBJS:.o=.d)

# -- compile flags --
CXX      = c++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98 -MMD -MP

# -- command define	--
RM = rm -rf

# Mandatory target ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	$(RM) $(OBJDIR)
	$(RM) $(BUILD_PATH)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(NAME_DEBUG)

re: fclean all


# Debug +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: debug

NAME_DEBUG    = $(NAME)_debug
DEBUG_FLAGS   = -fsanitize=address -fsanitize=bounds
OBJS_DEBUG    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.debug.o,$(SRCS))
DEPENDS_DEBUG = $(OBJS_DEBUG:.o=.d)

debug: $(NAME_DEBUG)

$(NAME_DEBUG): $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(INCLUDE) -o $@ $(OBJS_DEBUG)

$(OBJDIR)/%.debug.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(INCLUDE) -o $@ -c $<

# clang-format ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: format

format:
	clang-format --style=Google -i $(SRCS) $(HEADERS)
	clang-format --style=Google -i test/unit/*.cpp

# Test ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: test/unit test/e2e

BASE_DIR      = $(CURDIR)
BUILD_DIR     = build
BUILD_PATH    = $(BASE_DIR)/$(BUILD_DIR)
TEST_EXE_PATH = $(BUILD_PATH)/webserv_test

test: test/unit test/e2e

# Run all tests
test/unit:
	@# Ensure cmake runs to generate the test executable
	cmake -S $(BASE_DIR) -B $(BUILD_PATH)
	@# Build the project
	@cmake --build $(BUILD_PATH) || exit 1
	@# Execute the test
	@$(TEST_EXE_PATH) || exit 1

test/e2e: debug
	pytest ./test/e2e/pytest
	bash test/e2e/run.sh

# CI -+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: build

ci/build:
	docker build -t webserv .
	docker run --rm -it -v $(CURDIR):/app webserv -c "make re && make fclean"

# Clean log file -+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: logclean

LOGDIR = log

logclean:
	$(RM) $(LOGDIR)/*.log
# Permission change -++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: permission

PERMISSION_DIR = ./www/test
READ_ONLY = $(shell find $(PERMISSION_DIR) \( -type f -name 'read_only*' \) -o \( -type d -name 'read_only*' \))
WRITE_ONLY = $(shell find $(PERMISSION_DIR) \( -type f -name 'write_only*' \) -o \( -type d -name 'write_only*' \))
EXECUTE_ONLY = $(shell find $(PERMISSION_DIR) \( -type f -name 'execute_only*' \) -o \( -type d -name 'execute_only*' \))

permission:
	chmod 444 $(READ_ONLY)
	chmod 222 $(WRITE_ONLY)
	chmod 111 $(EXECUTE_ONLY)

permission-clean:
	chmod 755 $(READ_ONLY) $(WRITE_ONLY) $(EXECUTE_ONLY)
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#

-include $(DEPENDS) $(DEPENDS_DEBUG)
