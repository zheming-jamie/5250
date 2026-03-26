bash code to run:

mingw32-make clean
mingw32-make
./main.exe 1 10000000 > run_1.txt
./main.exe 2 10000000 > run_2.txt
./main.exe 4 10000000 > run_4.txt
./main.exe 8 10000000 > run_8.txt
./main.exe 16 10000000 > run_16.txt