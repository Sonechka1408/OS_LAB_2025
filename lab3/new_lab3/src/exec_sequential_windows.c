#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

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

    // Создаем командную строку для запуска sequential_min_max
    char command[512];
    sprintf(command, "sequential_min_max.exe %s %s", argv[1], argv[2]);

    // Запускаем процесс
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Создаем процесс
    if (!CreateProcess(
        NULL,           // Имя приложения
        command,        // Командная строка
        NULL,           // Атрибуты безопасности процесса
        NULL,           // Атрибуты безопасности потока
        FALSE,          // Наследование дескрипторов
        0,              // Флаги создания
        NULL,           // Блок окружения
        NULL,           // Текущий каталог
        &si,            // Информация о запуске
        &pi             // Информация о процессе
    )) {
        printf("CreateProcess failed (%d)\n", GetLastError());
        return 1;
    }

    // Ждем завершения процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Получаем код завершения
    DWORD exit_code;
    GetExitCodeProcess(pi.hProcess, &exit_code);

    // Закрываем дескрипторы
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("----------------------------------------\n");
    printf("sequential_min_max completed with exit code: %d\n", exit_code);
    
    return exit_code;
}
