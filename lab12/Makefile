# Makefile — Bisection method with Stack
# Follows the exact style from lecture 12.
#
# Usage:
#   make          — build main.exe
#   make clean    — remove .o files and main.exe
#   make help     — list targets

CC     = gcc
FFLAGS = -O3 -Wall
LFLAGS = -lm

OBJECTS = main.o       \
          GetInput.o   \
          Bisection.o  \
          Function.o   \
          Push.o       \
          Pop.o        \
          Peek.o       \
          DisplayStack.o \
          GetStackSize.o \
          DeleteStack.o

.PHONY: clean help

main.exe: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o main.exe

%.o: %.c
	$(CC) $(FFLAGS) -c $<

clean:
	rm -f $(OBJECTS) main.exe

help:
	@echo "Valid targets:"
	@echo "  main.exe        — build the program"
	@echo "  clean           — remove *.o and main.exe"
