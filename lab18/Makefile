CC = gcc
CFLAGS = -O3 -Wall -std=c99 -fopenmp
LDFLAGS = -lm -fopenmp

TARGET = main.exe
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)