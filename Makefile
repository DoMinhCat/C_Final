CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -g -Iinclude -Ihelper

# Directories
SRC_DIR = src
HELPER_DIR = helper
BUILD_DIR = build

# Collect all .c files from src/ and helper/
SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(HELPER_DIR)/*.c)

# Convert e.g. src/db.c → build/db.o
OBJ = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRC)))

# Output exe name
TARGET = MiniDB

# Default rule
all: $(TARGET)

# Link all object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Rule to compile .c → .o inside build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(HELPER_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build/ directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
