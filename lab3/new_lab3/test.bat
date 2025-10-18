@echo off
echo Testing Lab 3 solutions...

cd src

echo.
echo Testing sequential_min_max...
echo Running: sequential_min_max.exe 42 1000
sequential_min_max.exe 42 1000
echo.

echo Testing parallel_min_max with pipes...
echo Running: parallel_min_max.exe --seed 42 --array_size 1000 --pnum 4
parallel_min_max.exe --seed 42 --array_size 1000 --pnum 4
echo.

echo Testing parallel_min_max with files...
echo Running: parallel_min_max.exe --seed 42 --array_size 1000 --pnum 4 --by_files
parallel_min_max.exe --seed 42 --array_size 1000 --pnum 4 --by_files
echo.

echo Testing exec_sequential...
echo Running: exec_sequential.exe 42 1000
exec_sequential.exe 42 1000
echo.

echo All tests completed!
