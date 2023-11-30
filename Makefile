BASE_DIR := $(CURDIR)
BUILD_DIR := build
BUILD_PATH := $(BASE_DIR)/$(BUILD_DIR)
TEST_EXE_PATH := $(BUILD_PATH)/example_test # Rename test executable

# Default target
.PHONY: all
all: test

# Run all tests
.PHONY: test
test:
	@# Ensure cmake runs to generate the test executable
	cmake -S $(BASE_DIR) -B $(BUILD_PATH)
	@# Build the project
	@cmake --build $(BUILD_PATH) || exit 1
	@# Execute the test
	@$(TEST_EXE_PATH) || exit 1

# Delete tests related files
.PHONY: test/clean
test/clean:
	$(RM) -r $(BUILD_PATH)
