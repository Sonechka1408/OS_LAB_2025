#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s seed arraysize\n", argv[0]);
        printf("This program will launch sequential_min_max with the given parameters\n");
        return 1;
    }

    int seed = atoi(argv[1]);
    if (seed <= 0) {
        printf("seed must be a positive number\n");
        return 1;
    }

    int array_size = atoi(argv[2]);
    if (array_size <= 0) {
        printf("array_size must be a positive number\n");
        return 1;
    }

    printf("Launching sequential_min_max with seed=%d, array_size=%d\n", seed, array_size);
    printf("----------------------------------------\n");

    pid_t child_pid = fork();
    
    if (child_pid == -1) {
        perror("fork failed");
        return 1;
    }
    
    if (child_pid == 0) {
        // Child process - execute sequential_min_max
        char *args[] = {"./sequential_min_max", argv[1], argv[2], NULL};
        
        // Execute the sequential_min_max program
        if (execv("./sequential_min_max", args) == -1) {
            perror("execv failed");
            exit(1);
        }
    } else {
        // Parent process - wait for child to complete
        int status;
        pid_t waited_pid = wait(&status);
        
        if (waited_pid == -1) {
            perror("wait failed");
            return 1;
        }
        
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("----------------------------------------\n");
            printf("sequential_min_max completed with exit status: %d\n", exit_status);
            return exit_status;
        } else {
            printf("sequential_min_max terminated abnormally\n");
            return 1;
        }
    }
    
    return 0;
}
