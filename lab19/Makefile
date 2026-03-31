CC = gcc
CFLAGS = -O3 -Wall -std=c99 -fopenmp
LDFLAGS = -fopenmp -lm

OBJECTS = main.o

main.exe: $(OBJECTS)
	$(CC) $(OBJECTS) -o main.exe $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	del /Q *.o *.exe 2>nul || rm -f *.o *.exe