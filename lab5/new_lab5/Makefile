CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -pthread
SRC_DIR = src

.PHONY: all clean

all: mutex_no_mutex mutex_with_mutex factorial_mod deadlock_demo

mutex_no_mutex: $(SRC_DIR)/mutex_demo.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

mutex_with_mutex: $(SRC_DIR)/mutex_demo.c
	$(CC) $(CFLAGS) -DUSE_MUTEX $< -o $@ $(LDFLAGS)

factorial_mod: $(SRC_DIR)/factorial_mod.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

deadlock_demo: $(SRC_DIR)/deadlock_demo.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	$(RM) mutex_no_mutex mutex_with_mutex factorial_mod deadlock_demo

