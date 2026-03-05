# MinGW (Git Bash) build for Lab 14
# Usage:
#   mingw32-make clean
#   mingw32-make
#   ./main.exe
#   ./main.exe 10

CC = gcc
CFLAGS = -std=c99 -O2 -Wall -Wextra -pedantic
LDFLAGS = -lm

OBJS = main.o matrix.o

all: main.exe

main.exe: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

main.o: main.c matrix.h
	$(CC) $(CFLAGS) -c main.c

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c matrix.c

clean:
	rm -f *.o *.exe
