CC = gcc
CFLAGS = -O2 -std=c11 -Wall -Wextra -pedantic
LDFLAGS = -lm

SRC = main.c matrix.c trimatrix.c FindZero.c GetShift.c Hessenberg.c QRA.c QRdecomp.c
OBJ = $(SRC:.c=.o)
TARGET = main.exe
OUT = output.txt

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) > $(OUT) 2>&1

clean:
	rm -f $(OBJ) $(TARGET) $(OUT)