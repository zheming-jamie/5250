CC = mpicc
CFLAGS = -O2 -Wall -Wextra -std=c99
LDFLAGS =
OBJECTS = main.o

main.exe: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o main.exe

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) main.exe
