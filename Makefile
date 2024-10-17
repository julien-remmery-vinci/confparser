# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm  # Add any additional libraries here if needed

# Directories
SRC_DIR = src
TEST_DIR = tests
LIB_DIR = lib
BUILD_DIR = build
BIN_DIR = bin
LOGS_DIR = logs
INCLUDE_DIRS = -Iinclude $(shell find $(LIB_DIR) -type d -name include | sed 's/^/-I/')

# Find all .c files in src and lib directories
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
LIB_SRC_FILES = $(shell find $(LIB_DIR) -type f -name "*.c")
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

# Output test executable in the bin/ directory
TEST_TARGET = $(BIN_DIR)/test

# Object files
OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES) $(LIB_SRC_FILES))
TEST_OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(TEST_FILES))

# Default target: build the test program
all: test

# Build test executable and ensure the bin/ directory exists
test: $(OBJ_FILES) $(TEST_OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(TEST_OBJ_FILES) -o $(TEST_TARGET) $(LDFLAGS)

# Ensure bin/ directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile source and test files into object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# logs dir
$(LOGS_DIR):
	mkdir -p $(LOGS_DIR)

# test conf file
TEST_CONF = $(TEST_DIR)/test.conf

# Run tests with Valgrind (from bin/ directory)
valgrind: test $(LOGS_DIR)
	valgrind --leak-check=full --track-origins=yes ./$(TEST_TARGET) $(TEST_CONF) 2>&1 | tee $(LOGS_DIR)/valgrind.txt
	if grep -q 'ERROR SUMMARY: 0 errors from 0 contexts' $(LOGS_DIR)/valgrind.txt; then \
		echo "Valgrind passed with no memory leaks."; \
	else \
		echo "Valgrind found memory leaks or errors!"; \
		exit 1;  # Exit with a non-zero code to fail the job \
	fi

# Clean build and bin files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(LOGS_DIR)

# Phony targets
.PHONY: all test valgrind clean
