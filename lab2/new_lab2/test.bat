@echo off
echo Testing Lab 2 solutions...

echo.
echo Testing Task 1 (Swap function)...
cd task1_swap
echo Running: swap_program.exe
swap_program.exe
echo.

echo Testing Task 2 (Revert string)...
cd ..\task2_revert
echo Running: revert_program.exe "Hello World"
revert_program.exe "Hello World"
echo.

echo Testing Task 3 (Static library)...
cd ..\task3_libraries\static
echo Running: static_program.exe "Hello World"
static_program.exe "Hello World"
echo.

echo Testing Task 3 (Dynamic library)...
cd ..\dynamic
echo Running: dynamic_program.exe "Hello World"
dynamic_program.exe "Hello World"
echo.

echo All tests completed!
