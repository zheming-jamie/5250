mingw32-make clean
mingw32-make
./main.exe 1 100000000 > run_1.txt
./main.exe 2 100000000 > run_2.txt
./main.exe 4 100000000 > run_4.txt
./main.exe 8 100000000 > run_8.txt
./main.exe 16 100000000 > run_16.txt