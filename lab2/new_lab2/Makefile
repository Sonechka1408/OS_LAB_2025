# Makefile for Lab 2 - C Programming

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = 

# Task 1: Swap function
TASK1_DIR = task1_swap
TASK1_TARGET = $(TASK1_DIR)/swap_program
TASK1_OBJECTS = $(TASK1_DIR)/main.o $(TASK1_DIR)/swap.o

# Task 2: Revert string
TASK2_DIR = task2_revert
TASK2_TARGET = $(TASK2_DIR)/revert_program
TASK2_OBJECTS = $(TASK2_DIR)/main.o $(TASK2_DIR)/revert_string.o

# Task 3: Static library
TASK3_STATIC_DIR = task3_libraries/static
TASK3_STATIC_TARGET = $(TASK3_STATIC_DIR)/static_program
TASK3_STATIC_OBJECTS = $(TASK3_STATIC_DIR)/main.o
TASK3_STATIC_LIB = $(TASK3_STATIC_DIR)/librevert.a

# Task 3: Dynamic library
TASK3_DYNAMIC_DIR = task3_libraries/dynamic
TASK3_DYNAMIC_TARGET = $(TASK3_DYNAMIC_DIR)/dynamic_program
TASK3_DYNAMIC_OBJECTS = $(TASK3_DYNAMIC_DIR)/main.o
TASK3_DYNAMIC_LIB = $(TASK3_DYNAMIC_DIR)/librevert.dll

.PHONY: all clean task1 task2 task3_static task3_dynamic

all: task1 task2 task3_static task3_dynamic

# Task 1: Swap function
task1: $(TASK1_TARGET)

$(TASK1_TARGET): $(TASK1_OBJECTS)
	$(CC) $(TASK1_OBJECTS) -o $@

$(TASK1_DIR)/main.o: $(TASK1_DIR)/main.c $(TASK1_DIR)/swap.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TASK1_DIR)/swap.o: $(TASK1_DIR)/swap.c $(TASK1_DIR)/swap.h
	$(CC) $(CFLAGS) -c $< -o $@

# Task 2: Revert string
task2: $(TASK2_TARGET)

$(TASK2_TARGET): $(TASK2_OBJECTS)
	$(CC) $(TASK2_OBJECTS) -o $@

$(TASK2_DIR)/main.o: $(TASK2_DIR)/main.c $(TASK2_DIR)/revert_string.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TASK2_DIR)/revert_string.o: $(TASK2_DIR)/revert_string.c $(TASK2_DIR)/revert_string.h
	$(CC) $(CFLAGS) -c $< -o $@

# Task 3: Static library
task3_static: $(TASK3_STATIC_TARGET)

$(TASK3_STATIC_TARGET): $(TASK3_STATIC_OBJECTS) $(TASK3_STATIC_LIB)
	$(CC) $(TASK3_STATIC_OBJECTS) -L$(TASK3_STATIC_DIR) -lrevert -o $@

$(TASK3_STATIC_LIB): $(TASK3_STATIC_DIR)/revert_string.o
	ar rcs $@ $<

$(TASK3_STATIC_DIR)/main.o: $(TASK3_STATIC_DIR)/main.c $(TASK3_STATIC_DIR)/revert_string.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TASK3_STATIC_DIR)/revert_string.o: $(TASK3_STATIC_DIR)/revert_string.c $(TASK3_STATIC_DIR)/revert_string.h
	$(CC) $(CFLAGS) -c $< -o $@

# Task 3: Dynamic library
task3_dynamic: $(TASK3_DYNAMIC_TARGET)

$(TASK3_DYNAMIC_TARGET): $(TASK3_DYNAMIC_OBJECTS) $(TASK3_DYNAMIC_LIB)
	$(CC) $(TASK3_DYNAMIC_OBJECTS) -L$(TASK3_DYNAMIC_DIR) -lrevert -o $@

$(TASK3_DYNAMIC_LIB): $(TASK3_DYNAMIC_DIR)/revert_string.o
	$(CC) -shared -fPIC $< -o $@

$(TASK3_DYNAMIC_DIR)/main.o: $(TASK3_DYNAMIC_DIR)/main.c $(TASK3_DYNAMIC_DIR)/revert_string.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TASK3_DYNAMIC_DIR)/revert_string.o: $(TASK3_DYNAMIC_DIR)/revert_string.c $(TASK3_DYNAMIC_DIR)/revert_string.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Clean all generated files
clean:
	rm -f $(TASK1_DIR)/*.o $(TASK1_TARGET)
	rm -f $(TASK2_DIR)/*.o $(TASK2_TARGET)
	rm -f $(TASK3_STATIC_DIR)/*.o $(TASK3_STATIC_TARGET) $(TASK3_STATIC_LIB)
	rm -f $(TASK3_DYNAMIC_DIR)/*.o $(TASK3_DYNAMIC_TARGET) $(TASK3_DYNAMIC_LIB)

# Test targets
test_task1: task1
	@echo "Testing Task 1 (Swap function):"
	@cd $(TASK1_DIR) && ./swap_program

test_task2: task2
	@echo "Testing Task 2 (Revert string):"
	@cd $(TASK2_DIR) && ./revert_program "Hello World"

test_task3_static: task3_static
	@echo "Testing Task 3 (Static library):"
	@cd $(TASK3_STATIC_DIR) && ./static_program "Hello World"

test_task3_dynamic: task3_dynamic
	@echo "Testing Task 3 (Dynamic library):"
	@cd $(TASK3_DYNAMIC_DIR) && LD_LIBRARY_PATH=. ./dynamic_program "Hello World"

test_all: test_task1 test_task2 test_task3_static test_task3_dynamic
