@echo off
echo Building Lab 3 solutions...

cd src

echo.
echo Building object files...
gcc -Wall -Wextra -std=c99 -c utils.c -o utils.o
if %errorlevel% neq 0 (
    echo Error building utils.o
    exit /b 1
)

gcc -Wall -Wextra -std=c99 -c find_min_max.c -o find_min_max.o
if %errorlevel% neq 0 (
    echo Error building find_min_max.o
    exit /b 1
)

echo.
echo Building sequential_min_max...
gcc -Wall -Wextra -std=c99 -o sequential_min_max.exe utils.o find_min_max.o sequential_min_max.c
if %errorlevel% neq 0 (
    echo Error building sequential_min_max
    exit /b 1
)

echo.
echo Building parallel_min_max (Windows version)...
gcc -Wall -Wextra -std=c99 -o parallel_min_max.exe utils.o find_min_max.o parallel_min_max_windows.c
if %errorlevel% neq 0 (
    echo Error building parallel_min_max
    exit /b 1
)

echo.
echo Building exec_sequential (Windows version)...
gcc -Wall -Wextra -std=c99 -o exec_sequential.exe exec_sequential_windows.c
if %errorlevel% neq 0 (
    echo Error building exec_sequential
    exit /b 1
)

echo.
echo All programs built successfully!
echo.
echo To run the programs:
echo   Sequential: sequential_min_max.exe 42 1000
echo   Parallel (pipe): parallel_min_max.exe --seed 42 --array_size 1000 --pnum 4
echo   Parallel (files): parallel_min_max.exe --seed 42 --array_size 1000 --pnum 4 --by_files
echo   Exec: exec_sequential.exe 42 1000
