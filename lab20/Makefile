CC = gcc
CFLAGS = -O2 -std=c11 -Wall -Wextra -fopenmp
LDFLAGS = -lm -fopenmp
TARGET = main.exe

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o *.data *.csv *.png *.txt
