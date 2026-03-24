CC = gcc
CFLAGS = -O3 -Wall -std=c99 -fopenmp
LFLAGS = -fopenmp

TARGET = blur.exe
SRC = blur.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LFLAGS)

clean:
	rm -f $(TARGET)