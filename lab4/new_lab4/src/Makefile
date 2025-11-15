CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Задание 1 и 3: parallel_min_max и process_memory
all: parallel_min_max process_memory

# Задание 1: parallel_min_max с таймаутом
parallel_min_max: parallel_min_max.o find_min_max.o utils.o
	$(CC) $(CFLAGS) -o parallel_min_max parallel_min_max.o find_min_max.o utils.o

parallel_min_max.o: parallel_min_max.c find_min_max.h utils.h
	$(CC) $(CFLAGS) -c parallel_min_max.c

find_min_max.o: find_min_max.c find_min_max.h utils.h
	$(CC) $(CFLAGS) -c find_min_max.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

# Задание 3: process_memory
process_memory: process_memory.c
	$(CC) $(CFLAGS) -o process_memory process_memory.c

clean:
	rm -f *.o parallel_min_max process_memory
	rm -f min_max_*.txt

.PHONY: all clean


