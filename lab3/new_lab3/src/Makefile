# Makefile for Lab 3 - Parallel Min/Max Search

CC=gcc
CFLAGS=-I. -Wall -Wextra -std=c99

# Object files
UTILS_OBJ = utils.o
FIND_MIN_MAX_OBJ = find_min_max.o

# Executables
SEQUENTIAL_TARGET = sequential_min_max
PARALLEL_TARGET = parallel_min_max
EXEC_TARGET = exec_sequential

# Default target
all: $(SEQUENTIAL_TARGET) $(PARALLEL_TARGET) $(EXEC_TARGET)

# Sequential min max program
$(SEQUENTIAL_TARGET): $(UTILS_OBJ) $(FIND_MIN_MAX_OBJ) sequential_min_max.c
	$(CC) -o $(SEQUENTIAL_TARGET) $(UTILS_OBJ) $(FIND_MIN_MAX_OBJ) sequential_min_max.c $(CFLAGS)

# Parallel min max program
$(PARALLEL_TARGET): $(UTILS_OBJ) $(FIND_MIN_MAX_OBJ) parallel_min_max.c
	$(CC) -o $(PARALLEL_TARGET) $(UTILS_OBJ) $(FIND_MIN_MAX_OBJ) parallel_min_max.c $(CFLAGS)

# Exec program
$(EXEC_TARGET): exec_sequential.c
	$(CC) -o $(EXEC_TARGET) exec_sequential.c $(CFLAGS)

# Object files
$(UTILS_OBJ): utils.c utils.h
	$(CC) -o $(UTILS_OBJ) -c utils.c $(CFLAGS)

$(FIND_MIN_MAX_OBJ): find_min_max.c find_min_max.h utils.h
	$(CC) -o $(FIND_MIN_MAX_OBJ) -c find_min_max.c $(CFLAGS)

# Clean target
clean:
	rm -f $(UTILS_OBJ) $(FIND_MIN_MAX_OBJ) $(SEQUENTIAL_TARGET) $(PARALLEL_TARGET) $(EXEC_TARGET)
	rm -f min_max_*.txt

# Test targets
test_sequential: $(SEQUENTIAL_TARGET)
	@echo "Testing sequential min max with seed=42, array_size=1000"
	./$(SEQUENTIAL_TARGET) 42 1000

test_parallel_pipe: $(PARALLEL_TARGET)
	@echo "Testing parallel min max with pipes (seed=42, array_size=1000, pnum=4)"
	./$(PARALLEL_TARGET) --seed 42 --array_size 1000 --pnum 4

test_parallel_files: $(PARALLEL_TARGET)
	@echo "Testing parallel min max with files (seed=42, array_size=1000, pnum=4)"
	./$(PARALLEL_TARGET) --seed 42 --array_size 1000 --pnum 4 --by_files

test_exec: $(EXEC_TARGET)
	@echo "Testing exec program"
	./$(EXEC_TARGET) 42 1000

test_all: test_sequential test_parallel_pipe test_parallel_files test_exec

# Help target
help:
	@echo "Available targets:"
	@echo "  all              - Build all programs"
	@echo "  sequential_min_max - Build sequential min max program"
	@echo "  parallel_min_max   - Build parallel min max program"
	@echo "  exec_sequential    - Build exec program"
	@echo "  clean            - Remove all generated files"
	@echo "  test_sequential  - Test sequential program"
	@echo "  test_parallel_pipe - Test parallel program with pipes"
	@echo "  test_parallel_files - Test parallel program with files"
	@echo "  test_exec        - Test exec program"
	@echo "  test_all         - Run all tests"
	@echo "  help             - Show this help"

.PHONY: all clean test_sequential test_parallel_pipe test_parallel_files test_exec test_all help
