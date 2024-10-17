# Makefile made using GPT-4o

# Compiler
CC = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# Library directories
LIB_DIR = lib
LIBS := $(wildcard $(LIB_DIR)/*)  # Find all libraries (e.g., clogger, anotherlib)

# Gather source and include directories for each library
LIB_SRCS := $(foreach lib, $(LIBS), $(wildcard $(lib)/src/*.c))
LIB_INCLUDES := $(foreach lib, $(LIBS), -I$(lib)/include)
LIB_OBJS := $(patsubst $(LIB_DIR)/%/src/%.c, $(BUILD_DIR)/%/%.o, $(LIB_SRCS))

# Source files for the main project
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Output binary
MAIN_FILE := $(shell grep -l 'int main' $(SRC_DIR)/*.c)

TARGET = $(BIN_DIR)/$(basename $(notdir $(MAIN_FILE)))

# Compiler flags
CFLAGS = -I$(INCLUDE_DIR) $(LIB_INCLUDES)

# Default target
all: $(TARGET)

# Link object files (both main and library objects) into the final executable
$(TARGET): $(OBJS) $(LIB_OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LIB_OBJS) -o $(TARGET)

# Compile main source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile library source files into object files
$(BUILD_DIR)/%/%.o: $(LIB_DIR)/%/src/%.c | $(BUILD_DIR)/%
	$(CC) $(CFLAGS) -c $< -o $@

# Create necessary directories
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create library-specific build directories
$(BUILD_DIR)/%:
	mkdir -p $@

# Clean target to remove build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
