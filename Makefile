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

NAME_DEBUG  = $(NAME)_debug
DEBUG_FLAGS = -fsanitize=address -fsanitize=bounds
OBJS_DEBUG  = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o.debug,$(SRCS))

debug: $(NAME_DEBUG)

$(NAME_DEBUG): $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(INCLUDE) -o $@ $(OBJS_DEBUG)

$(OBJDIR)/%.o.debug: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(INCLUDE) -o $@ -c $<

# clang-format ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: format

format:
	clang-format --style=Google -i $(SRCS) $(HEADERS)
	clang-format --style=Google -i test/*.cpp

# Test ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: test

BASE_DIR      = $(CURDIR)
BUILD_DIR     = build
BUILD_PATH    = $(BASE_DIR)/$(BUILD_DIR)
TEST_EXE_PATH = $(BUILD_PATH)/webserv_test

# Run all tests
test:
	@# Ensure cmake runs to generate the test executable
	cmake -S $(BASE_DIR) -B $(BUILD_PATH)
	@# Build the project
	@cmake --build $(BUILD_PATH) || exit 1
	@# Execute the test
	@$(TEST_EXE_PATH) || exit 1

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

-include $(DEPENDS)
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
