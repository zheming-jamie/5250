CC = gcc
CFLAGS = -std=c99 -O2

# Build a Windows-friendly executable name to match your previous workflow
TARGET = main.exe

all: $(TARGET)

$(TARGET): deblur.o matrix.o normal_distribution.o
	$(CC) $(CFLAGS) $^ -lm -o $@

deblur.o: deblur.c matrix.h
	$(CC) $(CFLAGS) -c deblur.c

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c matrix.c

# normal_distribution.c has its demo main() renamed to normal_distribution_main()
normal_distribution.o: normal_distribution.c
	$(CC) $(CFLAGS) -c normal_distribution.c

clean:
	rm -f *.o *.exe deblur
