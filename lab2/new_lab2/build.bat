@echo off
echo Building Lab 2 solutions...

echo.
echo Building Task 1 (Swap function)...
cd task1_swap
gcc -Wall -Wextra -std=c99 main.c swap.c -o swap_program.exe
if %errorlevel% neq 0 (
    echo Error building Task 1
    exit /b 1
)
echo Task 1 built successfully

echo.
echo Building Task 2 (Revert string)...
cd ..\task2_revert
gcc -Wall -Wextra -std=c99 main.c revert_string.c -o revert_program.exe
if %errorlevel% neq 0 (
    echo Error building Task 2
    exit /b 1
)
echo Task 2 built successfully

echo.
echo Building Task 3 (Static library)...
cd ..\task3_libraries\static
gcc -Wall -Wextra -std=c99 -c revert_string.c
ar rcs librevert.a revert_string.o
gcc -Wall -Wextra -std=c99 main.c -L. -lrevert -o static_program.exe
if %errorlevel% neq 0 (
    echo Error building Task 3 (Static)
    exit /b 1
)
echo Task 3 (Static) built successfully

echo.
echo Building Task 3 (Dynamic library)...
cd ..\dynamic
gcc -Wall -Wextra -std=c99 -fPIC -c revert_string.c
gcc -shared -fPIC revert_string.o -o librevert.dll
gcc -Wall -Wextra -std=c99 main.c -L. -lrevert -o dynamic_program.exe
if %errorlevel% neq 0 (
    echo Error building Task 3 (Dynamic)
    exit /b 1
)
echo Task 3 (Dynamic) built successfully

echo.
echo All tasks built successfully!
echo.
echo To run the programs:
echo   Task 1: cd task1_swap && swap_program.exe
echo   Task 2: cd task2_revert && revert_program.exe "Hello World"
echo   Task 3 Static: cd task3_libraries\static && static_program.exe "Hello World"
echo   Task 3 Dynamic: cd task3_libraries\dynamic && dynamic_program.exe "Hello World"
