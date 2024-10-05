# Define directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
INCLUDE_DIR := includes/

# Compiler and flags
CC := gcc
CFLAGS := -Wall -I$(INCLUDE_DIR)  # -I$(INCLUDE_DIR) tells the compiler where to find the headers

# Executable name
TARGET := $(BIN_DIR)/cfactory

# Source and object files
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target: build the program
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(TARGET)

# Rule to compile .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the compiled files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: $(TARGET)
	$(TARGET)

cython-build:
	python setup.py build_ext --inplace
cython-clean:
	python setup.py clean --all
	rm -f cfactorypython/_dataset*.so cfactorypython/_dataset.c
cython-test:
	python -m unittest discover /test

