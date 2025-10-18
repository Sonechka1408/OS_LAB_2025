#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <windows.h>

#include "find_min_max.h"
#include "utils.h"

// Структура для хранения аргументов программы
typedef struct {
    int seed;
    int array_size;
    int pnum;
    bool with_files;
} ProgramArgs;

// Функция для парсинга аргументов командной строки
ProgramArgs parse_arguments(int argc, char **argv) {
    ProgramArgs args = {-1, -1, -1, false};
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--seed") == 0 && i + 1 < argc) {
            args.seed = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--array_size") == 0 && i + 1 < argc) {
            args.array_size = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--pnum") == 0 && i + 1 < argc) {
            args.pnum = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--by_files") == 0) {
            args.with_files = true;
        }
    }
    
    return args;
}

// Функция для работы дочернего процесса
DWORD WINAPI child_process(LPVOID lpParam) {
    struct {
        int *array;
        int array_size;
        int process_id;
        int pnum;
        bool with_files;
    } *params = (struct {
        int *array;
        int array_size;
        int process_id;
        int pnum;
        bool with_files;
    }*)lpParam;
    
    unsigned int begin = params->process_id * params->array_size / params->pnum;
    unsigned int end = (params->process_id + 1) * params->array_size / params->pnum;
    
    struct MinMax min_max = GetMinMax(params->array, begin, end);
    
    if (params->with_files) {
        // Используем файлы для передачи данных
        char filename[256];
        sprintf(filename, "min_max_%d.txt", params->process_id);
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            fprintf(file, "%d %d\n", min_max.min, min_max.max);
            fclose(file);
        }
    } else {
        // Используем именованные pipe для передачи данных
        char pipe_name[256];
        sprintf(pipe_name, "\\\\.\\pipe\\min_max_%d", params->process_id);
        
        HANDLE hPipe = CreateNamedPipe(
            pipe_name,
            PIPE_ACCESS_OUTBOUND,
            PIPE_TYPE_MESSAGE | PIPE_WAIT,
            1, 0, 0, 0, NULL
        );
        
        if (hPipe != INVALID_HANDLE_VALUE) {
            ConnectNamedPipe(hPipe, NULL);
            DWORD bytesWritten;
            WriteFile(hPipe, &min_max.min, sizeof(int), &bytesWritten, NULL);
            WriteFile(hPipe, &min_max.max, sizeof(int), &bytesWritten, NULL);
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);
        }
    }
    
    return 0;
}

int main(int argc, char **argv) {
    ProgramArgs args = parse_arguments(argc, argv);
    
    if (args.seed == -1 || args.array_size == -1 || args.pnum == -1) {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" [--by_files]\n", argv[0]);
        return 1;
    }
    
    if (args.seed <= 0) {
        printf("seed must be a positive number\n");
        return 1;
    }
    
    if (args.array_size <= 0) {
        printf("array_size must be a positive number\n");
        return 1;
    }
    
    if (args.pnum <= 0) {
        printf("pnum must be a positive number\n");
        return 1;
    }
    
    int *array = malloc(sizeof(int) * args.array_size);
    GenerateArray(array, args.array_size, args.seed);
    
    // Измеряем время начала
    LARGE_INTEGER frequency, start_time, end_time;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_time);
    
    // Создаем потоки для каждого процесса
    HANDLE *threads = malloc(sizeof(HANDLE) * args.pnum);
    struct {
        int *array;
        int array_size;
        int process_id;
        int pnum;
        bool with_files;
    } *thread_params = malloc(sizeof(*thread_params) * args.pnum);
    
    for (int i = 0; i < args.pnum; i++) {
        thread_params[i].array = array;
        thread_params[i].array_size = args.array_size;
        thread_params[i].process_id = i;
        thread_params[i].pnum = args.pnum;
        thread_params[i].with_files = args.with_files;
        
        threads[i] = CreateThread(NULL, 0, child_process, &thread_params[i], 0, NULL);
        if (threads[i] == NULL) {
            printf("Failed to create thread %d\n", i);
            return 1;
        }
    }
    
    // Ждем завершения всех потоков
    WaitForMultipleObjects(args.pnum, threads, TRUE, INFINITE);
    
    // Закрываем дескрипторы потоков
    for (int i = 0; i < args.pnum; i++) {
        CloseHandle(threads[i]);
    }
    
    // Измеряем время окончания
    QueryPerformanceCounter(&end_time);
    double elapsed_time = (double)(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart * 1000.0;
    
    // Собираем результаты
    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;
    
    for (int i = 0; i < args.pnum; i++) {
        int min = INT_MAX;
        int max = INT_MIN;
        
        if (args.with_files) {
            // Читаем из файлов
            char filename[256];
            sprintf(filename, "min_max_%d.txt", i);
            FILE *file = fopen(filename, "r");
            if (file != NULL) {
                fscanf(file, "%d %d", &min, &max);
                fclose(file);
                remove(filename); // Удаляем временный файл
            }
        } else {
            // Читаем из именованных pipe
            char pipe_name[256];
            sprintf(pipe_name, "\\\\.\\pipe\\min_max_%d", i);
            
            HANDLE hPipe = CreateFile(
                pipe_name,
                GENERIC_READ,
                0, NULL, OPEN_EXISTING, 0, NULL
            );
            
            if (hPipe != INVALID_HANDLE_VALUE) {
                DWORD bytesRead;
                ReadFile(hPipe, &min, sizeof(int), &bytesRead, NULL);
                ReadFile(hPipe, &max, sizeof(int), &bytesRead, NULL);
                CloseHandle(hPipe);
            }
        }
        
        if (min < min_max.min) min_max.min = min;
        if (max > min_max.max) min_max.max = max;
    }
    
    free(array);
    free(threads);
    free(thread_params);
    
    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %.2fms\n", elapsed_time);
    
    return 0;
}
